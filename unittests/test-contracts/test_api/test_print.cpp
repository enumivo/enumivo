/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#include <enulib/enu.hpp>

#include "test_api.hpp"

void test_print::test_prints_l() {
  char ab[] = { 'a', 'b' };
  const char test[] = "test";
  prints_l(ab, 2);
  prints_l(ab, 1);
  prints_l(ab, 0);
  prints_l(test, sizeof(test)-1);
}

void test_print::test_prints() {
   prints("ab");
   prints("c\0test_prints");
   prints("efg");
}

void test_print::test_printi() {
   printi(0);
   printi(556644);
   printi(-1);
}

void test_print::test_printui() {
   printui(0);
   printui(556644);
   printui((uint64_t)-1);
}

void test_print::test_printi128() {
  int128_t a(1);
  int128_t b(0);
  int128_t c(std::numeric_limits<int128_t>::lowest());
  int128_t d(-87654323456);
  printi128(&a);
  prints("\n");
  printi128(&b);
  prints("\n");
  printi128(&c);
  prints("\n");
  printi128(&d);
  prints("\n");
}

void test_print::test_printui128() {
  uint128_t a((uint128_t)-1);
  uint128_t b(0);
  uint128_t c(87654323456);
  printui128(&a);
  prints("\n");
  printui128(&b);
  prints("\n");
  printui128(&c);
  prints("\n");
}

void test_print::test_printn() {
   printn(enumivo::name{"1"}.value);
   printn(enumivo::name{"5"}.value);
   printn(enumivo::name{"a"}.value);
   printn(enumivo::name{"z"}.value);

   printn(enumivo::name{"abc"}.value);
   printn(enumivo::name{"123"}.value);

   printn(enumivo::name{"abc.123"}.value);
   printn(enumivo::name{"123.abc"}.value);

   printn(enumivo::name{"12345abcdefgj"}.value);
   printn(enumivo::name{"ijklmnopqrstj"}.value);
   printn(enumivo::name{"vwxyz.12345aj"}.value);

   printn(enumivo::name{"111111111111j"}.value);
   printn(enumivo::name{"555555555555j"}.value);
   printn(enumivo::name{"aaaaaaaaaaaaj"}.value);
   printn(enumivo::name{"zzzzzzzzzzzzj"}.value);
}


void test_print::test_printsf() {
   float x = 1.0f / 2.0f;
   float y = 5.0f * -0.75f;
   float z = 2e-6f / 3.0f;
   printsf(x);
   prints("\n");
   printsf(y);
   prints("\n");
   printsf(z);
   prints("\n");
}

void test_print::test_printdf() {
   double x = 1.0 / 2.0;
   double y = 5.0 * -0.75;
   double z = 2e-6 / 3.0;
   printdf(x);
   prints("\n");
   printdf(y);
   prints("\n");
   printdf(z);
   prints("\n");
}

void test_print::test_printqf() {
   long double x = 1.0l / 2.0l;
   long double y = 5.0l * -0.75l;
   long double z = 2e-6l / 3.0l;
   printqf(&x);
   prints("\n");
   printqf(&y);
   prints("\n");
   printqf(&z);
   prints("\n");
}

void test_print::test_print_simple() {
    const std::string cvalue = "cvalue";
    enumivo::print(cvalue);
    std::string value = "value";
    enumivo::print(std::move(value));
}
