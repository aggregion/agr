/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] test_api_db : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action("pg")]]
   void primary_i64_general();

   [[agrio::action("pl")]]
   void primary_i64_lowerbound();

   [[agrio::action("pu")]]
   void primary_i64_upperbound();

   [[agrio::action("s1g")]]
   void idx64_general();

   [[agrio::action("s1l")]]
   void idx64_lowerbound();

   [[agrio::action("s1u")]]
   void idx64_upperbound();

   [[agrio::action("tia")]]
   void test_invalid_access( agrio::name code, uint64_t val, uint32_t index, bool store );

   [[agrio::action("sdnancreate")]]
   void idx_double_nan_create_fail();

   [[agrio::action("sdnanmodify")]]
   void idx_double_nan_modify_fail();

   [[agrio::action("sdnanlookup")]]
   void idx_double_nan_lookup_fail( uint32_t lookup_type );

   [[agrio::action("sk32align")]]
   void misaligned_secondary_key256_tests();

};
