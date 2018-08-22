/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */

#include <agriolib/agrio.hpp>

namespace agrio {

   class noop: public contract {
      public:
         noop( account_name self ): contract( self ) { }
         void anyaction( account_name from,
                         const std::string& /*type*/,
                         const std::string& /*data*/ )
         {
            require_auth( from );
         }
   };

   AGRIO_ABI( noop, ( anyaction ) )

} /// agrio     
