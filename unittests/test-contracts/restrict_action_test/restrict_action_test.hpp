/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] restrict_action_test : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void noop( );

   [[agrio::action]]
   void sendinline( agrio::name authorizer );

   [[agrio::action]]
   void senddefer( agrio::name authorizer, uint32_t senderid );


   [[agrio::action]]
   void notifyinline( agrio::name acctonotify, agrio::name authorizer );

   [[agrio::action]]
   void notifydefer( agrio::name acctonotify, agrio::name authorizer, uint32_t senderid );

   [[agrio::on_notify("testacc::notifyinline")]]
   void on_notify_inline( agrio::name acctonotify, agrio::name authorizer );

   [[agrio::on_notify("testacc::notifydefer")]]
   void on_notify_defer( agrio::name acctonotify, agrio::name authorizer, uint32_t senderid );
};
