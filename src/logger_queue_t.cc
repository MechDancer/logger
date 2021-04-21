#include "logger_queue_t.hh"

#include <condition_variable>
#include <unordered_map>
#include <vector>

static std::unordered_map<std::string, mechdancer::logger::logger_queue_t> queues;

namespace mechdancer::logger {
    class logger_queue_t::kernel_t {
    public:
        uint8_t _level;
        std::mutex _mutex;
        std::condition_variable _signal;
        std::vector<log_item_t *> _buffer[2];

        volatile bool running;

        explicit kernel_t(uint8_t level)
            : running(true), _level(level) {
            _buffer[0].reserve(1024);
            _buffer[1].reserve(1024);
        }
    };

    logger_queue_t::logger_queue_t(uint8_t level)
        : _kernel(new kernel_t(level)),
          _worker(std::thread([q = _kernel] {
              std::vector<log_item_t *> result;
              result.reserve(1024);
              while (q->running) {
                  std::unique_lock<std::mutex> lock(q->_mutex);
                  q->_signal.wait_for(lock, std::chrono::milliseconds(50));
                  if (q->_buffer[1].size() == 1024)
                      result.swap(q->_buffer[1]);
                  else if (!q->_buffer[0].empty())
                      result.swap(q->_buffer[0]);
                  else
                      continue;
                  lock.unlock();
                  for (const auto &item : result)
                      item->display();
                  result.clear();
              };
          })) {}

    logger_queue_t::~logger_queue_t() {
        _kernel->running = false;
        _worker.join();
    }

    void logger_queue_t::enqueue(log_item_t *item) {
        if (item->level() > _kernel->_level) return;

        std::unique_lock<std::mutex> lock(_kernel->_mutex);
        _kernel->_buffer[0].push_back(item);
        if (_kernel->_buffer[0].size() == 1024) {
            _kernel->_buffer[0].swap(_kernel->_buffer[1]);
            _kernel->_signal.notify_one();
        }
    }

    void logger_queue_t::flush() {
        std::unique_lock<std::mutex> lock(_kernel->_mutex);
        _kernel->_signal.notify_one();
    }

    logger_queue_t *get_logger(std::string name, uint8_t level) {
        auto [p, _] = queues.try_emplace(name, level);
        return &p->second;
    }

}// namespace mechdancer::logger
