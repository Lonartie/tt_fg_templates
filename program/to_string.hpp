#pragma once

#include <vector>
#include "impl.hpp"

constexpr uint64_t size(std::vector<char> v) {
   return v.size();
}

template<uint64_t N>
inline constexpr cstring<N + 1> to_str(std::vector<char> v) {
   cstring<N + 1> str;
   for (uint64_t i = 0; i < N; ++i)
      str.data[i] = v[i];
   str.data[N] = '\0';
   return str;
}

template<uint64_t V>
constexpr std::vector<char> vectorify_uint64_t() {
   if (V == 0) {
      std::vector<char> v{'0'};
      return v;
   }
   std::vector<char> v;
   int n = V;
   while (n > 0) {
      char c = static_cast<char>(n % 10) + '0';
      v.insert(v.begin(), c);
      n /= 10;
   }
   return v;
}

template<int64_t V>
constexpr std::vector<char> vectorify_int64_t() {
   if constexpr (V >= 0) return vectorify_uint64_t<V>();
   else {
      std::vector<char> v = vectorify_uint64_t<(-V)>();
      v.insert(v.begin(), '-');
      return v;
   }
}

template <float V>
constexpr uint64_t precision_of() {
   uint64_t precision = 0;
   float back = V - static_cast<uint64_t>(V);
   while ((back * 10) - static_cast<int64_t>(back * 10) > 1e-6) {
      ++precision;
      back *= 10;
   }
   return precision;
}

template <uint64_t a, uint64_t b>
constexpr uint64_t pow() {
   uint64_t result = 1;
   for (uint64_t i = 0; i < b; ++i)
      result *= a;
   return result;
}

template<float V>
constexpr std::vector<char> vectorify_float() {
   std::vector<char> v;

   if (V < 0) {
      v = vectorify_float<(-V)>();
      v.insert(v.begin(), '-');
      return v;
   }

   float back = V - static_cast<uint64_t>(V);
   v = vectorify_uint64_t<(uint64_t)V>();

   if (back < 1e-6) {
      return v;
   }

   v.push_back('.');

   constexpr auto power = pow<10, precision_of<V>()>();
   constexpr auto back_int = static_cast<uint64_t>((V - static_cast<uint64_t>(V)) * power);
   std::vector<char> back_int_v = vectorify_uint64_t<back_int>();

   v.insert(v.end(), back_int_v.begin(), back_int_v.end());

   return v;
}

template<bool V>
constexpr std::vector<char> vectorify_bool() {
   if (V)
      return std::vector<char>{'t', 'r', 'u', 'e'};
   else
      return std::vector<char>{'f', 'a', 'l', 's', 'e'};
}

template<auto V>
static inline constexpr auto stringify = []() {
   if constexpr (std::is_same_v<decltype(V), int64_t>)
      return to_str<size(vectorify_int64_t<V>())>(vectorify_int64_t<V>());
   else if constexpr (std::is_same_v<decltype(V), uint64_t>)
      return to_str<size(vectorify_uint64_t<V>())>(vectorify_uint64_t<V>());
   else if constexpr (std::is_same_v<decltype(V), float>)
      return to_str<size(vectorify_float<V>())>(vectorify_float<V>());
   else if constexpr (std::is_same_v<decltype(V), bool>)
      return to_str<size(vectorify_bool<V>())>(vectorify_bool<V>());
   else
      throw std::exception("incompatible type");
}();

static_assert(cstring("1") == stringify<1ll>);
