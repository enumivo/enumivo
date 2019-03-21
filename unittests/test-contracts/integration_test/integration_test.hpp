/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enulib/enu.hpp>

class [[enumivo::contract]] integration_test : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action]]
   void store( enumivo::name from, enumivo::name to, uint64_t num );

   struct [[enumivo::table("payloads")]] payload {
      uint64_t              key;
      std::vector<uint64_t> data;

      uint64_t primary_key()const { return key; }

      ENULIB_SERIALIZE( payload, (key)(data) )
   };

   using payloads_table = enumivo::multi_index< "payloads"_n,  payload >;

};
