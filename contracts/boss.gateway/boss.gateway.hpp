/**
 *  @file
 *  @copyright defined in boss/LICENSE.txt
 */
#pragma once
#include <agriolib/asset.hpp>
#include <agriolib/agrio.hpp>

#include "../tools/claimable.hpp"

namespace boss {
class gateway : public tools::claimable {
public:

  gateway(account_name issuer);

  void create(agrio::symbol_name symbol);
  void purchase(account_name from,
                account_name beneficiary,
                uint64_t     amount);

  void transfer(account_name from,
                account_name to,
                uint64_t     quantity);

  void withdraw(account_name to,
                uint64_t     amount);

  void setrate(uint64_t rate);

  // @abi table state i64
  struct gateway_state {
    uint64_t           exchange_rate = 0;
    agrio::symbol_name symbol        = 0;
    uint64_t           supply        = 0;

    gateway_state() {}

    constexpr static uint64_t key = N(gateway_state);
    AGRLIB_SERIALIZE(gateway_state, (exchange_rate)(symbol)(supply))
  };

  // @abi table accounts i64
  struct account {
    account_name owner;
    uint64_t     balance;

    uint64_t primary_key() const {
      return owner;
    }

    AGRLIB_SERIALIZE(account, (owner)(balance))
  };

  typedef agrio::multi_index<N(accounts), account> accounts;

private:

  gateway_state _state;

  void    sub_balance(account_name owner,
                      uint64_t     value);
  void    add_balance(account_name owner,
                      uint64_t     value,
                      account_name ram_payer);
  int64_t coins_amount(uint64_t tokens) const;
};
} /// namespace boss
