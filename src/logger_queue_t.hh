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
        class kernel_t;
        std::shared_ptr<kernel_t> _kernel;
        std::thread _worker;

    public:
        explicit logger_queue_t(uint8_t level = ACTIVE_LOG_LEVEL);
        ~logger_queue_t();

        void enqueue(log_item_t *);
        void flush();
    };

    logger_queue_t *get_logger(std::string = "default", uint8_t = 255);

}// namespace mechdancer::logger
