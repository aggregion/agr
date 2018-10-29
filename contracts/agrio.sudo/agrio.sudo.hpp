#pragma once

#include <agriolib/agrio.hpp>

namespace agrio {

   class sudo : public contract {
      public:
         sudo( account_name self ):contract(self){}

         void exec();

   };

} /// namespace agrio
