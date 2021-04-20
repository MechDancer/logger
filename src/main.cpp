#include "compiling_string.hpp"
#include "logger.hpp"

#include <deque>
#include <iostream>

// any value in macro to string

#define MACRO_STR_INTERNAL(VALUE) #VALUE
#define MACRO_STR(VALUE) MACRO_STR_INTERNAL(VALUE)

#ifndef ACTIVE_LOG_LEVEL
#define ACTIVE_LOG_LEVEL 0xff
#endif

// log

using namespace mechdancer::logger;

std::deque<log_item_t *> _helpers;

#define LOG(level, fmt, ...)                                                               \
    if constexpr (level <= ACTIVE_LOG_LEVEL) {                                             \
        constexpr auto fmt_ = std::to_array(fmt);                                          \
        constexpr auto file = std::to_array(__FILE__);                                     \
        constexpr auto line = std::to_array(MACRO_STR(__LINE__));                          \
        constexpr auto func = std::to_array(__FUNCTION__);                                 \
        struct specific_meta_t {                                                           \
            constexpr static auto meta() noexcept {                                        \
                return conact_strings(                                                     \
                    std::to_array("LOG LEVEL {:<3} {:%Y-%m-%d %H:%M:%S}.{:<6} ("),         \
                    file, std::to_array("("),                                              \
                    line, std::to_array("): "),                                            \
                    func, std::to_array(") | "),                                           \
                    fmt_, std::to_array("\n"));                                            \
            }                                                                              \
        };                                                                                 \
        _helpers.push_back(make_specific_new_item<specific_meta_t>(level, ##__VA_ARGS__)); \
    } else {                                                                               \
        /*nothing to do*/                                                                  \
    }

int main() {
    for (auto i = 0; i < 1'000'000; ++i)
        LOG(1, "Hello, world! {}", i);
    std::cout << (*_helpers.begin())->format();
    std::cout << (*_helpers.rbegin())->format();
    return 0;
}
