/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enumivo/enumivo.hpp>

class [[enumivo::contract]] test_api_db : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action("pg")]]
   void primary_i64_general();

   [[enumivo::action("pl")]]
   void primary_i64_lowerbound();

   [[enumivo::action("pu")]]
   void primary_i64_upperbound();

   [[enumivo::action("s1g")]]
   void idx64_general();

   [[enumivo::action("s1l")]]
   void idx64_lowerbound();

   [[enumivo::action("s1u")]]
   void idx64_upperbound();

   [[enumivo::action("tia")]]
   void test_invalid_access( enumivo::name code, uint64_t val, uint32_t index, bool store );

   [[enumivo::action("sdnancreate")]]
   void idx_double_nan_create_fail();

   [[enumivo::action("sdnanmodify")]]
   void idx_double_nan_modify_fail();

   [[enumivo::action("sdnanlookup")]]
   void idx_double_nan_lookup_fail( uint32_t lookup_type );

   [[enumivo::action("sk32align")]]
   void misaligned_secondary_key256_tests();

};
