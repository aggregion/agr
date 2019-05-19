/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

class [[agrio::contract]] noop : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void anyaction( agrio::name                       from,
                   const agrio::ignore<std::string>& type,
                   const agrio::ignore<std::string>& data );
};
