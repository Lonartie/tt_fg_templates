#pragma once
#include <tuple>

template <uint64_t I, typename T, uint64_t L, typename ... Ts>
inline constexpr bool has_been_counted(std::tuple<Ts...> t) {
   if constexpr (I >= L) {
      return false;
   } else {
      using IthType = std::tuple_element_t<I, std::tuple<Ts...>>;
      if constexpr (std::is_same_v<T, IthType>) {
         return true;
      } else {
         return has_been_counted<I + 1, T, L, Ts...>(t);
      }
   }
}

template <uint64_t I, typename ... Ts>
inline constexpr uint64_t count_unique(std::tuple<Ts...> t) {
   if constexpr (I >= sizeof...(Ts)) {
      return 0;
   } else {
      using IthType = std::tuple_element_t<I, std::tuple<Ts...>>;
      if (has_been_counted<0, IthType, I>(t)) {
         return count_unique<I + 1>(t);
      } else {
         return 1 + count_unique<I + 1>(t);
      }
   }
}

template <typename ... Ts>
inline constexpr uint64_t unique_size_of() {
   return count_unique<0>(std::make_tuple(Ts()...));
}

template <template <typename ...> typename T, typename ... Ts>
inline constexpr std::tuple<Ts...> to_tuple(T<Ts...>);

template <typename F, typename ... R>
inline constexpr auto reverse_tuple(std::tuple<F, R...>) {
   if constexpr (sizeof...(R) == 0) {
      return std::tuple<F>();
   } else {
      return std::tuple_cat(reverse_tuple(std::tuple<R...>()), std::tuple<F>());
   }
}

auto reverse_tuple(std::tuple<>) -> std::tuple<>;

template <uint64_t From, uint64_t To, typename ... Ts>
inline constexpr auto tuple_range() {
   if constexpr (From > To) {
      return std::tuple<>();
   } else {
      return std::tuple_cat(std::tuple<typename std::tuple_element<From, std::tuple<Ts...>>::type>(), tuple_range<From + 1, To, Ts...>());
   }
}

template<typename ... Ts>
struct pack {
   static constexpr uint64_t size = sizeof...(Ts);
   static constexpr uint64_t unique_size = unique_size_of<Ts...>();

   template <typename ... Us>
   static auto from_tuple(std::tuple<Us...>) -> pack<Us...> {
      return {};
   }

   template <typename U>
   using from = decltype(from_tuple(to_tuple(std::declval<U>())));

   template <template <typename ...> typename U>
   using apply = U<Ts...>;

   using reverse = decltype(from_tuple(reverse_tuple(std::declval<std::tuple<Ts...>>())));

   template <typename U>
   static constexpr bool contains = has_been_counted<0, U, size>(std::make_tuple(Ts()...));

   template <uint64_t N>
   using nth = std::tuple_element_t<N, std::tuple<Ts...>>;

   template <uint64_t From, uint64_t To>
   using range = decltype(from_tuple(tuple_range<From, To, Ts...>()));

   template <typename ... Us>
   constexpr bool operator==(pack<Us...>) const {
      return false;
   }

   template <typename ... Us>
   constexpr bool operator!=(pack<Us...>) const {
      return true;
   }

   constexpr bool operator==(pack<Ts...>) const {
      return true;
   }

   constexpr bool operator!=(pack<Ts...>) const {
      return false;
   }
};