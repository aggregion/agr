/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */
#pragma once

#include <agrio/chain/types.hpp>
#include <agrio/chain/contract_types.hpp>

namespace agrio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_agrio_newaccount(apply_context&);
   void apply_agrio_updateauth(apply_context&);
   void apply_agrio_deleteauth(apply_context&);
   void apply_agrio_linkauth(apply_context&);
   void apply_agrio_unlinkauth(apply_context&);

   /*
   void apply_agrio_postrecovery(apply_context&);
   void apply_agrio_passrecovery(apply_context&);
   void apply_agrio_vetorecovery(apply_context&);
   */

   void apply_agrio_setcode(apply_context&);
   void apply_agrio_setabi(apply_context&);

   void apply_agrio_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace agrio::chain
