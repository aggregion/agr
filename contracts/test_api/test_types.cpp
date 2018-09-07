/**
 *  @file
 *  @copyright defined in agr/LICENSE.txt
 */
#include <agriolib/agrio.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   agrio_assert( sizeof(int64_t) == 8, "int64_t size != 8");
   agrio_assert( sizeof(uint64_t) ==  8, "uint64_t size != 8");
   agrio_assert( sizeof(uint32_t) ==  4, "uint32_t size != 4");
   agrio_assert( sizeof(int32_t) ==  4, "int32_t size != 4");
   agrio_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   agrio_assert( sizeof(int128_t) == 16, "int128_t size != 16");
   agrio_assert( sizeof(uint8_t) ==  1, "uint8_t size != 1");

   agrio_assert( sizeof(account_name) ==  8, "account_name size !=  8");
   agrio_assert( sizeof(table_name) ==  8, "table_name size !=  8");
   agrio_assert( sizeof(time) ==  4, "time size !=  4");
   agrio_assert( sizeof(agrio::key256) == 32, "key256 size != 32" );
}

void test_types::char_to_symbol() {

   agrio_assert( agrio::char_to_symbol('1') ==  1, "agrio::char_to_symbol('1') !=  1");
   agrio_assert( agrio::char_to_symbol('2') ==  2, "agrio::char_to_symbol('2') !=  2");
   agrio_assert( agrio::char_to_symbol('3') ==  3, "agrio::char_to_symbol('3') !=  3");
   agrio_assert( agrio::char_to_symbol('4') ==  4, "agrio::char_to_symbol('4') !=  4");
   agrio_assert( agrio::char_to_symbol('5') ==  5, "agrio::char_to_symbol('5') !=  5");
   agrio_assert( agrio::char_to_symbol('a') ==  6, "agrio::char_to_symbol('a') !=  6");
   agrio_assert( agrio::char_to_symbol('b') ==  7, "agrio::char_to_symbol('b') !=  7");
   agrio_assert( agrio::char_to_symbol('c') ==  8, "agrio::char_to_symbol('c') !=  8");
   agrio_assert( agrio::char_to_symbol('d') ==  9, "agrio::char_to_symbol('d') !=  9");
   agrio_assert( agrio::char_to_symbol('e') == 10, "agrio::char_to_symbol('e') != 10");
   agrio_assert( agrio::char_to_symbol('f') == 11, "agrio::char_to_symbol('f') != 11");
   agrio_assert( agrio::char_to_symbol('g') == 12, "agrio::char_to_symbol('g') != 12");
   agrio_assert( agrio::char_to_symbol('h') == 13, "agrio::char_to_symbol('h') != 13");
   agrio_assert( agrio::char_to_symbol('i') == 14, "agrio::char_to_symbol('i') != 14");
   agrio_assert( agrio::char_to_symbol('j') == 15, "agrio::char_to_symbol('j') != 15");
   agrio_assert( agrio::char_to_symbol('k') == 16, "agrio::char_to_symbol('k') != 16");
   agrio_assert( agrio::char_to_symbol('l') == 17, "agrio::char_to_symbol('l') != 17");
   agrio_assert( agrio::char_to_symbol('m') == 18, "agrio::char_to_symbol('m') != 18");
   agrio_assert( agrio::char_to_symbol('n') == 19, "agrio::char_to_symbol('n') != 19");
   agrio_assert( agrio::char_to_symbol('o') == 20, "agrio::char_to_symbol('o') != 20");
   agrio_assert( agrio::char_to_symbol('p') == 21, "agrio::char_to_symbol('p') != 21");
   agrio_assert( agrio::char_to_symbol('q') == 22, "agrio::char_to_symbol('q') != 22");
   agrio_assert( agrio::char_to_symbol('r') == 23, "agrio::char_to_symbol('r') != 23");
   agrio_assert( agrio::char_to_symbol('s') == 24, "agrio::char_to_symbol('s') != 24");
   agrio_assert( agrio::char_to_symbol('t') == 25, "agrio::char_to_symbol('t') != 25");
   agrio_assert( agrio::char_to_symbol('u') == 26, "agrio::char_to_symbol('u') != 26");
   agrio_assert( agrio::char_to_symbol('v') == 27, "agrio::char_to_symbol('v') != 27");
   agrio_assert( agrio::char_to_symbol('w') == 28, "agrio::char_to_symbol('w') != 28");
   agrio_assert( agrio::char_to_symbol('x') == 29, "agrio::char_to_symbol('x') != 29");
   agrio_assert( agrio::char_to_symbol('y') == 30, "agrio::char_to_symbol('y') != 30");
   agrio_assert( agrio::char_to_symbol('z') == 31, "agrio::char_to_symbol('z') != 31");

   for(unsigned char i = 0; i<255; i++) {
      if((i >= 'a' && i <= 'z') || (i >= '1' || i <= '5')) continue;
      agrio_assert( agrio::char_to_symbol((char)i) == 0, "agrio::char_to_symbol() != 0");
   }
}

void test_types::string_to_name() {

   agrio_assert( agrio::string_to_name("a") == N(a) , "agrio::string_to_name(a)" );
   agrio_assert( agrio::string_to_name("ba") == N(ba) , "agrio::string_to_name(ba)" );
   agrio_assert( agrio::string_to_name("cba") == N(cba) , "agrio::string_to_name(cba)" );
   agrio_assert( agrio::string_to_name("dcba") == N(dcba) , "agrio::string_to_name(dcba)" );
   agrio_assert( agrio::string_to_name("edcba") == N(edcba) , "agrio::string_to_name(edcba)" );
   agrio_assert( agrio::string_to_name("fedcba") == N(fedcba) , "agrio::string_to_name(fedcba)" );
   agrio_assert( agrio::string_to_name("gfedcba") == N(gfedcba) , "agrio::string_to_name(gfedcba)" );
   agrio_assert( agrio::string_to_name("hgfedcba") == N(hgfedcba) , "agrio::string_to_name(hgfedcba)" );
   agrio_assert( agrio::string_to_name("ihgfedcba") == N(ihgfedcba) , "agrio::string_to_name(ihgfedcba)" );
   agrio_assert( agrio::string_to_name("jihgfedcba") == N(jihgfedcba) , "agrio::string_to_name(jihgfedcba)" );
   agrio_assert( agrio::string_to_name("kjihgfedcba") == N(kjihgfedcba) , "agrio::string_to_name(kjihgfedcba)" );
   agrio_assert( agrio::string_to_name("lkjihgfedcba") == N(lkjihgfedcba) , "agrio::string_to_name(lkjihgfedcba)" );
   agrio_assert( agrio::string_to_name("mlkjihgfedcba") == N(mlkjihgfedcba) , "agrio::string_to_name(mlkjihgfedcba)" );
   agrio_assert( agrio::string_to_name("mlkjihgfedcba1") == N(mlkjihgfedcba2) , "agrio::string_to_name(mlkjihgfedcba2)" );
   agrio_assert( agrio::string_to_name("mlkjihgfedcba55") == N(mlkjihgfedcba14) , "agrio::string_to_name(mlkjihgfedcba14)" );

   agrio_assert( agrio::string_to_name("azAA34") == N(azBB34) , "agrio::string_to_name N(azBB34)" );
   agrio_assert( agrio::string_to_name("AZaz12Bc34") == N(AZaz12Bc34) , "agrio::string_to_name AZaz12Bc34" );
   agrio_assert( agrio::string_to_name("AAAAAAAAAAAAAAA") == agrio::string_to_name("BBBBBBBBBBBBBDDDDDFFFGG") , "agrio::string_to_name BBBBBBBBBBBBBDDDDDFFFGG" );
}

void test_types::name_class() {

   agrio_assert( agrio::name{agrio::string_to_name("azAA34")}.value == N(azAA34), "agrio::name != N(azAA34)" );
   agrio_assert( agrio::name{agrio::string_to_name("AABBCC")}.value == 0, "agrio::name != N(0)" );
   agrio_assert( agrio::name{agrio::string_to_name("AA11")}.value == N(AA11), "agrio::name != N(AA11)" );
   agrio_assert( agrio::name{agrio::string_to_name("11AA")}.value == N(11), "agrio::name != N(11)" );
   agrio_assert( agrio::name{agrio::string_to_name("22BBCCXXAA")}.value == N(22), "agrio::name != N(22)" );
   agrio_assert( agrio::name{agrio::string_to_name("AAAbbcccdd")} == agrio::name{agrio::string_to_name("AAAbbcccdd")}, "agrio::name == agrio::name" );

   uint64_t tmp = agrio::name{agrio::string_to_name("11bbcccdd")};
   agrio_assert(N(11bbcccdd) == tmp, "N(11bbcccdd) == tmp");
}
