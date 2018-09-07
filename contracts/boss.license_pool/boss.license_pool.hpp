/**
 *  @file
 *  @copyright defined in boss/LICENSE.txt
 */
#pragma once
#include <agriolib/asset.hpp>
#include <agriolib/agrio.hpp>
#include <agriolib/multi_index.hpp>

#include "../tools/claimable.hpp"
#include "../uuids/properties.hpp"

using std::string;
using std::vector;
using std::initializer_list;

using agrio::indexed_by;
using agrio::key256;
using agrio::multi_index;
using boost::multi_index::const_mem_fun;

namespace boss {
class license_pool : public tools::claimable {
public:

  enum class RuleCondition {
    Less = 0,
    LessEqual,
    Equal,
    MoreEqual,
    More,
    NotValidCondition
  };

  license_pool(account_name issuer);

  void initialize(const string& name,
                  const string& description);

  void liccreate(uint128_t     licenseUUID,
                 const string& name,
                 const string& description);
  void licsetprop(uint128_t     licenseUUID,
                  uint128_t     key,
                  const string& value);

  void typeadd(uint128_t     licenseUUID,
               const string& licenseType);
  void typeremove(uint128_t     licenseUUID,
                  const string& licenseType);

  void ruleadd(const string& property,
               uint16_t      condition,
               const string& value,
               const string& actions);
  void rulesremove(const string& property,
                   uint16_t      condition,
                   const string& value);

  void usrsetprop(account_name  user,
                  uint128_t     key,
                  const string& value);

  void recadd(uint128_t     licenseUUID,
              uint128_t     recordUUID,
              account_name  user,
              const string& licenseType,
              const string& extra);
  void recsetprop(uint128_t     recordUUID,
                  uint128_t     key,
                  const string& value);
  void recremove(uint128_t recordUUID);

  // @abi table state i64
  struct state {
    string name;
    string description;

    state() {}

    constexpr static uint64_t key = N(state);
    AGRLIB_SERIALIZE(state, (name)(description))
  };

  static key256 make_uuids_key(uint128_t r, uint128_t k) {
    return key256::make_from_word_sequence<uint64_t>(
      static_cast<uint64_t>(r & 0xFFFFFFFFFFFFFFFF),
      static_cast<uint64_t>(r >> 64),
      static_cast<uint64_t>(k & 0xFFFFFFFFFFFFFFFF),
      static_cast<uint64_t>(k >> 64));
  }

  // @abi table license i64
  struct license {
    uint64_t      id;
    uint128_t     licenseUUID;
    string        name;
    string        description;
    vector<string>types;

    license() {}

    uint64_t primary_key() const {
      return id;
    }

    uint128_t bylicuuid() const {
      return licenseUUID;
    }

    AGRLIB_SERIALIZE(license, (id)(licenseUUID)(name)(description)(types))
  };
  typedef multi_index<N(license), license, indexed_by<N(bylicuuid), const_mem_fun<license, uint128_t, &license::bylicuuid> > > license_index;

  // @abi table licsetting i64
  struct lic_settings {
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

    AGRLIB_SERIALIZE(lic_settings, (id)(licenseUUID)(key)(value))
  };
  typedef multi_index<N(licsetting), lic_settings, indexed_by<N(byuuidkey), const_mem_fun<lic_settings, key256, &lic_settings::byuuidkey> > >
    license_setting_index;

  // @abi table rule i64
  struct rule {
    uint64_t id;
    string   property;
    uint16_t condition;
    string   value;
    string   actions;

    uint64_t primary_key() const {
      return id;
    }

    AGRLIB_SERIALIZE(rule, (id)(property)(condition)(value)(actions))
  };
  typedef multi_index<N(rule), rule> rule_index;

  // @abi table setting i64
  struct setting {
    uint64_t     id;
    account_name user;
    uint128_t    key;
    string       value;

    uint64_t primary_key() const {
      return id;
    }

    key256 by_setting() const {
      return key256::make_from_word_sequence<uint64_t>(
        static_cast<uint64_t>(user),
        static_cast<uint64_t>(key & 0xFFFFFFFFFFFFFFFF),
        static_cast<uint64_t>(key >> 64),
        static_cast<uint64_t>(0));
    }

    AGRLIB_SERIALIZE(setting, (id)(user)(key)(value))
  };
  typedef multi_index<N(setting), setting, indexed_by<N(bysetting), const_mem_fun<setting, key256, &setting::by_setting> >
                      >user_setting_index;


  // @abi table record i64
  struct record {
    uint64_t     id;
    uint128_t    recordUUID;
    uint128_t    licenseUUID;
    account_name user;
    account_name creator;
    string       licenseType;

    uint64_t primary_key() const {
      return id;
    }

    uint128_t byrecuuid() const {
      return recordUUID;
    }

    AGRLIB_SERIALIZE(record, (id)(recordUUID)(licenseUUID)(user)(creator)(licenseType))
  };
  typedef multi_index<N(record), record, indexed_by<N(byrecuuid), const_mem_fun<record, uint128_t, &record::byrecuuid> > >
    record_index;

  // @abi table recsetting i64
  struct rec_settings {
    uint64_t  id;
    uint128_t recordUUID;
    uint128_t key;
    string    value;

    uint64_t primary_key() const {
      return id;
    }

    key256 byrecuuid() const {
      return make_uuids_key(recordUUID, key);
    }

    AGRLIB_SERIALIZE(rec_settings, (id)(recordUUID)(key)(value))
  };
  typedef multi_index<N(recsetting), rec_settings, indexed_by<N(byrecuuid), const_mem_fun<rec_settings, key256, &rec_settings::byrecuuid> > >
    record_setting_index;

protected:

  typedef struct {
    account_name user = 0;
    string       role;
  } user_context;

  state _state;
  license_index _licenses;
  license_setting_index _lic_settings;
  rule_index _rules;
  user_setting_index _settings;
  record_index _records;
  record_setting_index _record_settings;

  bool check_role(account_name  user,
                  const string& role);

  // check if caller user responds on of the role and return userinfo
  user_context check_caller_role(initializer_list<string>allowRolesList);
};
}
