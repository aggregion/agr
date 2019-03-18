/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] integration_test : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void store( agrio::name from, agrio::name to, uint64_t num );

   struct [[agrio::table("payloads")]] payload {
      uint64_t              key;
      std::vector<uint64_t> data;

      uint64_t primary_key()const { return key; }

      AGRLIB_SERIALIZE( payload, (key)(data) )
   };

   using payloads_table = agrio::multi_index< "payloads"_n,  payload >;

};
