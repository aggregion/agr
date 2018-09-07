#include "boss.license_pool.hpp"
#include <agriolib/action.hpp>
#include <agriolib/transaction.hpp>
#include <initializer_list>

using namespace std;
using namespace agrio;
namespace boss {
license_pool::license_pool(account_name issuer)
  : tools::claimable(issuer),
  _licenses(_self, _self),
  _lic_settings(_self, _self),
  _rules(_self, _self),
  _settings(_self, _self),
  _records(_self, _self),
  _record_settings(_self, _self) {
  configs::get(_state, _self, N(state));
}

void license_pool::initialize(const string& name, const string& description) {
  require_auth(_self);

  if (configs::get(_state, _self, N(state))) {
    return;
  }
  _state.name        = name;
  _state.description = description;
  configs::store(_state, _self, N(state));

  // set owner role
  setting s;
  s.user = _self;
  s.key  = (uuids::properties::Role_HI << 64) | uuids::properties::Role_LO;

  auto idx  = _settings.template get_index<N(bysetting)>();
  auto s_it = idx.find(s.by_setting());

  if (s_it == idx.end()) {
    _settings.emplace(_self, [&](auto& s_obj) {
        s_obj.id    = _settings.available_primary_key();
        s_obj.user  = _self;
        s_obj.key   = s.key;
        s_obj.value = "Owner";
      });
  }
}

void license_pool::liccreate(uint128_t licenseUUID, const string& name, const string& description) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  // check if the same name exists
  auto idx = _licenses.template get_index<N(bylicuuid)>();
  auto it  = idx.find(licenseUUID);

  agrio_assert(it == idx.end(), "License UUID must be unique");

  // add new license
  _licenses.emplace(_self, [&](auto& lic) {
      lic.id          = _licenses.available_primary_key();
      lic.licenseUUID = licenseUUID;
      lic.name        = name;
      lic.description = description;
    });
}

void license_pool::licsetprop(uint128_t     licenseUUID,
                              uint128_t     key,
                              const string& value) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  auto lic_idx = _licenses.template get_index<N(bylicuuid)>();
  auto lic_it  = lic_idx.find(licenseUUID);

  agrio_assert(lic_it != lic_idx.end(), "License must exists");

  // find settings
  auto idx = _lic_settings.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(licenseUUID, key));

  if (it == idx.end()) {
    _lic_settings.emplace(ctx.user, [&](auto& rec) {
        rec.id          = _lic_settings.available_primary_key();
        rec.licenseUUID = licenseUUID;
        rec.key         = key;
        rec.value       = value;
      });
  } else {
    idx.modify(it, 0, [&](auto& rec) {
        rec.value = value;
      });
  }
}

void license_pool::typeadd(uint128_t     licenseUUID,
                           const string& licenseType) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  auto idx = _licenses.template get_index<N(bylicuuid)>();
  auto it  = idx.find(licenseUUID);

  agrio_assert(it != idx.end(), "License must exists");

  for (auto& t: it->types) {
    agrio_assert(t != licenseType, "Type already exists");
  }

  idx.modify(it, 0, [&](auto& lic) {
      lic.types.push_back(licenseType);
    });
}

void license_pool::typeremove(uint128_t licenseUUID, const string& licenseType) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  auto idx = _licenses.template get_index<N(bylicuuid)>();
  auto it  = idx.find(licenseUUID);

  agrio_assert(it != idx.end(), "License must exists");

  idx.modify(it, 0, [&](auto& lic) {
      for (auto t = lic.types.begin(), e = lic.types.end(); t != e; t++) {
        if (*t == licenseType) {
          lic.types.erase(t);
          break;
        }
      }
    });
}

void license_pool::ruleadd(const string& property,
                           uint16_t      condition,
                           const string& value,
                           const string& actions) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  // add new rule
  _rules.emplace(_self, [&](auto& rule) {
      rule.id        = _rules.available_primary_key();
      rule.property  = property;
      rule.condition = condition;
      rule.value     = value;
      rule.actions   = actions;
    });
}

void license_pool::rulesremove(const string& property,
                               uint16_t      condition,
                               const string& value) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  for (auto it = _rules.begin(); it != _rules.end();) {
    bool erased = false;

    if (property.empty() || (it->property == property)) {
      if ((condition == static_cast<uint16_t>(RuleCondition::NotValidCondition)) || (it->condition == condition)) {
        if (value.empty() || (it->value == value)) {
          it     = _rules.erase(it);
          erased = true;
        }
      }
    }

    if (!erased) {
      ++it;
    }
  }
}

void license_pool::usrsetprop(account_name  user,
                              uint128_t     key,
                              const string& value) {
  auto ctx = check_caller_role({ "Owner", "Manager" });
  setting s;

  s.user = user;
  s.key  = key;

  auto idx  = _settings.template get_index<N(bysetting)>();
  auto s_it = idx.find(s.by_setting());

  if (s_it != idx.end()) {
    if (!value.empty()) {
      idx.modify(s_it, 0, [&](auto& s_obj) {
          s_obj.value = value;
        });
    } else {
      idx.erase(s_it);
    }
  } else if (!value.empty()) {
    _settings.emplace(ctx.user, [&](auto& s_obj) {
        s_obj.id    = _settings.available_primary_key();
        s_obj.user  = user;
        s_obj.key   = key;
        s_obj.value = value;
      });
  }
}

void license_pool::recadd(uint128_t     licenseUUID,
                          uint128_t     recordUUID,
                          account_name  user,
                          const string& licenseType,
                          const string& /*extra*/) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  auto lic_idx = _licenses.template get_index<N(bylicuuid)>();
  auto lic_it  = lic_idx.find(licenseUUID);

  agrio_assert(lic_it != lic_idx.end(), "License must exists");

  // check type
  bool found = false;

  for (auto& t: lic_it->types) {
    if (t == licenseType) {
      found = true;
      break;
    }
  }
  agrio_assert(found, "License type must exists");

  auto rec_idx = _records.template get_index<N(byrecuuid)>();
  auto rec_it  = rec_idx.find(recordUUID);
  agrio_assert(rec_it == rec_idx.end(), "Record UUID must be unique");

  _records.emplace(ctx.user, [&](auto& rec) {
      rec.id          = _records.available_primary_key();
      rec.recordUUID  = recordUUID;
      rec.licenseUUID = licenseUUID;
      rec.user        = user;
      rec.creator     = ctx.user;
      rec.licenseType = licenseType;
    });
}

void license_pool::recsetprop(uint128_t     recordUUID,
                              uint128_t     key,
                              const string& value) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  auto rec_idx = _records.template get_index<N(byrecuuid)>();
  auto rec_it  = rec_idx.find(recordUUID);

  agrio_assert(rec_it != rec_idx.end(), "Record must exists");

  // find settings
  auto idx = _record_settings.template get_index<N(byrecuuid)>();
  auto it  = idx.find(make_uuids_key(recordUUID, key));

  if (it == idx.end()) {
    _record_settings.emplace(ctx.user, [&](auto& rec) {
        rec.id         = _record_settings.available_primary_key();
        rec.recordUUID = recordUUID;
        rec.key        = key;
        rec.value      = value;
      });
  } else {
    idx.modify(it, 0, [&](auto& rec) {
        rec.value = value;
      });
  }
}

void license_pool::recremove(uint128_t recordUUID) {
  auto ctx = check_caller_role({ "Owner", "Manager" });

  // remove record
  auto rec_idx = _records.template get_index<N(byrecuuid)>();
  auto rec_it  = rec_idx.find(recordUUID);

  agrio_assert(rec_it != rec_idx.end(), "Record must exists");

  rec_idx.erase(rec_it);

  // remove properties
  auto idx = _record_settings.template get_index<N(byrecuuid)>();
  auto it  = idx.lower_bound(make_uuids_key(recordUUID, 0LL));

  while (it != idx.end() && it->recordUUID == recordUUID) {
    it = idx.erase(it);
  }
}

bool license_pool::check_role(account_name user, const string& role) {
  setting s;

  s.user = user;
  s.key  = (uuids::properties::Role_HI << 64) | uuids::properties::Role_LO;

  auto idx = _settings.template get_index<N(bysetting)>();
  auto it  = idx.find(s.by_setting());

  return it != idx.end() && it->value == role;
}

license_pool::user_context license_pool::check_caller_role(initializer_list<string>allowRolesList) {
  agrio::action act = agrio::get_action(1, 0);
  user_context  ctx;

  for (auto& auth : act.authorization) {
    if (auth.permission != N(active)) continue;

    for (auto& role : allowRolesList) {
      if (check_role(auth.actor, role)) {
        ctx.user = auth.actor;
        ctx.role = role;
        break;
      }
    }
  }
  agrio_assert(ctx.user > 0, "No valid permissions for authorized users with action permission!");

  require_auth(ctx.user);
  return ctx;
}
}
AGRIO_ABI(boss::license_pool, (initialize)(liccreate)(licsetprop)(typeadd)(typeremove)(ruleadd)(rulesremove)(usrsetprop)(recadd)(recsetprop)(recremove)(setowner)(claim))
