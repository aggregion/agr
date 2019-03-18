/**
 *  @file
 *  @copyright defined in agr/LICENSE
 */
#include <agriolib/agrio.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   agrio_assert( sizeof(int64_t)   ==  8, "int64_t size != 8"   );
   agrio_assert( sizeof(uint64_t)  ==  8, "uint64_t size != 8"  );
   agrio_assert( sizeof(uint32_t)  ==  4, "uint32_t size != 4"  );
   agrio_assert( sizeof(int32_t)   ==  4, "int32_t size != 4"   );
   agrio_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   agrio_assert( sizeof(int128_t)  == 16, "int128_t size != 16" );
   agrio_assert( sizeof(uint8_t)   ==  1, "uint8_t size != 1"   );

   agrio_assert( sizeof(agrio::name) ==  8, "name size !=  8");
}

void test_types::char_to_symbol() {

   agrio_assert( agrio::name::char_to_value('1') ==  1, "agrio::char_to_symbol('1') !=  1" );
   agrio_assert( agrio::name::char_to_value('2') ==  2, "agrio::char_to_symbol('2') !=  2" );
   agrio_assert( agrio::name::char_to_value('3') ==  3, "agrio::char_to_symbol('3') !=  3" );
   agrio_assert( agrio::name::char_to_value('4') ==  4, "agrio::char_to_symbol('4') !=  4" );
   agrio_assert( agrio::name::char_to_value('5') ==  5, "agrio::char_to_symbol('5') !=  5" );
   agrio_assert( agrio::name::char_to_value('a') ==  6, "agrio::char_to_symbol('a') !=  6" );
   agrio_assert( agrio::name::char_to_value('b') ==  7, "agrio::char_to_symbol('b') !=  7" );
   agrio_assert( agrio::name::char_to_value('c') ==  8, "agrio::char_to_symbol('c') !=  8" );
   agrio_assert( agrio::name::char_to_value('d') ==  9, "agrio::char_to_symbol('d') !=  9" );
   agrio_assert( agrio::name::char_to_value('e') == 10, "agrio::char_to_symbol('e') != 10" );
   agrio_assert( agrio::name::char_to_value('f') == 11, "agrio::char_to_symbol('f') != 11" );
   agrio_assert( agrio::name::char_to_value('g') == 12, "agrio::char_to_symbol('g') != 12" );
   agrio_assert( agrio::name::char_to_value('h') == 13, "agrio::char_to_symbol('h') != 13" );
   agrio_assert( agrio::name::char_to_value('i') == 14, "agrio::char_to_symbol('i') != 14" );
   agrio_assert( agrio::name::char_to_value('j') == 15, "agrio::char_to_symbol('j') != 15" );
   agrio_assert( agrio::name::char_to_value('k') == 16, "agrio::char_to_symbol('k') != 16" );
   agrio_assert( agrio::name::char_to_value('l') == 17, "agrio::char_to_symbol('l') != 17" );
   agrio_assert( agrio::name::char_to_value('m') == 18, "agrio::char_to_symbol('m') != 18" );
   agrio_assert( agrio::name::char_to_value('n') == 19, "agrio::char_to_symbol('n') != 19" );
   agrio_assert( agrio::name::char_to_value('o') == 20, "agrio::char_to_symbol('o') != 20" );
   agrio_assert( agrio::name::char_to_value('p') == 21, "agrio::char_to_symbol('p') != 21" );
   agrio_assert( agrio::name::char_to_value('q') == 22, "agrio::char_to_symbol('q') != 22" );
   agrio_assert( agrio::name::char_to_value('r') == 23, "agrio::char_to_symbol('r') != 23" );
   agrio_assert( agrio::name::char_to_value('s') == 24, "agrio::char_to_symbol('s') != 24" );
   agrio_assert( agrio::name::char_to_value('t') == 25, "agrio::char_to_symbol('t') != 25" );
   agrio_assert( agrio::name::char_to_value('u') == 26, "agrio::char_to_symbol('u') != 26" );
   agrio_assert( agrio::name::char_to_value('v') == 27, "agrio::char_to_symbol('v') != 27" );
   agrio_assert( agrio::name::char_to_value('w') == 28, "agrio::char_to_symbol('w') != 28" );
   agrio_assert( agrio::name::char_to_value('x') == 29, "agrio::char_to_symbol('x') != 29" );
   agrio_assert( agrio::name::char_to_value('y') == 30, "agrio::char_to_symbol('y') != 30" );
   agrio_assert( agrio::name::char_to_value('z') == 31, "agrio::char_to_symbol('z') != 31" );

   for(unsigned char i = 0; i<255; i++) {
      if( (i >= 'a' && i <= 'z') || (i >= '1' || i <= '5') ) continue;
      agrio_assert( agrio::name::char_to_value((char)i) == 0, "agrio::char_to_symbol() != 0" );
   }
}

void test_types::string_to_name() {
   return;
   agrio_assert( agrio::name("a") == "a"_n, "agrio::string_to_name(a)" );
   agrio_assert( agrio::name("ba") == "ba"_n, "agrio::string_to_name(ba)" );
   agrio_assert( agrio::name("cba") == "cba"_n, "agrio::string_to_name(cba)" );
   agrio_assert( agrio::name("dcba") == "dcba"_n, "agrio::string_to_name(dcba)" );
   agrio_assert( agrio::name("edcba") == "edcba"_n, "agrio::string_to_name(edcba)" );
   agrio_assert( agrio::name("fedcba") == "fedcba"_n, "agrio::string_to_name(fedcba)" );
   agrio_assert( agrio::name("gfedcba") == "gfedcba"_n, "agrio::string_to_name(gfedcba)" );
   agrio_assert( agrio::name("hgfedcba") == "hgfedcba"_n, "agrio::string_to_name(hgfedcba)" );
   agrio_assert( agrio::name("ihgfedcba") == "ihgfedcba"_n, "agrio::string_to_name(ihgfedcba)" );
   agrio_assert( agrio::name("jihgfedcba") == "jihgfedcba"_n, "agrio::string_to_name(jihgfedcba)" );
   agrio_assert( agrio::name("kjihgfedcba") == "kjihgfedcba"_n, "agrio::string_to_name(kjihgfedcba)" );
   agrio_assert( agrio::name("lkjihgfedcba") == "lkjihgfedcba"_n, "agrio::string_to_name(lkjihgfedcba)" );
   agrio_assert( agrio::name("mlkjihgfedcba") == "mlkjihgfedcba"_n, "agrio::string_to_name(mlkjihgfedcba)" );
}
