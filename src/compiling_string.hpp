#ifndef COMPILING_STRING_HPP
#define COMPILING_STRING_HPP

#include <array>

/**
 * @brief to generate std::array during compiling
 * @see https://zhuanlan.zhihu.com/p/352914170
 */

template<class T, size_t N, class F, size_t... I>
[[nodiscard]] constexpr static std::array<std::remove_cv_t<T>, N>
generate_array_internal(F f, std::index_sequence<I...>) noexcept {
    return {f(I)...};
}

template<size_t N, class F, class T = std::invoke_result_t<F, size_t>>
[[nodiscard]] constexpr std::array<T, N>
generate_array(F f) noexcept {
    return generate_array_internal<T, N>(f, std::make_index_sequence<N>{});
}

/**
 * @brief get subarray during compiling
 * @see https://zhuanlan.zhihu.com/p/352914170
 */

// for perfect forwarding
template<size_t N, class T>
[[nodiscard]] constexpr auto
subarray(T &&t, size_t begin = 0) noexcept {
    return generate_array<N>([begin, t = std::forward<T>(t)](size_t i) { return t[begin + i]; });
}

// for initialize list
template<size_t N, class T, size_t M>
[[nodiscard]] constexpr auto
subarray(const T (&t)[M], size_t begin = 0) noexcept {
    return generate_array<N>([begin, &t](size_t i) { return t[begin + i]; });
}

/**
 * @brief concat arrays during compiling
 * @see https://zhuanlan.zhihu.com/p/352914170
 */

template<class T, class... Ts>
[[nodiscard]] static constexpr auto
total_length(const T &head, const Ts &...tail) noexcept {
    if constexpr (sizeof...(tail)) return std::size(head) + total_length(tail...);
    return std::size(head);
}

template<class T, class... Ts>
[[nodiscard]] static constexpr auto
get_from_array(size_t i, const T &head, const Ts &...tail) noexcept {
    if constexpr (sizeof...(tail))
        return i < std::size(head) ? head[i] : get_from_array(i - size(head), tail...);
    return head[i];
}

template<typename... T>
[[nodiscard]] constexpr auto
concat_arrays(T... arr) noexcept {
    return generate_array<total_length(arr...)>([&arr...](auto i) { return get_from_array(i, arr...); });
}

[[nodiscard]] static constexpr auto
get_char_from_string(size_t i) noexcept { return '\0'; }

template<class T, class... Ts>
[[nodiscard]] static constexpr auto
get_char_from_string(size_t i, const T &head, const Ts &...tail) {
    if (!std::size(head))
        return get_char_from_string(i, tail...);
    if (head[std::size(head) - 1])
        throw "string not end with \\0";
    if (i + 1 < std::size(head))
        if (!head[i])
            throw "\0 at middle of string";
        else
            return head[i];
    return get_char_from_string(i - std::size(head) + 1, tail...);
}

template<typename... T>
[[nodiscard]] constexpr auto
conact_strings(T... str) {
    return generate_array<total_length(str...) - sizeof...(T) + 1>([&str...](auto i) { return get_char_from_string(i, str...); });
}

#endif// COMPILING_STRING_HPP
