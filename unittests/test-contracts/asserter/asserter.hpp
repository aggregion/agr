/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] asserter : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void procassert( int8_t condition, std::string message );

   [[agrio::action]]
   void provereset();
};
