#pragma once
#include <cstdint>
#include <array>
#include <string>

template <uint64_t N>
struct cstring {
   constexpr cstring() = default;

   constexpr cstring(const char (&str)[N]) {
      for (uint64_t i = 0; i < N; ++i)
         data[i] = str[i];
   }

   constexpr char operator[](uint64_t i) const {
      return data[i];
   }

   template <uint64_t N2>
   constexpr bool operator==(const cstring<N2>&) const {
      return false;
   }

   template <uint64_t N2>
   constexpr bool operator!=(const cstring<N2>&) const {
      return true;
   }

   constexpr bool operator==(const cstring<N>& other) const {
      for (uint64_t i = 0; i < N; ++i)
         if (data[i] != other.data[i])
            return false;
      return true;
   }

   constexpr bool operator!=(const cstring<N>& other) const {
      return !(*this == other);
   }

   constexpr std::string to_str() const {
      std::string str;
      for (uint64_t i = 0; i < N; ++i)
         str += data[i];
      return str;
   }

   constexpr const char* c_str() const {
      return data.data();
   }

   static constexpr auto size = N;
   std::array<char, N> data = {};
};

template<uint64_t N1, uint64_t N2>
inline constexpr bool are_equal(cstring<N1> str1, cstring<N2> str2) {
   if (N1 != N2) return false;
   for (uint64_t i = 0; i < N1; ++i)
      if (str1[i] != str2[i])
         return false;
   return true;
}

static_assert(cstring("1") == cstring("1"));
static_assert(cstring("true") == cstring("true"));
static_assert(cstring("2") != cstring("3"));