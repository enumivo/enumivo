/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enumivo/enu.hpp>
#include <vector>

class [[enumivo::contract]] deferred_test : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action]]
   void defercall( enumivo::name payer, uint64_t sender_id, enumivo::name contract, uint64_t payload );

   [[enumivo::action]]
   void deferfunc( uint64_t payload );
   using deferfunc_action = enumivo::action_wrapper<"deferfunc"_n, &deferred_test::deferfunc>;

   [[enumivo::action]]
   void inlinecall( enumivo::name contract, enumivo::name authorizer, uint64_t payload );

   [[enumivo::on_notify("enumivo::onerror")]]
   void on_error( uint128_t sender_id, enumivo::ignore<std::vector<char>> sent_trx );
};
