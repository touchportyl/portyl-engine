#include "pch.h"

#include "flexbase64.h"

#include "wincrypt.h" // CryptBinaryToStringA, CryptStringToBinaryA

namespace FlexEngine
{
  namespace Base64
  {

    __FLX_API std::string Encode(const std::vector<BYTE>& data)
    {
      // guard
      if (data.empty()) return "";

      DWORD data_size = static_cast<DWORD>(data.size());
      DWORD size = 0;

      DWORD flags = CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF;

      // get size
      if (!CryptBinaryToStringA(data.data(), data_size, flags, nullptr, &size))
      {
        Log::Error("Base64 encoding: Failed to get the size of the encoded data.");
        return "";
      }

      // allocate
      std::string result(size, '\0');

      // encode
      if (!CryptBinaryToStringA(data.data(), data_size, flags, &result[0], &size))
      {
        Log::Error("Base64 encoding: Failed to encode the data.");
        return "";
      }

      // Adjust the size of the string to remove the null terminator.
      // CryptBinaryToStringA includes the null terminator in the string.
      result.resize(size - 1);

      return result;
    }

    __FLX_API std::vector<BYTE> Decode(const std::string& data)
    {
      // guard
      if (data.empty()) return {};

      DWORD data_size = static_cast<DWORD>(data.size());
      DWORD size = 0;

      DWORD flags = CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF;

      // get size
      if (!CryptStringToBinaryA(data.c_str(), data_size, flags, nullptr, &size, nullptr, nullptr))
      {
        Log::Error("Base64 decoding: Failed to get the size of the decoded data.");
        return {};
      }

      // allocate
      std::vector<BYTE> result(size);

      // decode
      if (!CryptStringToBinaryA(data.c_str(), data_size, flags, result.data(), &size, nullptr, nullptr))
      {
        Log::Error("Base64 decoding: Failed to decode the data.");
        return {};
      }

      return result;
    }

  }
}