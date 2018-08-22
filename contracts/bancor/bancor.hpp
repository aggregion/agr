/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */
#pragma once

#include <agriolib/agrio.hpp>
#include <agriolib/token.hpp>
#include <agriolib/reflect.hpp>
#include <agriolib/generic_currency.hpp>

#include <bancor/converter.hpp>
#include <currency/currency.hpp>

namespace bancor {
   typedef agrio::generic_currency< agrio::token<N(other),S(4,OTHER)> >  other_currency;
   typedef agrio::generic_currency< agrio::token<N(bancor),S(4,RELAY)> > relay_currency;
   typedef agrio::generic_currency< agrio::token<N(currency),S(4,CUR)> > cur_currency;

   typedef converter<relay_currency, other_currency, cur_currency > example_converter;
} /// bancor

