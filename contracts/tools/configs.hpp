#pragma once
#include <agriolib/agrio.hpp>
#include <agriolib/serialize.hpp>
#include <agriolib/datastream.hpp>

namespace configs {
template<typename Config>
bool get(Config& out, const account_name& self, uint64_t table) {
  auto it = db_find_i64(self, self, table, Config::key);

  if (it != -1) {
    auto size = db_get_i64(it, nullptr, 0);
    agrio_assert(size >= 0, "error reading iterator");
    void *buffer = malloc(size_t(size));
    db_get_i64(it, buffer, static_cast<uint32_t>(size));

    agrio::datastream<const char *> ds((char *)buffer, uint32_t(size));
    ds >> out;

    free(buffer);
    return true;
  } else {
    return false;
  }
}

template<typename Config>
void store(const Config& in, const account_name& self, uint64_t table) {
  size_t size = agrio::pack_size(in);

  char *buffer = reinterpret_cast<char *>(malloc(size));

  agrio::datastream<char *> ds(buffer, size);
  ds << in;

  auto it = db_find_i64(self, self, table, Config::key);

  if (it != -1) {
    db_update_i64(it, self, buffer, size);
  } else {
    db_store_i64(self, table, self, Config::key, buffer, static_cast<uint32_t>(size));
  }

  free(buffer);
}
}
