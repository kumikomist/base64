#include "CppUTest/TestHarness.h"

#include <string.h>

#include "base64.h"

// clang-format off
TEST_GROUP(base64){
  TEST_SETUP(){
  }

  TEST_TEARDOWN(){
  }
};
// clang-format on

TEST(base64, Test_base64_encode)
{
  uint8_t bin[128];
  uint8_t ascii[128];
  int32_t size;

  strncpy((char *)bin, "ABCDE", 5);
  size = base64_encode(bin, 5, ascii);
  LONGS_EQUAL(size, 8);
  STRNCMP_EQUAL((char *)ascii, "QUJDREU=", size);
}

TEST(base64, Test_base64_decode)
{
  uint8_t bin[128];
  uint8_t ascii[128];
  int32_t size;

  // 成功パターン
  strncpy((char *)ascii, "QUJDREU=", 8);
  size = base64_decode(ascii, 8, bin);
  LONGS_EQUAL(size, 5);
  STRNCMP_EQUAL((char *)bin, "ABCDE", size);

  // 失敗パターン
  strncpy((char *)ascii, "QUJDRE-=", 8);
  size = base64_decode(ascii, 8, bin);
  LONGS_EQUAL(size, -1);
}
