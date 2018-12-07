/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */

#include <agriolib/agrio.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      AGRLIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
