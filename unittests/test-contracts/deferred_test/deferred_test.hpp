/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>
#include <vector>

class [[agrio::contract]] deferred_test : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void defercall( agrio::name payer, uint64_t sender_id, agrio::name contract, uint64_t payload );

   [[agrio::action]]
   void delayedcall( agrio::name payer, uint64_t sender_id, agrio::name contract,
                     uint64_t payload, uint32_t delay_sec, bool replace_existing );

   [[agrio::action]]
   void deferfunc( uint64_t payload );
   using deferfunc_action = agrio::action_wrapper<"deferfunc"_n, &deferred_test::deferfunc>;

   [[agrio::action]]
   void inlinecall( agrio::name contract, agrio::name authorizer, uint64_t payload );

   [[agrio::action]]
   void fail();

   [[agrio::on_notify("agrio::onerror")]]
   void on_error( uint128_t sender_id, agrio::ignore<std::vector<char>> sent_trx );
};
