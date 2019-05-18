/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#pragma once

#include <agrio/agrio.hpp>

namespace agrio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((agrio_wasm_import))
         uint64_t get_sender();
      }
   }
}

namespace agrio {
   name get_sender() {
      return name( internal_use_do_not_use::get_sender() );
   }
}

class [[agrio::contract]] get_sender_test : public agrio::contract {
public:
   using agrio::contract::contract;

   [[agrio::action]]
   void assertsender( agrio::name expected_sender );
   using assertsender_action = agrio::action_wrapper<"assertsender"_n, &get_sender_test::assertsender>;

   [[agrio::action]]
   void sendinline( agrio::name to, agrio::name expected_sender );

   [[agrio::action]]
   void notify( agrio::name to, agrio::name expected_sender, bool send_inline );

   // agrio.cdt 1.6.1 has a problem with "*::notify" so hardcode to tester1 for now.
   // TODO: Change it back to "*::notify" when the bug is fixed in agrio.cdt.
   [[agrio::on_notify("tester1::notify")]]
   void on_notify( agrio::name to, agrio::name expected_sender, bool send_inline );

};
