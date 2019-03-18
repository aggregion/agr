/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] test_api_multi_index : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action("s1g")]]
   void idx64_general();

   [[agrio::action("s1store")]]
   void idx64_store_only();

   [[agrio::action("s1check")]]
   void idx64_check_without_storing();

   [[agrio::action("s1findfail1")]]
   void idx64_require_find_fail();

   [[agrio::action("s1findfail2")]]
   void idx64_require_find_fail_with_msg();

   [[agrio::action("s1findfail3")]]
   void idx64_require_find_sk_fail();

   [[agrio::action("s1findfail4")]]
   void idx64_require_find_sk_fail_with_msg();

   [[agrio::action("s1pkend")]]
   void idx64_pk_iterator_exceed_end();

   [[agrio::action("s1skend")]]
   void idx64_sk_iterator_exceed_end();

   [[agrio::action("s1pkbegin")]]
   void idx64_pk_iterator_exceed_begin();

   [[agrio::action("s1skbegin")]]
   void idx64_sk_iterator_exceed_begin();

   [[agrio::action("s1pkref")]]
   void idx64_pass_pk_ref_to_other_table();

   [[agrio::action("s1skref")]]
   void idx64_pass_sk_ref_to_other_table();

   [[agrio::action("s1pkitrto")]]
   void idx64_pass_pk_end_itr_to_iterator_to();

   [[agrio::action("s1pkmodify")]]
   void idx64_pass_pk_end_itr_to_modify();

   [[agrio::action("s1pkerase")]]
   void idx64_pass_pk_end_itr_to_erase();

   [[agrio::action("s1skitrto")]]
   void idx64_pass_sk_end_itr_to_iterator_to();

   [[agrio::action("s1skmodify")]]
   void idx64_pass_sk_end_itr_to_modify();

   [[agrio::action("s1skerase")]]
   void idx64_pass_sk_end_itr_to_erase();

   [[agrio::action("s1modpk")]]
   void idx64_modify_primary_key();

   [[agrio::action("s1exhaustpk")]]
   void idx64_run_out_of_avl_pk();

   [[agrio::action("s1skcache")]]
   void idx64_sk_cache_pk_lookup();

   [[agrio::action("s1pkcache")]]
   void idx64_pk_cache_sk_lookup();

   [[agrio::action("s2g")]]
   void idx128_general();

   [[agrio::action("s2store")]]
   void idx128_store_only();

   [[agrio::action("s2check")]]
   void idx128_check_without_storing();

   [[agrio::action("s2autoinc")]]
   void idx128_autoincrement_test();

   [[agrio::action("s2autoinc1")]]
   void idx128_autoincrement_test_part1();

   [[agrio::action("s2autoinc2")]]
   void idx128_autoincrement_test_part2();

   [[agrio::action("s3g")]]
   void idx256_general();

   [[agrio::action("sdg")]]
   void idx_double_general();

   [[agrio::action("sldg")]]
   void idx_long_double_general();

};
