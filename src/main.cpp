#include "logger_queue_t.hh"

int main() {
    auto logger = mechdancer::logger::get_logger("default", 10);
    auto thread = logger->start_worker_thread();
    LOG(logger, 1, "begin");
    for (auto i = 0; i < 1000; ++i) {
        LOG(logger, 20, "Hello, world! {} {} {}", i, "-i", .5);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    LOG(logger, 1, "end");
    logger->stop();
    thread.join();
    return 0;
}
