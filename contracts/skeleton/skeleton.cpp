#include <agriolib/agrio.hpp>

using namespace agrio;

class hello : public agrio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }
};

AGRIO_ABI( hello, (hi) )
