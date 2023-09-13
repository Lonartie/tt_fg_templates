#pragma once

constexpr bool is_prime(int num) {
   if (num < 2) return false;
   if (num == 2) return true;
   for (int i = 2; i < num; ++i)
      if (num % i == 0)
         return false;
   return true;
}

constexpr int nth_prime(int n) {
   int num = 2;
   int count = 0;
   while (count < n) {
      num++;
      if (is_prime(num))
         ++count;
   }
   return num;
}

template <int N>
constexpr int prime = nth_prime(N);

template <int N>
constexpr std::array<int, N> primes() {
   std::array<int, N> arr;
   int n = 0;
   int size = 0;
   while (size < N) {
      if (is_prime(n)) {
         arr[size] = n;
         ++size;
      }
      ++n;
   }
   return arr;
}