/**
 *  @file
 *  @copyright defined in enumivo/LICENSE.txt
 */
#include <eosio/producer_plugin/producer_plugin.hpp>
#include <eosio/chain/producer_object.hpp>
#include <eosio/chain/plugin_interface.hpp>

#include <fc/io/json.hpp>
#include <fc/smart_ref_impl.hpp>
#include <fc/scoped_exit.hpp>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <algorithm>
#include <boost/range/adaptor/map.hpp>
#include <boost/function_output_iterator.hpp>

using std::string;
using std::vector;

namespace eosio {

static appbase::abstract_plugin& _producer_plugin = app().register_plugin<producer_plugin>();

using namespace eosio::chain;
using namespace eosio::chain::plugin_interface;

class producer_plugin_impl {
   public:
      producer_plugin_impl(boost::asio::io_service& io)
      :_timer(io)
      {}

      void schedule_production_loop();
      block_production_condition::block_production_condition_enum block_production_loop();
      block_production_condition::block_production_condition_enum maybe_produce_block(fc::mutable_variant_object& capture);

      boost::program_options::variables_map _options;
      bool     _production_enabled                 = false;
      uint32_t _required_producer_participation    = uint32_t(config::required_producer_participation);
      uint32_t _production_skip_flags              = 0; //eosio::chain::skip_nothing;

      std::map<chain::public_key_type, chain::private_key_type> _private_keys;
      std::set<chain::account_name>                             _producers;
      boost::asio::deadline_timer                               _timer;
      std::map<chain::account_name, uint32_t>                   _producer_watermarks;

      int32_t                                                   _max_deferred_transaction_time_ms;
      int32_t                                                   _max_pending_transaction_time_ms;

      block_production_condition::block_production_condition_enum _prev_result = block_production_condition::produced;
      uint32_t _prev_result_count = 0;

      time_point _last_signed_block_time;
      time_point _start_time = fc::time_point::now();
      uint32_t   _last_signed_block_num = 0;

      producer_plugin* _self = nullptr;

      channels::incoming_block::channel_type::handle          _incoming_block_subscription;
      channels::incoming_transaction::channel_type::handle    _incoming_transaction_subscription;

      methods::incoming_block_sync::method_type::handle       _incoming_block_sync_provider;
      methods::incoming_transaction_sync::method_type::handle _incoming_transaction_sync_provider;
      methods::start_coordinator::method_type::handle         _start_coordinator_provider;

      void startup();

      void on_block( const block_state_ptr& bsp ) {
         if( bsp->header.timestamp <= _last_signed_block_time ) return;
         if( bsp->header.timestamp <= _start_time ) return;
         if( bsp->block_num <= _last_signed_block_num ) return;

         const auto& active_producer_to_signing_key = bsp->active_schedule.producers;

         flat_set<account_name> active_producers;
         active_producers.reserve(bsp->active_schedule.producers.size());
         for (const auto& p: bsp->active_schedule.producers) {
            active_producers.insert(p.producer_name);
         }

         std::set_intersection( _producers.begin(), _producers.end(),
                                active_producers.begin(), active_producers.end(),
                                boost::make_function_output_iterator( [&]( const chain::account_name& producer )
         {
            if( producer != bsp->header.producer ) {
               auto itr = std::find_if( active_producer_to_signing_key.begin(), active_producer_to_signing_key.end(),
                                        [&](const producer_key& k){ return k.producer_name == producer; } );
               if( itr != active_producer_to_signing_key.end() ) {
                  auto private_key_itr = _private_keys.find( itr->block_signing_key );
                  if( private_key_itr != _private_keys.end() ) {
                     auto d = bsp->sig_digest();
                     auto sig = private_key_itr->second.sign( d );
                     _last_signed_block_time = bsp->header.timestamp;
                     _last_signed_block_num  = bsp->block_num;

   //                  ilog( "${n} confirmed", ("n",name(producer)) );
                     _self->confirmed_block( { bsp->id, d, producer, sig } );
                  }
               }
            }
         } ) );
      }

      void on_incoming_block(const signed_block_ptr& block) {
         chain::controller& chain = app().get_plugin<chain_plugin>().chain();
         // abort the pending block
         chain.abort_block();

         // exceptions throw out, make sure we restart our loop
         auto ensure = fc::make_scoped_exit([this](){
            // restart our production loop
            schedule_production_loop();
         });
         // push the new block
         chain.push_block(block);

         ilog("Received block ${id}... #${n} @ ${t} signed by ${p} [trxs: ${count}, lib: ${lib}, confirmed: ${confs}]",
              ("p",block->producer)("id",fc::variant(block->id()).as_string().substr(0,16))
              ("n",block_header::num_from_id(block->id()))("t",block->timestamp)
              ("count",block->transactions.size())("lib",chain.last_irreversible_block_num())("confs", block->confirmed) );

      }

      transaction_trace_ptr on_incoming_transaction(const packed_transaction_ptr& trx) {
         chain::controller& chain = app().get_plugin<chain_plugin>().chain();
         return chain.sync_push(std::make_shared<transaction_metadata>(*trx), fc::time_point::now() + fc::milliseconds(_max_pending_transaction_time_ms));
      }
};

void new_chain_banner(const eosio::chain::controller& db)
{
   std::cerr << "\n"
      "*******************************\n"
      "*                             *\n"
      "*   ------ NEW CHAIN ------   *\n"
      "*   -  Welcome to EOSIO!  -   *\n"
      "*   -----------------------   *\n"
      "*                             *\n"
      "*******************************\n"
      "\n";

   if( db.head_block_state()->header.timestamp.to_time_point() < (fc::time_point::now() - fc::milliseconds(200 * config::block_interval_ms)))
   {
      std::cerr << "Your genesis seems to have an old timestamp\n"
         "Please consider using the --genesis-timestamp option to give your genesis a recent timestamp\n"
         "\n"
         ;
   }
   return;
}

producer_plugin::producer_plugin()
   : my(new producer_plugin_impl(app().get_io_service())){
      my->_self = this;
   }

producer_plugin::~producer_plugin() {}

void producer_plugin::set_program_options(
   boost::program_options::options_description& command_line_options,
   boost::program_options::options_description& config_file_options)
{
   auto default_priv_key = private_key_type::regenerate<fc::ecc::private_key_shim>(fc::sha256::hash(std::string("nathan")));
   auto private_key_default = std::make_pair(default_priv_key.get_public_key(), default_priv_key );

   boost::program_options::options_description producer_options;

   producer_options.add_options()
         ("enable-stale-production,e", boost::program_options::bool_switch()->notifier([this](bool e){my->_production_enabled = e;}), "Enable block production, even if the chain is stale.")
         ("max-pending-transaction-time", bpo::value<int32_t>()->default_value(30),
          "Limits the maximum time (in milliseconds) that is allowed a pushed transaction's code to execute before being considered invalid")
         ("max-deferred-transaction-time", bpo::value<int32_t>()->default_value(20),
          "Limits the maximum time (in milliseconds) that is allowed a to push deferred transactions at the start of a block")
         ("required-participation", boost::program_options::value<uint32_t>()
                                       ->default_value(uint32_t(config::required_producer_participation/config::percent_1))
                                       ->notifier([this](uint32_t e) {
                                          my->_required_producer_participation = std::min(e, 100u) * config::percent_1;
                                       }),
                                       "Percent of producers (0-100) that must be participating in order to produce blocks")
         ("producer-name,p", boost::program_options::value<vector<string>>()->composing()->multitoken(),
          "ID of producer controlled by this node (e.g. inita; may specify multiple times)")
         ("private-key", boost::program_options::value<vector<string>>()->composing()->multitoken()->default_value({fc::json::to_string(private_key_default)},
                                                                                                fc::json::to_string(private_key_default)),
          "Tuple of [public key, WIF private key] (may specify multiple times)")
         ;
   config_file_options.add(producer_options);
}

bool producer_plugin::is_producer_key(const chain::public_key_type& key) const
{
  auto private_key_itr = my->_private_keys.find(key);
  if(private_key_itr != my->_private_keys.end())
    return true;
  return false;
}

chain::signature_type producer_plugin::sign_compact(const chain::public_key_type& key, const fc::sha256& digest) const
{
  if(key != chain::public_key_type()) {
    auto private_key_itr = my->_private_keys.find(key);
    FC_ASSERT(private_key_itr != my->_private_keys.end(), "Local producer has no private key in config.ini corresponding to public key ${key}", ("key", key));

    return private_key_itr->second.sign(digest);
  }
  else {
    return chain::signature_type();
  }
}

template<typename T>
T dejsonify(const string& s) {
   return fc::json::from_string(s).as<T>();
}

#define LOAD_VALUE_SET(options, name, container, type) \
if( options.count(name) ) { \
   const std::vector<std::string>& ops = options[name].as<std::vector<std::string>>(); \
   std::copy(ops.begin(), ops.end(), std::inserter(container, container.end())); \
}

void producer_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{ try {
   my->_options = &options;
   LOAD_VALUE_SET(options, "producer-name", my->_producers, types::account_name)

   if( options.count("private-key") )
   {
      const std::vector<std::string> key_id_to_wif_pair_strings = options["private-key"].as<std::vector<std::string>>();
      for (const std::string& key_id_to_wif_pair_string : key_id_to_wif_pair_strings)
      {
         auto key_id_to_wif_pair = dejsonify<std::pair<public_key_type, private_key_type>>(key_id_to_wif_pair_string);
         my->_private_keys[key_id_to_wif_pair.first] = key_id_to_wif_pair.second;
      }
   }

   my->_max_deferred_transaction_time_ms = options.at("max-deferred-transaction-time").as<int32_t>();
   my->_max_pending_transaction_time_ms = options.at("max-pending-transaction-time").as<int32_t>();


   my->_incoming_block_subscription = app().get_channel<channels::incoming_block>().subscribe([this](const signed_block_ptr& block){
      try {
         my->on_incoming_block(block);
      } FC_LOG_AND_DROP();
   });

   my->_incoming_transaction_subscription = app().get_channel<channels::incoming_transaction>().subscribe([this](const packed_transaction_ptr& trx){
      try {
         my->on_incoming_transaction(trx);
      } FC_LOG_AND_DROP();
   });

   static const int my_priority = 1;

   my->_incoming_block_sync_provider = app().get_method<methods::incoming_block_sync>().register_provider([this](const signed_block_ptr& block){
      my->on_incoming_block(block);
   }, my_priority);

   my->_incoming_transaction_sync_provider = app().get_method<methods::incoming_transaction_sync>().register_provider([this](const packed_transaction_ptr& trx) -> transaction_trace_ptr {
      return my->on_incoming_transaction(trx);
   }, my_priority);


   my->_start_coordinator_provider = app().get_method<methods::start_coordinator>().register_provider([this]() {
      my->startup();
   }, my_priority);


} FC_LOG_AND_RETHROW() }

void producer_plugin::plugin_startup()
{ try {
   ilog("producer plugin:  plugin_startup() begin");

   ilog("producer plugin:  plugin_startup() end");
} FC_CAPTURE_AND_RETHROW() }

void producer_plugin::plugin_shutdown() {
   try {
      my->_timer.cancel();
   } catch(fc::exception& e) {
      edump((e.to_detail_string()));
   }
}

void producer_plugin_impl::startup() {
   chain::controller& chain = app().get_plugin<chain_plugin>().chain();
   chain.accepted_block.connect( [this]( const auto& bsp ){ on_block( bsp ); } );

   if (!_producers.empty())
   {
      ilog("Launching block production for ${n} producers.", ("n", _producers.size()));
      if(_production_enabled)
      {
         if(chain.head_block_num() == 0)
            new_chain_banner(chain);
         //_production_skip_flags |= eosio::chain::skip_undo_history_check;
      }
      schedule_production_loop();
   } else
      elog("No producers configured! Please add producer IDs and private keys to configuration.");

}

void producer_plugin_impl::schedule_production_loop() {
   _timer.cancel();

   //Schedule for the next second's tick regardless of chain state
   // If we would wait less than 50ms (1/10 of block_interval), wait for the whole block interval.
   fc::time_point now = fc::time_point::now();
   int64_t time_to_next_block_time = (config::block_interval_us) - (now.time_since_epoch().count() % (config::block_interval_us) );

   if(time_to_next_block_time < config::block_interval_us/10 ) {     // we must sleep for at least 50ms
      ilog("Less than ${t}us to next block time, time_to_next_block_time ${bt}us",
           ("t", config::block_interval_us/10)("bt", time_to_next_block_time));
      time_to_next_block_time += config::block_interval_us;
   }

   fc::time_point block_time = now + fc::microseconds(time_to_next_block_time);
   static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
   _timer.expires_at( epoch + boost::posix_time::microseconds(block_time.time_since_epoch().count()));

   chain::controller& chain = app().get_plugin<chain_plugin>().chain();
   try {
      // determine how many blocks this producer can confirm
      // 1) if it is not a producer from this node, assume no confirmations (we will discard this block anyway)
      // 2) if it is a producer on this node that has never produced, the conservative approach is to assume no
      //    confirmations to make sure we don't double sign after a crash TODO: make these watermarks durable?
      // 3) if it is a producer on this node where this node knows the last block it produced, safely set it -UNLESS-
      // 4) the producer on this node's last watermark is higher (meaning on a different fork)
      const auto& hbs = chain.head_block_state();
      auto producer_name = hbs->get_scheduled_producer(block_time).producer_name;
      uint16_t blocks_to_confirm = 0;
      auto itr = _producer_watermarks.find(producer_name);
      if (itr != _producer_watermarks.end()) {
         auto watermark = itr->second;
         if (watermark < hbs->block_num) {
            blocks_to_confirm = std::min<uint16_t>(std::numeric_limits<uint16_t>::max(), (uint16_t)(hbs->block_num - watermark));
         }
      }

      chain.abort_block();
      chain.start_block(block_time, blocks_to_confirm);
   } FC_LOG_AND_DROP();

   if (chain.pending_block_state()) {
      // TODO:  BIG BAD WARNING, THIS WILL HAPPILY BLOW PAST DEADLINES BUT CONTROLLER IS NOT YET SAFE FOR DEADLINE USAGE
      try {
         while (chain.push_next_unapplied_transaction(fc::time_point::maximum()));
      } FC_LOG_AND_DROP();

      try {
         while (chain.push_next_scheduled_transaction(fc::time_point::maximum()));
      } FC_LOG_AND_DROP();


      //_timer.async_wait(boost::bind(&producer_plugin_impl::block_production_loop, this));
      _timer.async_wait([&](const boost::system::error_code& ec) {
         if (ec != boost::asio::error::operation_aborted) {
            block_production_loop();
         }
      });
   } else {
      elog("Failed to start a pending block, will try again later");
      // we failed to start a block, so try again later?
      _timer.async_wait([&](const boost::system::error_code& ec) {
         if (ec != boost::asio::error::operation_aborted) {
            schedule_production_loop();
         }
      });
   }
}

block_production_condition::block_production_condition_enum producer_plugin_impl::block_production_loop() {
   block_production_condition::block_production_condition_enum result;
   fc::mutable_variant_object capture;
   try
   {
      result = maybe_produce_block(capture);
   }
   catch( const fc::canceled_exception& )
   {
      //We're trying to exit. Go ahead and let this one out.
      throw;
   }
   catch( const fc::exception& e )
   {
      elog("Got exception while generating block:\n${e}", ("e", e.to_detail_string()));
      result = block_production_condition::exception_producing_block;
   }
   if(result != block_production_condition::produced && result == _prev_result) {
      _prev_result_count++;
   }
   else {
      /*
      if (_prev_result_count > 1) {
         ilog("Previous result occurred ${r} times",("r", _prev_result_count));
      }
      */
      _prev_result_count = 1;
      _prev_result = result;
      switch(result)
         {
         case block_production_condition::produced: {
            const auto& db = app().get_plugin<chain_plugin>().chain();
            auto producer  = db.head_block_producer();
            auto pending   = db.head_block_state();

            ilog("Produced block ${id}... #${n} @ ${t} signed by ${p} [trxs: ${count}, lib: ${lib}, confirmed: ${confs}]",
                 ("p",producer)("id",fc::variant(pending->id).as_string().substr(0,16))
                 ("n",pending->block_num)("t",pending->block->timestamp)
                 ("count",pending->block->transactions.size())("lib",db.last_irreversible_block_num())("confs", pending->header.confirmed)(capture) );
            break;
         }
         case block_production_condition::not_synced:
            ilog("Not producing block because production is disabled until we receive a recent block (see: --enable-stale-production)");
            break;
         case block_production_condition::not_my_turn:
       //     ilog("Not producing block because it isn't my turn, its ${scheduled_producer}", (capture) );
            break;
         case block_production_condition::not_time_yet:
        //    ilog("Not producing block because slot has not yet arrived");
            break;
         case block_production_condition::no_private_key:
            ilog("Not producing block because I don't have the private key for ${scheduled_key}", (capture) );
            break;
         case block_production_condition::low_participation:
            elog("Not producing block because node appears to be on a minority fork with only ${pct}% producer participation", (capture) );
            break;
         case block_production_condition::lag:
            elog("Not producing block because node didn't wake up within 500ms of the slot time.");
            break;
         case block_production_condition::exception_producing_block:
            elog( "exception producing block" );
            break;
         case block_production_condition::fork_below_watermark:
            elog( "Not producing block because \"${producer}\" signed a BFT confirmation OR block at a higher block number (${watermark}) than the current fork's head (${head_block_num})" );
            break;
         }
   }
   schedule_production_loop();
   return result;
}

block_production_condition::block_production_condition_enum producer_plugin_impl::maybe_produce_block(fc::mutable_variant_object& capture) {
   chain::controller& chain = app().get_plugin<chain_plugin>().chain();
   block_state_ptr hbs = chain.head_block_state();
   fc::time_point now = fc::time_point::now();

   /*
   if (app().get_plugin<chain_plugin>().is_skipping_transaction_signatures()) {
      _production_skip_flags |= skip_transaction_signatures;
   }
   */
   // If the next block production opportunity is in the present or future, we're synced.
   if( !_production_enabled )
   {
      if( hbs->header.timestamp.next().to_time_point() >= now )
         _production_enabled = true;
      else
         return block_production_condition::not_synced;
   }

   auto pending_block_timestamp = chain.pending_block_state()->header.timestamp;

   //
   // this assert should not fail, because now <= db.head_block_time()
   // should have resulted in slot == 0.
   //
   // if this assert triggers, there is a serious bug in get_slot_at_time()
   // which would result in allowing a later block to have a timestamp
   // less than or equal to the previous block
   //
   assert( now > chain.head_block_time() );

   const auto& scheduled_producer = hbs->get_scheduled_producer( pending_block_timestamp );
   // we must control the producer scheduled to produce the next block.
   if( _producers.find( scheduled_producer.producer_name ) == _producers.end() )
   {
      capture("scheduled_producer", scheduled_producer.producer_name);
      return block_production_condition::not_my_turn;
   }

   auto private_key_itr = _private_keys.find( scheduled_producer.block_signing_key );

   if( private_key_itr == _private_keys.end() )
   {
      capture("scheduled_key", scheduled_producer.block_signing_key);
      return block_production_condition::no_private_key;
   }

   /*uint32_t prate = hbs->producer_participation_rate();
   if( prate < _required_producer_participation )
   {
      capture("pct", uint32_t(prate / config::percent_1));
      return block_production_condition::low_participation;
   }*/

   if( llabs(( pending_block_timestamp.to_time_point() - now).count()) > fc::milliseconds( config::block_interval_ms ).count() )
   {
      capture("scheduled_time", pending_block_timestamp)("now", now);
      return block_production_condition::lag;
   }

   // determine if our watermark excludes us from producing at this point
   auto itr = _producer_watermarks.find(scheduled_producer.producer_name);
   if (itr != _producer_watermarks.end()) {
      if (itr->second >= hbs->block_num + 1) {
         capture("producer", scheduled_producer.producer_name)("watermark",itr->second)("head_block_num",hbs->block_num);
         return block_production_condition::fork_below_watermark;
      }
   }

   //idump( (fc::time_point::now() - chain.pending_block_time()) );
   chain.finalize_block();
   chain.sign_block( [&]( const digest_type& d ) { return private_key_itr->second.sign(d); } );
   chain.commit_block();
   auto hbt = chain.head_block_time();
   //idump((fc::time_point::now() - hbt));

   block_state_ptr new_bs = chain.head_block_state();
   // for newly installed producers we can set their watermarks to the block they became
   if (hbs->active_schedule.version != new_bs->active_schedule.version) {
      flat_set<account_name> new_producers;
      new_producers.reserve(new_bs->active_schedule.producers.size());
      for( const auto& p: new_bs->active_schedule.producers) {
         if (_producers.count(p.producer_name) > 0)
            new_producers.insert(p.producer_name);
      }

      for( const auto& p: hbs->active_schedule.producers) {
         new_producers.erase(p.producer_name);
      }

      for (const auto& new_producer: new_producers) {
         _producer_watermarks[new_producer] = chain.head_block_num();
      }
   }
   _producer_watermarks[scheduled_producer.producer_name] = chain.head_block_num();
   return block_production_condition::produced;
}

} // namespace eosio
