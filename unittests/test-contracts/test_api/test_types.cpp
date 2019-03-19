/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#include <enulib/enu.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   enumivo_assert( sizeof(int64_t)   ==  8, "int64_t size != 8"   );
   enumivo_assert( sizeof(uint64_t)  ==  8, "uint64_t size != 8"  );
   enumivo_assert( sizeof(uint32_t)  ==  4, "uint32_t size != 4"  );
   enumivo_assert( sizeof(int32_t)   ==  4, "int32_t size != 4"   );
   enumivo_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   enumivo_assert( sizeof(int128_t)  == 16, "int128_t size != 16" );
   enumivo_assert( sizeof(uint8_t)   ==  1, "uint8_t size != 1"   );

   enumivo_assert( sizeof(enumivo::name) ==  8, "name size !=  8");
}

void test_types::char_to_symbol() {

   enumivo_assert( enumivo::name::char_to_value('1') ==  1, "enumivo::char_to_symbol('1') !=  1" );
   enumivo_assert( enumivo::name::char_to_value('2') ==  2, "enumivo::char_to_symbol('2') !=  2" );
   enumivo_assert( enumivo::name::char_to_value('3') ==  3, "enumivo::char_to_symbol('3') !=  3" );
   enumivo_assert( enumivo::name::char_to_value('4') ==  4, "enumivo::char_to_symbol('4') !=  4" );
   enumivo_assert( enumivo::name::char_to_value('5') ==  5, "enumivo::char_to_symbol('5') !=  5" );
   enumivo_assert( enumivo::name::char_to_value('a') ==  6, "enumivo::char_to_symbol('a') !=  6" );
   enumivo_assert( enumivo::name::char_to_value('b') ==  7, "enumivo::char_to_symbol('b') !=  7" );
   enumivo_assert( enumivo::name::char_to_value('c') ==  8, "enumivo::char_to_symbol('c') !=  8" );
   enumivo_assert( enumivo::name::char_to_value('d') ==  9, "enumivo::char_to_symbol('d') !=  9" );
   enumivo_assert( enumivo::name::char_to_value('e') == 10, "enumivo::char_to_symbol('e') != 10" );
   enumivo_assert( enumivo::name::char_to_value('f') == 11, "enumivo::char_to_symbol('f') != 11" );
   enumivo_assert( enumivo::name::char_to_value('g') == 12, "enumivo::char_to_symbol('g') != 12" );
   enumivo_assert( enumivo::name::char_to_value('h') == 13, "enumivo::char_to_symbol('h') != 13" );
   enumivo_assert( enumivo::name::char_to_value('i') == 14, "enumivo::char_to_symbol('i') != 14" );
   enumivo_assert( enumivo::name::char_to_value('j') == 15, "enumivo::char_to_symbol('j') != 15" );
   enumivo_assert( enumivo::name::char_to_value('k') == 16, "enumivo::char_to_symbol('k') != 16" );
   enumivo_assert( enumivo::name::char_to_value('l') == 17, "enumivo::char_to_symbol('l') != 17" );
   enumivo_assert( enumivo::name::char_to_value('m') == 18, "enumivo::char_to_symbol('m') != 18" );
   enumivo_assert( enumivo::name::char_to_value('n') == 19, "enumivo::char_to_symbol('n') != 19" );
   enumivo_assert( enumivo::name::char_to_value('o') == 20, "enumivo::char_to_symbol('o') != 20" );
   enumivo_assert( enumivo::name::char_to_value('p') == 21, "enumivo::char_to_symbol('p') != 21" );
   enumivo_assert( enumivo::name::char_to_value('q') == 22, "enumivo::char_to_symbol('q') != 22" );
   enumivo_assert( enumivo::name::char_to_value('r') == 23, "enumivo::char_to_symbol('r') != 23" );
   enumivo_assert( enumivo::name::char_to_value('s') == 24, "enumivo::char_to_symbol('s') != 24" );
   enumivo_assert( enumivo::name::char_to_value('t') == 25, "enumivo::char_to_symbol('t') != 25" );
   enumivo_assert( enumivo::name::char_to_value('u') == 26, "enumivo::char_to_symbol('u') != 26" );
   enumivo_assert( enumivo::name::char_to_value('v') == 27, "enumivo::char_to_symbol('v') != 27" );
   enumivo_assert( enumivo::name::char_to_value('w') == 28, "enumivo::char_to_symbol('w') != 28" );
   enumivo_assert( enumivo::name::char_to_value('x') == 29, "enumivo::char_to_symbol('x') != 29" );
   enumivo_assert( enumivo::name::char_to_value('y') == 30, "enumivo::char_to_symbol('y') != 30" );
   enumivo_assert( enumivo::name::char_to_value('z') == 31, "enumivo::char_to_symbol('z') != 31" );

   for(unsigned char i = 0; i<255; i++) {
      if( (i >= 'a' && i <= 'z') || (i >= '1' || i <= '5') ) continue;
      enumivo_assert( enumivo::name::char_to_value((char)i) == 0, "enumivo::char_to_symbol() != 0" );
   }
}

void test_types::string_to_name() {
   return;
   enumivo_assert( enumivo::name("a") == "a"_n, "enumivo::string_to_name(a)" );
   enumivo_assert( enumivo::name("ba") == "ba"_n, "enumivo::string_to_name(ba)" );
   enumivo_assert( enumivo::name("cba") == "cba"_n, "enumivo::string_to_name(cba)" );
   enumivo_assert( enumivo::name("dcba") == "dcba"_n, "enumivo::string_to_name(dcba)" );
   enumivo_assert( enumivo::name("edcba") == "edcba"_n, "enumivo::string_to_name(edcba)" );
   enumivo_assert( enumivo::name("fedcba") == "fedcba"_n, "enumivo::string_to_name(fedcba)" );
   enumivo_assert( enumivo::name("gfedcba") == "gfedcba"_n, "enumivo::string_to_name(gfedcba)" );
   enumivo_assert( enumivo::name("hgfedcba") == "hgfedcba"_n, "enumivo::string_to_name(hgfedcba)" );
   enumivo_assert( enumivo::name("ihgfedcba") == "ihgfedcba"_n, "enumivo::string_to_name(ihgfedcba)" );
   enumivo_assert( enumivo::name("jihgfedcba") == "jihgfedcba"_n, "enumivo::string_to_name(jihgfedcba)" );
   enumivo_assert( enumivo::name("kjihgfedcba") == "kjihgfedcba"_n, "enumivo::string_to_name(kjihgfedcba)" );
   enumivo_assert( enumivo::name("lkjihgfedcba") == "lkjihgfedcba"_n, "enumivo::string_to_name(lkjihgfedcba)" );
   enumivo_assert( enumivo::name("mlkjihgfedcba") == "mlkjihgfedcba"_n, "enumivo::string_to_name(mlkjihgfedcba)" );
}
