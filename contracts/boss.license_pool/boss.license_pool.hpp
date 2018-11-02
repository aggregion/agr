/**
 *  @file
 *  @copyright defined in boss/LICENSE.txt
 */
#pragma once
#include <agriolib/asset.hpp>
#include <agriolib/agrio.hpp>
#include <agriolib/multi_index.hpp>
#include <agriolib/contract.hpp>

#include "../uuids/properties.hpp"

using std::string;
using std::map;

using agrio::indexed_by;
using agrio::key256;
using agrio::multi_index;
using boost::multi_index::const_mem_fun;

namespace boss {
class license_pool : public agrio::contract {
public:

  enum class RuleCondition: uint8_t {
    Less = 0,
    LessEqual,
    Equal,
    MoreEqual,
    More,
    OneFromList,
    AllFromList,
    NotValidCondition
  };

  enum class RuleType: uint8_t {
    Allow = 0,
    Deny,
    NotValidRule
  };

  enum class LicenseState: uint8_t {
    Reserved = 0,
    Active,
    Revoked,
    NotValidState
  };

  license_pool(account_name issuer);

  void initialize(const string& name,
                  const string& description);

  // License Offers
  void offerset(uint128_t     offerUUID,
                account_name  owner,
                const string& name,
                const string& description);
  void offerpropset(uint128_t     offerUUID,
                    uint128_t     name,
                    const string& value);
  void offerproprem(uint128_t     offerUUID,
                    uint128_t     name,
                    const string& value);

  // Distribution Delegations
  void distrcreate(uint128_t    distributionUUID,
                   uint128_t    parentUUID,
                   uint128_t    offerUUID,
                   account_name deleg_to);
  void distrruleset(uint128_t     distributionUUID,
                    uint8_t       type,
                    uint8_t       condition,
                    uint128_t     name,
                    const string& value);
  void distrrulerem(uint128_t     distributionUUID,
                    uint8_t       type,
                    uint8_t       condition,
                    uint128_t     name,
                    const string& value);
  void distrlimset(uint128_t distributionUUID,
                   uint128_t name,
                   uint16_t  change,
                   uint16_t  remain);
  void distrlimrem(uint128_t distributionUUID,
                   uint128_t name);

  // Licenses
  void liccreate(uint128_t    distributionUUID,
                 uint128_t    licenseUUID,
                 account_name issued_to);
  void licpropset(uint128_t     licenseUUID,
                  uint128_t     name,
                  const string& value);
  void licproprem(uint128_t     licenseUUID,
                  uint128_t     name,
                  const string& value);
  void licactivate(uint128_t licenseUUID);
  void licrevoke(uint128_t licenseUUID);

  // @abi table info i64
  struct info {
    string name;
    string description;

    info() {}

    constexpr static uint64_t key = N(info);
    AGRLIB_SERIALIZE(info, (name)(description))
  };

  static key256 make_uuids_key(uint128_t r, uint128_t k) {
    return key256::make_from_word_sequence<uint64_t>(
      static_cast<uint64_t>(r & 0xFFFFFFFFFFFFFFFF),
      static_cast<uint64_t>(r >> 64),
      static_cast<uint64_t>(k & 0xFFFFFFFFFFFFFFFF),
      static_cast<uint64_t>(k >> 64));
  }

  // @abi table license i64
  struct licenseoffer {
    uint64_t     id;
    uint128_t    offerUUID;
    account_name owner;
    string       name;
    string       description;

    licenseoffer() = default;

    uint64_t primary_key() const {
      return id;
    }

    uint128_t byofferuuid() const {
      return offerUUID;
    }

    AGRLIB_SERIALIZE(licenseoffer, (id)(offerUUID)(name)(description))
  };
  typedef multi_index<N(licenseoffer), licenseoffer,
                      indexed_by<N(byofferuuid), const_mem_fun<licenseoffer, uint128_t, &license::byofferuuid> > >license_offer_index;

  // @abi table offerprops i64
  struct offerprops {
    uint64_t  id;
    uint128_t offerUUID;
    uint128_t key;
    string    value;

    uint64_t primary_key() const {
      return id;
    }

    key256 byuuidkey() const {
      return make_uuids_key(offerUUID, key);
    }

    AGRLIB_SERIALIZE(offerprops, (id)(offerUUID)(key)(value))
  };
  typedef multi_index<N(licsetting), offerprops, indexed_by<N(byuuidkey), const_mem_fun<offerprops, key256, &offerprops::byuuidkey> > >
    offerprops_index;

  // @abi table license i64
  struct license {
    uint64_t     id;
    uint8_t      state;
    uint128_t    licenseUUID;
    uint128_t    distributionUUID;
    account_name issued_from;
    account_name issued_to;

    uint64_t primary_key() const {
      return id;
    }

    uint128_t bylicuuid() const {
      return licenseUUID;
    }

    AGRLIB_SERIALIZE(license, (id)(state)(licenseUUID)(distributionUUID)(issued_from)(issued_to))
  };
  typedef multi_index<N(license), license, indexed_by<N(bylicuuid), const_mem_fun<license, uint128_t, &license::bylicuuid> > > license_index;

  // @abi table licprops i64
  struct licprops {
    uint64_t  id;
    uint128_t licenseUUID;
    uint128_t key;
    string    value;

    uint64_t primary_key() const {
      return id;
    }

    key256 byuuidkey() const {
      return make_uuids_key(licenseUUID, key);
    }

    AGRLIB_SERIALIZE(licprops, (id)(recordUUID)(key)(value))
  };
  typedef multi_index<N(licprops), licprops, indexed_by<N(byuuidkey), const_mem_fun<licprops, key256, &licprops::byuuidkey> > > licprops_index;

  // @abi table distrdelg i64
  struct distrdelg {
    uint64_t     id;
    uint128_t    distributionUUID;
    uint128_t    parentUUID;
    uint128_t    offerUUID;
    account_name delegate_from;
    account_name delegate_to;

    uint64_t primary_key() const {
      return id;
    }

    uint128_t bydistruuid() const {
      return distributionUUID;
    }

    AGRLIB_SERIALIZE(distrdelg, (id)(distributionUUID)(parentUUID)(offerUUID)(delegate_from)(delegate_to))
  };
  typedef multi_index<N(distrdelg), distrdelg, indexed_by<N(bydistruuid), const_mem_fun<distrdelg, uint128_t, &distrdelg::bydistruuid> > >
    distrdelg_index;

  // @abi table distrrule i64
  struct distrrule {
    uint64_t  id;
    uint128_t distributionUUID;
    uint16_t  type;
    uint16_t  condition;
    uint128_t name;
    string    value;

    uint64_t primary_key() const {
      return id;
    }

    uint128_t bydistruuid() const {
      return distributionUUID;
    }

    AGRLIB_SERIALIZE(rule, (id)(distributionUUID)(type)(condition)(name)(value))
  };
  typedef multi_index<N(distr_lic_rule), distr_delg, indexed_by<N(bydistruuid)> > distrrule_index;

  // @abi table distrlimit i64
  struct distrlimit {
    uint64_t  id;
    uint128_t distributionUUID;
    uint128_t name;
    uint32_t  change;
    uint32_t  value;

    uint64_t primary_key() const {
      return id;
    }

    uint128_t bydistruuid() const {
      return distributionUUID;
    }

    key256 byuuidkey() const {
      return make_uuids_key(distributionUUID, name);
    }

    AGRLIB_SERIALIZE(rule, (id)(distributionUUID)(name)(change)(value))
  };
  typedef multi_index<N(distrlimit), distr_delg,
                      indexed_by<N(byuuidkey), const_mem_fun<distrlimit, key256, &distrlimit::byuuidkey> >,
                      indexed_by<N(bydistruuid), const_mem_fun<distrlimit, uint128_t, &distrlimit::bydistruuid> > >distrlimit_index;

protected:

  info _info;
  license_offer_index _offers;
  offerprops_index _offer_properties;
  license_index _licenses;
  licprops_index _license_properties;
  distrdelg_index _distrubution_delegations;
  distrrule_index _distr_deleg_rules;
  distrlimit_index _distr_deleg_limits;

  void check_counters(uint128_t distributionUUID,
                      bool      shift);
  bool check_rules(uint128_t licenseUUID);
};
}
