#include "compiling_string.hpp"

#include "fmt/core.h"

#include <array>
#include <chrono>
#include <iostream>

// any value in macro to string

#define MACRO_STR_INTERNAL(VALUE) #VALUE
#define MACRO_STR(VALUE) MACRO_STR_INTERNAL(VALUE)

#ifndef ACTIVE_LOG_LEVEL
#define ACTIVE_LOG_LEVEL 0xff
#endif

template<class _meta_t, class... Args>
void log(uint8_t level, Args... args) {
    constexpr static auto meta = _meta_t::meta();
    fmt::print(meta.data(), level, args...);
}

// log

#define LOG(level, fmt, ...)                                      \
    if constexpr (level <= ACTIVE_LOG_LEVEL) {                    \
        constexpr auto fmt_ = std::to_array(fmt);                 \
        constexpr auto file = std::to_array(__FILE__);            \
        constexpr auto line = std::to_array(MACRO_STR(__LINE__)); \
        constexpr auto func = std::to_array(__FUNCTION__);        \
        struct specific_meta_t {                                  \
            constexpr static auto meta() noexcept {               \
                return conact_strings(                            \
                    std::to_array("LOG LEVEL {} | "),             \
                    fmt_, std::to_array(" | "),                   \
                    file, std::to_array("("),                     \
                    line, std::to_array("): "),                   \
                    func, std::to_array("\n"));                   \
            }                                                     \
        };                                                        \
        log<specific_meta_t>(level, ##__VA_ARGS__);               \
    } else {                                                      \
        /*nothing to do*/                                         \
    }

int main() {
    constexpr static auto array = std::to_array<uint8_t>({1, 2, 3, 4, 5});
    LOG(1, "{} {} {}", 1, 2.0, '3');
    for (auto i = 0; i < 3; ++i)
        LOG(2, "sedfgdfghdfehdfhdg");
    return 0;
}
