#include <openssl/hmac.h>
#include <vector>
#include <time.h>
#include "utils.h"

#include <iostream>

namespace onetoken {
namespace utils {
void CharToHex(unsigned char c, unsigned char &hex1, unsigned char &hex2) {
  hex1 = c / 16;
  hex2 = c % 16;
  hex1 += hex1 <= 9 ? '0' : 'a' - 10;
  hex2 += hex2 <= 9 ? '0' : 'a' - 10;
}

std::string BinaryToHex(const std::string &input) {
  std::string result;
  for (size_t i = 0; i < input.size(); ++i) {
    unsigned char d1, d2;
    CharToHex(input.at(i), d1, d2);
    result += d1;
    result += d2;
  }

  return result;
}

std::string UrlEncode(std::string s) {
  const char *str = s.c_str();
  std::vector<char> v(s.size());
  v.clear();
  for (size_t i = 0, l = s.size(); i < l; i++) {
    char c = str[i];
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') || c == '-' || c == '_' || c == '.' ||
        c == '!' || c == '*' || c == '(' || c == ')') {
      v.push_back(c);
    } else if (c == ' ') {
      v.push_back('+');
    } else {
      v.push_back('%');
      unsigned char d1, d2;
      CharToHex(c, d1, d2);
      v.push_back(d1);
      v.push_back(d2);
    }
  }
  return std::string(v.cbegin(), v.cend());
}

std::string HmacSha256Encode(const std::string &key, const std::string &input) {
  const EVP_MD *engine = EVP_sha256();

  unsigned char output[EVP_MAX_MD_SIZE] = {0};

  HMAC_CTX ctx;
  HMAC_CTX_init(&ctx);
  HMAC_Init_ex(&ctx, key.c_str(), key.length(), engine, NULL);
  HMAC_Update(&ctx, (unsigned char *)input.c_str(), input.length());

  uint32_t output_length;
  HMAC_Final(&ctx, output, &output_length);
  HMAC_CTX_cleanup(&ctx);

  std::string result = (char *)output;
  result.resize(32);
  std::cout << result.length() << std::endl;

  return result;
}

std::string GenerateRandomId(const std::string &base) {
  std::string result = base;

  // base + timestamp + randstr
  time_t now = time(NULL);
  struct tm t;
#ifdef _WIN32
  localtime_s(&t, &now);
#else
  localtime_r(&now, &t);
#endif
  char buf[20];
  strftime(buf, sizeof(buf), "-%Y%m%d-%H%M%S", &t);
  result += buf;

  std::string randstr;
  for (size_t i = 0; i < 20; ++i) {
    auto r = rand() % 35;
    if (r < 26) {
      randstr += (char)('a' + r); 
    } else {
      randstr += std::to_string(r - 26);
    }
  }
  result += randstr;
  
  return result;
}

}  // namespace utils
}  // namespace onetoken