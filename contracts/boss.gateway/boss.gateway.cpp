#include <cmath>

#include "boss.gateway.hpp"
#include "../tools/configs.hpp"

using namespace agrio;

namespace boss {
gateway::gateway(account_name issuer)
  : tools::claimable(issuer) {
  configs::get(_state, _self, N(state));
}

void gateway::create(agrio::symbol_name symbol) {
  require_auth(_self);

  if (configs::get(_state, _self, N(state))) {
    return;
  }
  _state.symbol        = symbol;
  _state.exchange_rate = 0;
  _state.supply        = 0;
  configs::store(_state, _self, N(state));
}

void gateway::purchase(account_name from,
                       account_name beneficiary,
                       uint64_t     amount) {
  require_auth(from);
  agrio_assert(is_account(beneficiary),              "to account does not exist");
  agrio_assert(amount > 0,                           "must purchase positive quantity");
  agrio_assert(_state.exchange_rate > 0,             "gateway must be active now!");
  agrio_assert(amount <= ULLONG_MAX - _state.supply, "quantity exceeds available supply");

  // calculate token amount to be created
  int64_t coins = coins_amount(amount);

  // send CORE coins
  action(
    permission_level{ from, N(active) },
    N(agrio.token), N(transfer),
    std::make_tuple(from, _self, asset(coins), std::string(""))
    ).send();

  // update supply count
  _state.supply += amount;
  configs::store(_state, _self, N(state));

  add_balance(beneficiary, amount, from);
}

void gateway::transfer(account_name from,
                       account_name to,
                       uint64_t     quantity)
{
  agrio_assert(from != to, "cannot transfer to self");
  require_auth(from);
  agrio_assert(is_account(to), "to account does not exist");

  require_recipient(from);
  require_recipient(to);

  sub_balance(from, quantity);
  add_balance(to, quantity, from);
}

void gateway::withdraw(account_name to, uint64_t amount) {
  require_auth(to);
  agrio_assert(amount > 0,               "must withdraw positive quantity");
  agrio_assert(_state.exchange_rate > 0, "gateway must be active now!");
  agrio_assert(_state.supply >= amount,  "Amount must be valid");

  int64_t coins = coins_amount(amount);

  sub_balance(to, amount);

  // update supply count
  _state.supply -= amount;
  configs::store(_state, _self, N(state));

  // send CORE coins
  action(
    permission_level{ _self, N(active) },
    N(agrio.token), N(transfer),
    std::make_tuple(_self, to, asset(coins), std::string(""))
    ).send();
}

void gateway::setrate(uint64_t rate) {
  require_auth(_self);
  _state.exchange_rate = rate;
  configs::store(_state, _self, N(state));
}

int64_t gateway::coins_amount(uint64_t tokens) const {
  asset as(0, CORE_SYMBOL);
  auto  res = tokens / _state.exchange_rate;

  res *= pow(10, as.symbol.precision());
  return res;
}

void gateway::sub_balance(account_name owner, uint64_t value) {
  accounts from_acnts(_self, owner);
  const auto& from = from_acnts.get(owner);

  agrio_assert(from.balance >= value, "overdrawn balance");

  if (from.balance == value) {
    from_acnts.erase(from);
  } else {
    from_acnts.modify(from, owner, [&](auto& a) {
        a.balance -= value;
      });
  }
}

void gateway::add_balance(account_name owner, uint64_t value, account_name  ram_payer) {
  accounts to_acnts(_self, owner);
  auto     to = to_acnts.find(owner);

  if (to == to_acnts.end()) {
    to_acnts.emplace(ram_payer, [&](auto& a) {
        a.owner   = owner;
        a.balance = value;
      });
  } else {
    to_acnts.modify(to, 0, [&](auto& a) {
        a.balance += value;
      });
  }
}
}

using namespace boss;
using namespace agrio;

AGRIO_ABI(boss::gateway, (create)(purchase)(transfer)(withdraw)(setrate)(setowner)(claim))
