/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <enumivo/chain/types.hpp>
#include <enumivo/chain/authority.hpp>
#include <enumivo/chain/block_timestamp.hpp>
#include <enumivo/chain/contracts/types.hpp>

#include "multi_index_includes.hpp"

namespace enumivo { namespace chain {

   class account_object : public chainbase::object<account_object_type, account_object> {
      OBJECT_CTOR(account_object,(code)(abi))

      id_type              id;
      account_name         name;
      uint8_t              vm_type      = 0;
      uint8_t              vm_version   = 0;
      bool                 privileged   = false;

      time_point_sec       last_code_update;
      digest_type          code_version;
      block_timestamp_type creation_date;

      shared_vector<char>  code;
      shared_vector<char>  abi;

      void set_abi( const enumivo::chain::contracts::abi_def& a ) {
         // Added resize(0) here to avoid bug in boost vector container
         abi.resize( 0 );
         abi.resize( fc::raw::pack_size( a ) );
         fc::datastream<char*> ds( abi.data(), abi.size() );
         fc::raw::pack( ds, a );
      }

      enumivo::chain::contracts::abi_def get_abi()const {
         enumivo::chain::contracts::abi_def a;
         fc::datastream<const char*> ds( abi.data(), abi.size() );
         fc::raw::unpack( ds, a );
         return a;
      }
   };
   using account_id_type = account_object::id_type;

   struct by_name;
   using account_index = chainbase::shared_multi_index_container<
      account_object,
      indexed_by<
         ordered_unique<tag<by_id>, member<account_object, account_object::id_type, &account_object::id>>,
         ordered_unique<tag<by_name>, member<account_object, account_name, &account_object::name>>
      >
   >;

} } // enumivo::chain

CHAINBASE_SET_INDEX_TYPE(enumivo::chain::account_object, enumivo::chain::account_index)


FC_REFLECT(enumivo::chain::account_object, (name)(vm_type)(vm_version)(code_version)(code)(creation_date))
