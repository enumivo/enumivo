/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enumivo/enu.hpp>
#include <enumivo/singleton.hpp>
#include <enumivo/asset.hpp>

// Extacted from enu.token contract:
namespace enumivo {
   class [[enumivo::contract("enu.token")]] token : public enumivo::contract {
   public:
      using enumivo::contract::contract;

      [[enumivo::action]]
      void transfer( enumivo::name        from,
                     enumivo::name        to,
                     enumivo::asset       quantity,
                     const std::string& memo );
      using transfer_action = enumivo::action_wrapper<"transfer"_n, &token::transfer>;
   };
}

// This contract:
class [[enumivo::contract]] proxy : public enumivo::contract {
public:
   proxy( enumivo::name self, enumivo::name first_receiver, enumivo::datastream<const char*> ds );

   [[enumivo::action]]
   void setowner( enumivo::name owner, uint32_t delay );

   [[enumivo::on_notify("enu.token::transfer")]]
   void on_transfer( enumivo::name        from,
                     enumivo::name        to,
                     enumivo::asset       quantity,
                     const std::string& memo );

   [[enumivo::on_notify("enumivo::onerror")]]
   void on_error( uint128_t sender_id, enumivo::ignore<std::vector<char>> sent_trx );

   struct [[enumivo::table]] config {
      enumivo::name owner;
      uint32_t    delay   = 0;
      uint32_t    next_id = 0;

      ENULIB_SERIALIZE( config, (owner)(delay)(next_id) )
   };

   using config_singleton = enumivo::singleton< "config"_n,  config >;

protected:
   config_singleton _config;
};
