#pragma once
#include <agriolib/agrio.hpp>
#include <agriolib/contract.hpp>
#include "../tools/configs.hpp"
/**
 * @title claimable
 * @dev Extension for the contract, where the ownership could to be claimed.
 * This allows the new owner to accept the transfer.
 */
namespace tools {
class claimable : public agrio::contract {
public:
  // @abi table ownership i64
  struct ownership {
    account_name pending_owner;
    ownership(account_name o) { pending_owner = o; }
    ownership() { pending_owner = 0; }

    constexpr static uint64_t key = N(ownership);
  };

  claimable(account_name self) : agrio::contract(self) {}

  /**
   * @dev Allows the current owner to set the pendingOwner address.
   * @param new_owner The address to transfer ownership to.
   */
  inline void setowner(account_name new_owner) {
    require_auth(_self);
    require_recipient(new_owner);

    ownership own = { new_owner };
    configs::store(own, _self, N(ownership));
  }

  /**
   * @dev Allows the pendingOwner address to finalize the transfer.
   */
  inline void claim() {
    ownership own;
    agrio_assert(configs::get(own, _self, N(ownership)), "New owner must be set");

    require_recipient(own.pending_owner);
    require_auth(own.pending_owner);
    _self          = own.pending_owner;
    
    own.pending_owner = 0;
    configs::store(own, _self, N(ownership));
  }
};
}

