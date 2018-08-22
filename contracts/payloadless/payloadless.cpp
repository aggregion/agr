#include <agriolib/agrio.hpp>
#include <agriolib/print.hpp>
using namespace agrio;

class payloadless : public agrio::contract {
  public:
      using contract::contract;

      void doit() {
         print( "Im a payloadless action" );
      }
};

AGRIO_ABI( payloadless, (doit) )
