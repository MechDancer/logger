#ifndef LOGGER_CORE_H
#define LOGGER_CORE_H

#include "logger_queue_t.hh"

// any value in macro to string

#define MACRO_STR_INTERNAL(VALUE) #VALUE
#define MACRO_STR(VALUE) MACRO_STR_INTERNAL(VALUE)

namespace {
    template<auto level>
    struct log_item_model_t {
        constexpr static auto prefix = std::to_array("\033[1;30m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<0> {
        constexpr static auto prefix = std::to_array("\033[1;31m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<1> {
        constexpr static auto prefix = std::to_array("\033[1;33m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<2> {
        constexpr static auto prefix = std::to_array("\033[1;35m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<3> {
        constexpr static auto prefix = std::to_array("\033[1;34m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<4> {
        constexpr static auto prefix = std::to_array("\033[1;36m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<5> {
        constexpr static auto prefix = std::to_array("\033[1;32m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<6> {
        constexpr static auto prefix = std::to_array("\033[1;37m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<7> {
        constexpr static auto prefix = std::to_array("\033[0;37m");
        constexpr static auto suffix = std::to_array("\033[0m\n");
    };

    template<>
    struct log_item_model_t<8> {
        constexpr static auto prefix = std::to_array("");
        constexpr static auto suffix = std::to_array("\n");
    };
}// namespace

#define LOG(queue, level, fmt, ...)                                                        \
    if constexpr (level <= ACTIVE_LOG_LEVEL) {                                             \
        using namespace mechdancer::logger;                                                \
        constexpr auto fmt_ = std::to_array(fmt);                                          \
        constexpr auto file = std::to_array(__FILE__);                                     \
        constexpr auto line = std::to_array(MACRO_STR(__LINE__));                          \
        constexpr auto func = std::to_array(__FUNCTION__);                                 \
        struct specific_meta_t {                                                           \
            constexpr static auto meta() noexcept {                                        \
                return conact_strings(                                                     \
                    log_item_model_t<level>::prefix,                                       \
                    std::to_array("LOG LEVEL {:<3} {:%Y-%m-%d %H:%M:%S}.{:>6} ("),         \
                    file, std::to_array("("),                                              \
                    line, std::to_array("): "),                                            \
                    func, std::to_array(") | "),                                           \
                    fmt_,                                                                  \
                    log_item_model_t<level>::suffix);                                      \
            }                                                                              \
        };                                                                                 \
        (queue)->enqueue(make_specific_new_item<specific_meta_t>((level), ##__VA_ARGS__)); \
    } else {                                                                               \
        /*nothing to do*/                                                                  \
    }

#endif// LOGGER_CORE_H
