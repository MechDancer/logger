#ifndef LOGGER_COMPILING_STRING_HPP
#define LOGGER_COMPILING_STRING_HPP

/**
 * 编译期的字符串，支持拼接、截取
 * @see https://zhuanlan.zhihu.com/p/352914170
 */

#include <array>
#include <cstddef>

// 编译期的字符串利用 `std::array<char>` 来存储
// 下面的函数用于根据特定函数生成 `std::array<char>`

template<class T, size_t N, class F, size_t... I>
[[nodiscard("consteval")]] constexpr static std::array<std::remove_cv_t<T>, N>
generate_array_internal(F f, std::index_sequence<I...>) noexcept {
    return {f(I)...};
}

template<size_t N, class F, class T = std::invoke_result_t<F, size_t>>
[[nodiscard("consteval")]] constexpr std::array<T, N>
generate_array(F f) noexcept {
    return generate_array_internal<T, N>(f, std::make_index_sequence<N>{});
}

// 下面的函数用于截取数组

// 完美转发版
template<size_t N, class T>
[[nodiscard("consteval"), maybe_unused]] constexpr auto
subarray(T &&t, size_t begin = 0) noexcept {
    return generate_array<N>([begin, t = std::forward<T>(t)](size_t i) { return t[begin + i]; });
}

// 初始化列表版（初始化列表不支持完美转发）
template<size_t N, class T, size_t M>
[[nodiscard("consteval"), maybe_unused]] constexpr auto
subarray(const T (&t)[M], size_t begin = 0) noexcept {
    return generate_array<N>([begin, &t](size_t i) { return t[begin + i]; });
}

template<class T, class... Ts>
[[nodiscard("consteval")]] static constexpr auto
total_length(const T &head, const Ts &...tail) noexcept {
    if constexpr (sizeof...(tail)) return std::size(head) + total_length(tail...);
    return std::size(head);
}

// 下面的函数用于拼接数组

template<class T, class... Ts>
[[nodiscard("consteval")]] static constexpr auto
get_from_array(size_t i, const T &head, const Ts &...tail) noexcept {
    if constexpr (sizeof...(tail))
        return i < std::size(head) ? head[i] : get_from_array(i - size(head), tail...);
    return head[i];
}

template<typename... T>
[[nodiscard("consteval")]] constexpr auto
concat_arrays(T... arr) noexcept {
    return generate_array<total_length(arr...)>([&arr...](auto i) { return get_from_array(i, arr...); });
}

[[nodiscard("consteval")]] static constexpr auto
get_char_from_string(size_t i) noexcept { return '\0'; }

template<class T, class... Ts>
[[nodiscard("consteval")]] static constexpr auto
get_char_from_string(size_t i, const T &head, const Ts &...tail) {
    if (!std::size(head))
        return get_char_from_string(i, tail...);
    if (head[std::size(head) - 1])
        throw "string not end with \\0";
    if (i + 1 < std::size(head))
        if (!head[i])
            throw "\\0 at middle of string";
        else
            return head[i];
    return get_char_from_string(i - std::size(head) + 1, tail...);
}

template<typename... T>
[[nodiscard("consteval")]] constexpr auto
conact_strings(T... str) {
    return generate_array<total_length(str...) - sizeof...(T) + 1>([&str...](auto i) { return get_char_from_string(i, str...); });
}

#endif// LOGGER_COMPILING_STRING_HPP
