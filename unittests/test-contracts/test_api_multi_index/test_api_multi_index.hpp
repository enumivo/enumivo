/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include <enumivo/enu.hpp>

class [[enumivo::contract]] test_api_multi_index : public enumivo::contract {
public:
   using enumivo::contract::contract;

   [[enumivo::action("s1g")]]
   void idx64_general();

   [[enumivo::action("s1store")]]
   void idx64_store_only();

   [[enumivo::action("s1check")]]
   void idx64_check_without_storing();

   [[enumivo::action("s1findfail1")]]
   void idx64_require_find_fail();

   [[enumivo::action("s1findfail2")]]
   void idx64_require_find_fail_with_msg();

   [[enumivo::action("s1findfail3")]]
   void idx64_require_find_sk_fail();

   [[enumivo::action("s1findfail4")]]
   void idx64_require_find_sk_fail_with_msg();

   [[enumivo::action("s1pkend")]]
   void idx64_pk_iterator_exceed_end();

   [[enumivo::action("s1skend")]]
   void idx64_sk_iterator_exceed_end();

   [[enumivo::action("s1pkbegin")]]
   void idx64_pk_iterator_exceed_begin();

   [[enumivo::action("s1skbegin")]]
   void idx64_sk_iterator_exceed_begin();

   [[enumivo::action("s1pkref")]]
   void idx64_pass_pk_ref_to_other_table();

   [[enumivo::action("s1skref")]]
   void idx64_pass_sk_ref_to_other_table();

   [[enumivo::action("s1pkitrto")]]
   void idx64_pass_pk_end_itr_to_iterator_to();

   [[enumivo::action("s1pkmodify")]]
   void idx64_pass_pk_end_itr_to_modify();

   [[enumivo::action("s1pkerase")]]
   void idx64_pass_pk_end_itr_to_erase();

   [[enumivo::action("s1skitrto")]]
   void idx64_pass_sk_end_itr_to_iterator_to();

   [[enumivo::action("s1skmodify")]]
   void idx64_pass_sk_end_itr_to_modify();

   [[enumivo::action("s1skerase")]]
   void idx64_pass_sk_end_itr_to_erase();

   [[enumivo::action("s1modpk")]]
   void idx64_modify_primary_key();

   [[enumivo::action("s1exhaustpk")]]
   void idx64_run_out_of_avl_pk();

   [[enumivo::action("s1skcache")]]
   void idx64_sk_cache_pk_lookup();

   [[enumivo::action("s1pkcache")]]
   void idx64_pk_cache_sk_lookup();

   [[enumivo::action("s2g")]]
   void idx128_general();

   [[enumivo::action("s2store")]]
   void idx128_store_only();

   [[enumivo::action("s2check")]]
   void idx128_check_without_storing();

   [[enumivo::action("s2autoinc")]]
   void idx128_autoincrement_test();

   [[enumivo::action("s2autoinc1")]]
   void idx128_autoincrement_test_part1();

   [[enumivo::action("s2autoinc2")]]
   void idx128_autoincrement_test_part2();

   [[enumivo::action("s3g")]]
   void idx256_general();

   [[enumivo::action("sdg")]]
   void idx_double_general();

   [[enumivo::action("sldg")]]
   void idx_long_double_general();

};
