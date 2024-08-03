/**
 * @file base64.c
 * @brief Base64処理
 */

//------------------------------------------------------------------------------
// include
//------------------------------------------------------------------------------
#include "base64.h"

//------------------------------------------------------------------------------
// private variable
//------------------------------------------------------------------------------
//! Base64エンコードテーブル
static const uint8_t m_enc_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// clang-format off
//! @brief Base64デコードテーブル
//! @details '+'～'z'のASCIIデータ
static const int8_t m_dec_table[] = {
                                              62, -1, -1, -1, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,
  -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
  -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};
// clang-format on

//------------------------------------------------------------------------------
// private function
//------------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
 * @brief Base64エンコード
 *
 * @param [in] bin : バイナリーデータ
 * @param bin_size : バイナリーデータ長
 * @param [out] ascii : ASCIIデータ
 * @return ASCIIデータ長
 */
int32_t base64_encode(const uint8_t bin[], uint32_t bin_size, uint8_t ascii[])
{
  uint8_t  bit6;
  uint8_t  buf_size   = 0;
  uint16_t buf        = 0;
  uint32_t ascii_size = 0;

  for (uint32_t i = 0; i < bin_size; i++) {
    buf = (uint16_t)(buf << 8) | bin[i];
    buf_size += 8;

    // 6ビット毎に変換
    do {
      bit6 = (uint8_t)(buf >> (buf_size - 6)) & 0x3f;
      buf_size -= 6;
      ascii[ascii_size] = m_enc_table[bit6];
      ascii_size++;
    } while (buf_size >= 6);
  }

  // 端数ビットがある場合は0を補完して変換
  if (buf_size > 0) {
    bit6              = (uint8_t)(buf << (6 - buf_size)) & 0x3f;
    ascii[ascii_size] = m_enc_table[bit6];
    ascii_size++;
  }

  // バイト数が4の倍数でないなら'='を追加
  while ((ascii_size % 4) != 0) {
    ascii[ascii_size] = (uint8_t)'=';
    ascii_size++;
  }
  ascii[ascii_size] = (uint8_t)'\0';

  return (int32_t)ascii_size;
}

/** ----------------------------------------------------------------------------
 * @brief Base64デコード
 *
 * @param [in] ascii : ASCIIデータ
 * @param ascii_size : ASCIIデータ長
 * @param [out] bin : バイナリーデータ
 * @return バイナリーデータ長
 * @retval データ長 >= 0 : バイナリデータ長
 * @retval データ長 < 0 : ASCIIデータ異常
 */
int32_t base64_decode(const uint8_t ascii[], uint32_t ascii_size, uint8_t bin[])
{
  int8_t   i;
  int8_t   tmp;
  uint8_t  buf[4];
  uint32_t size     = 0;
  uint32_t bin_size = 0;

  if ((ascii_size % 4) != 0) {
    return -1;
  }

  do {
    // 4バイトのASCIIデータを3バイトのバイナリーデータに変換
    for (i = 0; i < 4; i++) {
      if ((ascii[size] < (uint8_t)'+') || (ascii[size] > (uint8_t)'z')) {
        return -1;
      }

      tmp = m_dec_table[ascii[size] - (uint8_t)'+'];
      if (tmp < 0) {
        return -1;
      }
      buf[i] = (uint8_t)tmp;
      size++;
    }

    bin[bin_size] = (uint8_t)((uint8_t)(buf[0] & 0x3f) << 2) | (uint8_t)((uint8_t)(buf[1] & 0x30) >> 4);
    bin_size++;
    bin[bin_size] = (uint8_t)((uint8_t)(buf[1] & 0x0f) << 4) | (uint8_t)((uint8_t)(buf[2] & 0x3c) >> 2);
    bin_size++;
    bin[bin_size] = (uint8_t)((uint8_t)(buf[2] & 0x03) << 6) | (uint8_t)((uint8_t)(buf[3] & 0x3f));
    bin_size++;
  } while (size < ascii_size);

  // '='分を削除
  if (ascii[ascii_size - 1] == (uint8_t)'=') {
    bin_size--;
  }
  if (ascii[ascii_size - 2] == (uint8_t)'=') {
    bin_size--;
  }

  return bin_size;
}