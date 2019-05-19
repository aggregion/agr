/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] payloadless : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void doit();
};
