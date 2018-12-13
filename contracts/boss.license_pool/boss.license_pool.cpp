#include "../tools/configs.hpp"
#include "boss.license_pool.hpp"

using namespace std;
using namespace agrio;
namespace boss {

license_pool::license_pool(account_name issuer)
  : contract(issuer),
  _offers(_self, _self),
  _offer_properties(_self, _self),
  _licenses(_self, _self),
  _license_properties(_self, _self),
  _distrubution_delegations(_self, _self),
  _distr_deleg_rules(_self, _self),
  _distr_deleg_limits(_self, _self) {
  configs::get(_info, _self, N(info));
}

void license_pool::initialize(const string& name, const string& description) {
  require_auth(_self);
  agrio_assert(!configs::get(_info, _self, N(info)), "Contract has been already initialized!");

  _info.name        = name;
  _info.description = description;
  configs::store(_info, _self, N(info));
}

void license_pool::offerset(uint128_t     offerUUID,
                            account_name  owner,
                            const string& name,
                            const string& description) {
  require_auth(_self);

  agrio_assert(is_account(owner),    "Owner account does not exist!");
  agrio_assert(!name.empty(),        "Offer name is empty!");
  agrio_assert(!description.empty(), "Offer description is empty!");

  auto idx = _offers.template get_index<N(byofferuuid)>();
  auto it  = idx.find(offerUUID);

  if (it == idx.end()) {
    _offers.emplace(_self, [&](auto& rec) {
        rec.id          = _offers.available_primary_key();
        rec.offerUUID   = offerUUID;
        rec.owner       = owner;
        rec.name        = name;
        rec.description = description;
      });
  } else {
    idx.modify(it, 0, [&](auto& rec) {
        rec.owner       = owner;
        rec.name        = name.empty();
        rec.description = description;
      });
  }
}

void license_pool::offerpropset(uint128_t     offerUUID,
                                uint128_t     name,
                                const string& value) {
  require_auth(_self);

  auto idxOffer = _offers.template get_index<N(byofferuuid)>();
  auto itOffer  = idxOffer.find(offerUUID);
  agrio_assert(itOffer != idxOffer.end(), "Offer doesn't exist!");

  auto idx = _offer_properties.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(offerUUID, name));

  if (it == idx.end()) {
    _offer_properties.emplace(_self, [&](auto& rec) {
        rec.id        = _offer_properties.available_primary_key();
        rec.offerUUID = offerUUID;
        rec.key       = name;
        rec.value     = value;
      });
  } else {
    idx.modify(it, 0, [&](auto& rec) {
        rec.value = value;
      });
  }
}

void license_pool::offerproprem(uint128_t offerUUID,
                                uint128_t name) {
  require_auth(_self);

  auto idx = _offer_properties.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(offerUUID, name));

  agrio_assert(it != idx.end(), "Offer property doesn't exist!");
  idx.erase(it);
}

void license_pool::distrcreate(uint128_t    distributionUUID,
                               uint128_t    parentUUID,
                               uint128_t    offerUUID,
                               account_name delegate_to) {
  account_name issuer;

  agrio_assert(distributionUUID != uuids::properties::NotValidUUID, "UUID is not valid!");

  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();

  if (parentUUID == uuids::properties::NotValidUUID) {
    require_auth(_self);
    issuer = _self;
  } else {
    auto itDistr = idxDistr.find(parentUUID);
    agrio_assert(itDistr != idxDistr.end(), "Parent distribution delegation doesn't exist!");

    require_auth(itDistr->delegate_to);
    issuer = itDistr->delegate_to;
  }

  auto itDistr = idxDistr.find(distributionUUID);
  agrio_assert(itDistr == idxDistr.end(), "Distribution delegation already exist!");

  _distrubution_delegations.emplace(issuer, [&](auto& rec) {
      rec.id               = _distrubution_delegations.available_primary_key();
      rec.distributionUUID = distributionUUID;
      rec.parentUUID       = parentUUID;
      rec.offerUUID        = offerUUID;
      rec.delegate_from    = issuer;
      rec.delegate_to      = delegate_to;
    });
}

void license_pool::distrruleset(uint128_t     distributionUUID,
                                uint8_t       type,
                                uint8_t       condition,
                                uint128_t     name,
                                const string& value) {
  agrio_assert(distributionUUID != uuids::properties::NotValidUUID,              "UUID is not valid!");
  agrio_assert(name != uuids::properties::NotValidUUID,                          "UUID is not valid!");
  agrio_assert(type < static_cast<int>(RuleType::TotalTypes),                    "Invalid type!");
  agrio_assert(condition < static_cast<uint8_t>(RuleCondition::TotalConditions), "Invalid condition!");

  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto itDistr  = idxDistr.find(distributionUUID);
  agrio_assert(itDistr != idxDistr.end(), "Distribution delegation doesn't exist!");

  require_auth(itDistr->delegate_from);

  auto idx      = _distr_deleg_rules.template get_index<N(bydistruuid)>();
  bool modified = false;

  for (auto it  = idx.find(distributionUUID), itEnd = idx.end(); it != itEnd && it->distributionUUID == distributionUUID; it++) {
    if ((it->name == name) && (it->type == type) && (it->condition == condition)) {
      idx.modify(it, 0, [&](auto& rec) {
          rec.value = value;
        });
      modified = true;
      break;
    }
  }

  if (!modified) {
    _distr_deleg_rules.emplace(itDistr->delegate_from, [&](auto& rec) {
        rec.id               = _distr_deleg_rules.available_primary_key();
        rec.distributionUUID = distributionUUID;
        rec.type             = type;
        rec.condition        = condition;
        rec.name             = name;
        rec.value            = value;
      });
  }
}

void license_pool::distrrulerem(uint128_t distributionUUID,
                                uint8_t   type,
                                uint8_t   condition,
                                uint128_t name) {
  agrio_assert(distributionUUID != uuids::properties::NotValidUUID, "UUID is not valid!");

  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto itDistr  = idxDistr.find(distributionUUID);
  agrio_assert(itDistr != idxDistr.end(), "Distribution delegation doesn't exist!");

  require_auth(itDistr->delegate_from);

  auto idx = _distr_deleg_rules.template get_index<N(bydistruuid)>();

  for (auto it  = idx.find(distributionUUID), itEnd = idx.end(); it != itEnd && it->distributionUUID == distributionUUID;) {
    bool erased = false;

    if ((type == static_cast<int>(RuleType::TotalTypes)) || (it->type == type)) {
      if ((condition == static_cast<uint8_t>(RuleCondition::TotalConditions)) || (it->condition == condition)) {
        if ((name == uuids::properties::NotValidUUID) || (it->name == name)) {
          it     = idx.erase(it);
          erased = true;
        }
      }
    }

    if (!erased) {
      it++;
    }
  }
}

void license_pool::distrlimset(uint128_t distributionUUID,
                               uint128_t name,
                               uint16_t  change,
                               uint16_t  remain) {
  agrio_assert(distributionUUID != uuids::properties::NotValidUUID, "UUID is not valid!");
  agrio_assert(name != uuids::properties::NotValidUUID,             "UUID is not valid!");

  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto itDistr  = idxDistr.find(distributionUUID);
  agrio_assert(itDistr != idxDistr.end(), "Distribution delegation doesn't exist!");

  require_auth(itDistr->delegate_from);

  auto idx = _distr_deleg_limits.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(distributionUUID, name));

  if (it == idx.end()) {
    _distr_deleg_limits.emplace(itDistr->delegate_from, [&](auto& rec) {
        rec.id               = _distr_deleg_limits.available_primary_key();
        rec.distributionUUID = distributionUUID;
        rec.name             = name;
        rec.change           = change;
        rec.value            = remain;
      });
  } else {
    idx.modify(it, 0, [&](auto& rec) {
        rec.change = change;
        rec.value  = remain;
      });
  }
}

void license_pool::distrlimrem(uint128_t distributionUUID,
                               uint128_t name) {
  agrio_assert(distributionUUID != uuids::properties::NotValidUUID, "UUID is not valid!");
  agrio_assert(name != uuids::properties::NotValidUUID,             "UUID is not valid!");

  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto itDistr  = idxDistr.find(distributionUUID);
  agrio_assert(itDistr != idxDistr.end(), "Distribution delegation doesn't exist!");

  require_auth(itDistr->delegate_from);

  auto idx = _distr_deleg_limits.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(distributionUUID, name));

  agrio_assert(it != idx.end(), "Distribution delegation license limit doesn't exist!");
  idx.erase(it);
}

// Licenses
void license_pool::liccreate(uint128_t    distributionUUID,
                             uint128_t    licenseUUID,
                             account_name issued_to) {
  agrio_assert(distributionUUID != uuids::properties::NotValidUUID, "UUID is not valid!");
  agrio_assert(licenseUUID != uuids::properties::NotValidUUID,      "UUID is not valid!");
  agrio_assert(is_account(issued_to),                               "Issued_to account is not exist!");

  // check distribution delegation
  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto itDistr  = idxDistr.find(distributionUUID);

  agrio_assert(itDistr != idxDistr.end(), "Distribution delegation doesn't exist!");
  require_auth(itDistr->delegate_to);

  // check offer
  auto idxOffers = _offers.template get_index<N(byofferuuid)>();
  auto itOffers  = idxOffers.find(itDistr->offerUUID);

  agrio_assert(itOffers != idxOffers.end(), "Offer doesn't exist!");

  // check license UUID
  auto idx = _licenses.template get_index<N(bylicuuid)>();
  agrio_assert(idx.find(licenseUUID) == idx.end(), "License is already exist!");

  _licenses.emplace(itDistr->delegate_to, [&](auto& rec) {
      rec.id               = _licenses.available_primary_key();
      rec.state            = static_cast<int>(LicenseState::Reserved);
      rec.licenseUUID      = licenseUUID;
      rec.distributionUUID = distributionUUID;
      rec.issued_from      = itDistr->delegate_to;
      rec.issued_to        = issued_to;
    });
}

void license_pool::licpropset(uint128_t     licenseUUID,
                              uint128_t     name,
                              const string& value) {
  auto idxLicense = _licenses.template get_index<N(bylicuuid)>();
  auto itLicense  = idxLicense.find(licenseUUID);

  agrio_assert(itLicense != idxLicense.end(),                                "License doesn't exist!");
  agrio_assert(itLicense->state == static_cast<int>(LicenseState::Reserved), "License MUST be in reserved state!");

  require_auth(itLicense->issued_from);

  auto idx = _license_properties.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(licenseUUID, name));

  if (it == idx.end()) {
    _license_properties.emplace(_self, [&](auto& rec) {
        rec.id          = _license_properties.available_primary_key();
        rec.licenseUUID = licenseUUID;
        rec.key         = name;
        rec.value       = value;
      });
  } else {
    idx.modify(it, 0, [&](auto& rec) {
        rec.value = value;
      });
  }
}

void license_pool::licproprem(uint128_t licenseUUID,
                              uint128_t name) {
  auto idxLicense = _licenses.template get_index<N(bylicuuid)>();
  auto itLicense  = idxLicense.find(licenseUUID);

  agrio_assert(itLicense != idxLicense.end(),                                "License doesn't exist!");
  agrio_assert(itLicense->state == static_cast<int>(LicenseState::Reserved), "License MUST be in reserved state!");

  require_auth(itLicense->issued_from);

  auto idx = _license_properties.template get_index<N(byuuidkey)>();
  auto it  = idx.find(make_uuids_key(licenseUUID, name));

  agrio_assert(it != idx.end(), "License property doesn't exist!");
  idx.erase(it);
}

void license_pool::licactivate(uint128_t licenseUUID) {
  agrio_assert(licenseUUID != uuids::properties::NotValidUUID, "UUID is not valid!");

  auto idx = _licenses.template get_index<N(bylicuuid)>();
  auto it  = idx.find(licenseUUID);

  agrio_assert(it != idx.end(),                                       "License doesn't exist!");
  agrio_assert(it->state == static_cast<int>(LicenseState::Reserved), "License MUST be in Reserved state!");

  require_auth(it->issued_from);

  // check distribution chain
  check_counters(it->distributionUUID, false);
  check_rules(licenseUUID);

  idx.modify(it, 0, [&](auto& rec) {
      rec.state = static_cast<int>(LicenseState::Active);
    });

  // shift counters
  check_counters(it->distributionUUID, true);
}

void license_pool::licrevoke(uint128_t licenseUUID) {
  agrio_assert(licenseUUID != uuids::properties::NotValidUUID, "UUID is not valid!");

  auto idx = _licenses.template get_index<N(bylicuuid)>();
  auto it  = idx.find(licenseUUID);

  agrio_assert(it != idx.end(),                              "License doesn't exist!");

  // check auth
  agrio_assert(has_auth(_self) || has_auth(it->issued_from), "Invalid credentials!");

  idx.modify(it, 0, [&](auto& rec) {
      rec.state = static_cast<int>(LicenseState::Revoked);
    });
}

void license_pool::check_counters(uint128_t distributionUUID, bool shift) {
  // check distribution delegation limits
  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto idxLimit = _distr_deleg_limits.template get_index<N(bydistruuid)>();

  // moving up to base delegation
  while (distributionUUID != uuids::properties::NotValidUUID) {
    auto itDistr = idxDistr.find(distributionUUID);

    // check validation
    agrio_assert(itDistr != idxDistr.end(), "Broken delegation chain!");

    for (auto itLimit = idxLimit.find(distributionUUID); itLimit != idxLimit.end() && itLimit->distributionUUID == distributionUUID; itLimit++) {
      agrio_assert(itLimit->value >= itLimit->change, "Counter limit exceeded!");

      if (shift) {
        idxLimit.modify(itLimit, 0, [&](auto& rec) {
            rec.value = rec.value - rec.change;
          });
      }
    }

    // move up
    distributionUUID = itDistr->parentUUID;
  }
}

void license_pool::check_rules(uint128_t licenseUUID) {
  auto idxLicense = _licenses.template get_index<N(bylicuuid)>();
  auto itLicense  = idxLicense.find(licenseUUID);

  agrio_assert(itLicense != idxLicense.end(), "License not found!");
  auto distributionUUID = itLicense->distributionUUID;

  // check distribution delegation rules
  auto idxDistr = _distrubution_delegations.template get_index<N(bydistruuid)>();
  auto idxRule  = _distr_deleg_rules.template get_index<N(bydistruuid)>();
  auto idxProp  = _license_properties.template get_index<N(byuuidkey)>();

  // moving up to base delegation
  while (distributionUUID != uuids::properties::NotValidUUID) {
    auto itDistr = idxDistr.find(distributionUUID);

    // check validation
    agrio_assert(itDistr != idxDistr.end(), "Broken delegation chain!");

    // check rules
    for (auto itRule = idxRule.find(distributionUUID); itRule != idxRule.end() && itRule->distributionUUID == distributionUUID; itRule++) {
      // iter license settings
      auto itProp = idxProp.find(make_uuids_key(licenseUUID, itRule->name));
      bool match  = false;

      if (itProp != idxProp.end()) {
        switch (static_cast<RuleCondition>(itRule->condition)) {
        case RuleCondition::Less:
          match = (itProp->value < itRule->value);
          break;

        case RuleCondition::LessEqual:
          match = (itProp->value <= itRule->value);
          break;

        case RuleCondition::Equal:
          match = (itProp->value == itRule->value);
          break;

        case RuleCondition::GreaterEqual:
          match = (itProp->value >= itRule->value);
          break;

        case RuleCondition::Greater:
          match = (itProp->value > itRule->value);
          break;

        case RuleCondition::OneFromList:
        case RuleCondition::AllFromList:
        case RuleCondition::TotalConditions:
          break;

        default:
          agrio_assert(false, "Invalid rule condition!");
        }

        agrio_assert(itRule->type != static_cast<int>(RuleType::Allow) || match, "The allow property doesn't match the value!");
        agrio_assert(itRule->type != static_cast<int>(RuleType::Deny) || !match, "The deny property doesn't match the value!");
      }
    }

    // move up
    distributionUUID = itDistr->parentUUID;
  }
}
}

AGRIO_ABI(boss::license_pool,(initialize)(offerset)(offerpropset)(offerproprem)(distrcreate)(distrruleset)(distrrulerem)(distrlimset)(distrlimrem)(liccreate)(licpropset)(licproprem)(licactivate)(licrevoke))
