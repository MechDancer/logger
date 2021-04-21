#include "compiling_string.hpp"
#include "logger.hpp"

#include <memory>
#include <thread>

#ifndef ACTIVE_LOG_LEVEL
#define ACTIVE_LOG_LEVEL 0xff
#endif

// log
namespace mechdancer::logger {
    class logger_queue_t {
        using log_item_ptr = mechdancer::logger::log_item_t *;

        class kernel_t;
        std::shared_ptr<kernel_t> _kernel;

    public:
        explicit logger_queue_t(uint8_t level = 255);
        ~logger_queue_t();

        std::thread start_worker_thread();
        void stop();
        void enqueue(log_item_ptr);
    };

    logger_queue_t *get_logger(std::string, uint8_t);

}// namespace mechdancer::logger

// any value in macro to string

#define MACRO_STR_INTERNAL(VALUE) #VALUE
#define MACRO_STR(VALUE) MACRO_STR_INTERNAL(VALUE)

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
                    std::to_array("LOG LEVEL {:<3} {:%Y-%m-%d %H:%M:%S}.{:<6} ("),         \
                    file, std::to_array("("),                                              \
                    line, std::to_array("): "),                                            \
                    func, std::to_array(") | "),                                           \
                    fmt_, std::to_array("\n"));                                            \
            }                                                                              \
        };                                                                                 \
        (queue)->enqueue(make_specific_new_item<specific_meta_t>((level), ##__VA_ARGS__)); \
    } else {                                                                               \
        /*nothing to do*/                                                                  \
    }
