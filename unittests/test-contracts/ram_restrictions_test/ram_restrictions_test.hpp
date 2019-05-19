/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] ram_restrictions_test : public agrio::contract {
public:
   struct [[agrio::table]] data {
      uint64_t           key;
      std::vector<char>  value;

      uint64_t primary_key() const { return key; }
   };

   typedef agrio::multi_index<"tablea"_n, data> tablea;
   typedef agrio::multi_index<"tableb"_n, data> tableb;

public:
   using agrio::contract::contract;

   [[agrio::action]]
   void noop();

   [[agrio::action]]
   void setdata( uint32_t len1, uint32_t len2, agrio::name payer );

   [[agrio::action]]
   void notifysetdat( agrio::name acctonotify, uint32_t len1, uint32_t len2, agrio::name payer );

   [[agrio::on_notify("tester2::notifysetdat")]]
   void on_notify_setdata( agrio::name acctonotify, uint32_t len1, uint32_t len2, agrio::name payer );

   [[agrio::action]]
   void senddefer( uint64_t senderid, agrio::name payer );

   [[agrio::action]]
   void notifydefer( agrio::name acctonotify, uint64_t senderid, agrio::name payer );

   [[agrio::on_notify("tester2::notifydefer")]]
   void on_notifydefer( agrio::name acctonotify, uint64_t senderid, agrio::name payer );

};
