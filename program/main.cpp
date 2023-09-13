#include "impl.hpp"
#include "type_traits.hpp"
#include "to_string.hpp"
#include "primes.hpp"
#include "pack.hpp"
#include <iostream>

static constexpr auto N = sizeof(int);

int main() {
   // class type_is_smaller_than should provide a static value member that
   // is true if the size of type is smaller than N, false otherwise
   // type_is_smaller_than_v should be an alias for type_is_smaller_than::value
   static_assert(type_is_smaller_than<16, int>::value);
   static_assert(!type_is_smaller_than_v<3, float>);
   static_assert(!type_is_smaller_than_v<1, bool>);

   // stringify shall be an alias to a constexpr function that acts just like std::to_string but returns a std::array<char, N>
   // types that should be supported are bool, int64_t, uint64_t and float
   static_assert(cstring("1") == stringify<1ull>);
   static_assert(cstring("-1") == stringify<(-1ll)>);
   static_assert(cstring("12323") == stringify<12323ll>);
   static_assert(cstring("-12323") == stringify<(-12323ll)>);
   static_assert(cstring("false") == stringify<false>);
   static_assert(cstring("true") == stringify<true>);
   static_assert(cstring("23.2000007") == stringify<23.2f>);
   static_assert(cstring("-23.2000007") == stringify<(-23.2f)>);

   // prime shall be an alias to a constexpr function that returns the nth prime number
   static_assert(prime<0> == 2);
   static_assert(prime<1> == 3);
   static_assert(prime<2> == 5);
   static_assert(prime<3> == 7);
   static_assert(prime<4> == 11);
   static_assert(prime<5> == 13);
   static_assert(prime<6> == 17);

   // primes shall be a constexpr function that returns a std::array<int, N> containing the first N prime numbers
   static_assert(primes<2>() == std::array {2, 3});
   static_assert(primes<12>() == std::array {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37});

   // pack shall be a struct that represents a pack of types providing the following interface:
   static_assert(pack<int, float, double>::size == 3);
   static_assert(pack<int, int, double>::size == 3);
   static_assert(pack<int, int, double>::unique_size == 2);
   static_assert(pack<int, float>() == pack<int, float>());
   static_assert(pack<float, int>() != pack<int, float>());
   static_assert(pack<>::template from<std::tuple<int, float>>() == pack<int, float>());
   static_assert(pack<float, int>::reverse() == pack<int, float>());
   static_assert(pack<int, float>::template contains<int>);
   static_assert(!pack<int, float>::template contains<double>);
   static_assert(std::is_same_v<pack<int, float>::template apply<std::tuple>, std::tuple<int, float>>);
   static_assert(std::is_same_v<pack<int, double, float>::template nth<0>, int>);
   static_assert(std::is_same_v<pack<int, double, float>::template nth<1>, double>);
   static_assert(std::is_same_v<pack<int, double, float>::template nth<2>, float>);
   static_assert(pack<int, float, double>::template range<0, 1>() == pack<int, float>());
   static_assert(pack<int, float, double>::template range<1, 2>() == pack<float, double>());
   static_assert(pack<int, float, double>::template range<1, 1>() == pack<float>());
   static_assert(pack<int, float, double>::template range<2, 1>() == pack<>());
}
