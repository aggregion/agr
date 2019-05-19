/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>
#include <agrio/singleton.hpp>
#include <agrio/asset.hpp>

// Extacted from agrio.token contract:
namespace agrio {
   class [[agrio::contract("agrio.token")]] token : public agrio::contract {
   public:
      using agrio::contract::contract;

      [[agrio::action]]
      void transfer( agrio::name        from,
                     agrio::name        to,
                     agrio::asset       quantity,
                     const std::string& memo );
      using transfer_action = agrio::action_wrapper<"transfer"_n, &token::transfer>;
   };
}

// This contract:
class [[agrio::contract]] proxy : public agrio::contract {
public:
   proxy( agrio::name self, agrio::name first_receiver, agrio::datastream<const char*> ds );

   [[agrio::action]]
   void setowner( agrio::name owner, uint32_t delay );

   [[agrio::on_notify("agrio.token::transfer")]]
   void on_transfer( agrio::name        from,
                     agrio::name        to,
                     agrio::asset       quantity,
                     const std::string& memo );

   [[agrio::on_notify("agrio::onerror")]]
   void on_error( uint128_t sender_id, agrio::ignore<std::vector<char>> sent_trx );

   struct [[agrio::table]] config {
      agrio::name owner;
      uint32_t    delay   = 0;
      uint32_t    next_id = 0;

      AGRLIB_SERIALIZE( config, (owner)(delay)(next_id) )
   };

   using config_singleton = agrio::singleton< "config"_n,  config >;

protected:
   config_singleton _config;
};
