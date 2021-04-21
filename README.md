# logger

使用一堆模板元实现的低延迟日志库。

示例：

```c++
#include "core.h"

int main() {
    auto logger = mechdancer::logger::get_logger("default", 10);
    LOG(logger, 0, "begin");
    LOG(logger, 1, "test");
    LOG(logger, 2, "test");
    LOG(logger, 3, "test");
    LOG(logger, 4, "test");
    LOG(logger, 5, "test");
    LOG(logger, 6, "test");
    LOG(logger, 7, "test");
    LOG(logger, 8, "test");
    LOG(logger, 9, "test");
    for (auto i = 0; i < 1000; ++i)
        LOG(logger, 20, "Hello, world! {} {} {}", i, "-i", .5);
    LOG(logger, 0, "end");
    return 0;
}
```
