#pragma once
#include <cstdint>

template <uint64_t N, typename T>
struct type_is_smaller_than {
    static constexpr bool value = sizeof(T) < N;
};

template <uint64_t N, typename T>
static inline constexpr bool type_is_smaller_than_v = type_is_smaller_than<N, T>::value;