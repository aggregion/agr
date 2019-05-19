/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#include "noop.hpp"

using namespace agrio;

void noop::anyaction( name                       from,
                      const ignore<std::string>& type,
                      const ignore<std::string>& data )
{
   require_auth( from );
}
