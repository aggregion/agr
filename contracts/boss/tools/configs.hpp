#pragma once
#include <agriolib/agrio.hpp>

namespace configs {
template<typename Config>
bool get(Config& out, const account_name& self, uint64_t table) {
  auto it = db_find_i64(self, self, table, Config::key);

  if (it != -1) {
    auto size = db_get_i64(it, (char *)&out, sizeof(Config));
    agrio_assert(size == sizeof(Config), "Wrong record size");
    return true;
  } else {
    return false;
  }
}

template<typename Config>
void store(const Config& in, const account_name& self, uint64_t table) {
  auto it = db_find_i64(self, self, table, Config::key);

  if (it != -1) {
    db_update_i64(it, self, (const char *)&in, sizeof(Config));
  } else {
    db_store_i64(self, table, self, Config::key, (const char *)&in, sizeof(Config));
  }
}
}
