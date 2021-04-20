#include "fmt/chrono.h"
#include "fmt/core.h"

/**
 * @brief 日志核心功能
 *
 * 方案：将参数序列化到 `std::tuple`，使之可延迟格式化到 `std::string`
 *
 * 技术参考：
 * - 编译期收集 文件、行、函数 等元信息并构造输出格式：[quill](https://github.com/odygrd/quill)
 * - 利用 `std::tuple` 实现序列化：[c++11的 元组（tuple）有什么用？有什么使用场景？ - rayhunter的回答 - 知乎](https://www.zhihu.com/question/455522367/answer/1844213866)
 */

namespace mechdancer::logger {
    /** @brief 日志条目接口 */
    struct log_item_t {
        virtual std::string format() const = 0;
    };

    /**
     * @brief 特异日志条目类型
     *
     * @tparam _meta_t 元数据类型
     * @tparam _args_t 参数类型
     */
    template<class _meta_t, class... _args_t>
    class specific_item_t : public log_item_t {
        using clock = std::chrono::system_clock;
        using tuple_sequence = std::make_index_sequence<sizeof...(_args_t)>;

        std::chrono::system_clock::time_point _stamp;
        std::tuple<uint8_t, std::decay_t<_args_t>...> _parameters;

        template<size_t... _index>
        auto log(std::index_sequence<_index...>) const {
            using namespace std::chrono;

            constexpr static auto meta = _meta_t::meta();
            const auto tse = _stamp.time_since_epoch();
            const auto s = duration_cast<seconds>(tse);
            const auto ms = duration_cast<microseconds>(tse - s);

            return fmt::format(meta.data(), std::get<0>(_parameters), _stamp, ms.count(), std::get<_index + 1>(_parameters)...);
        }

    public:
        specific_item_t(uint8_t level, _args_t... args)
            : _stamp(clock::now()),
              _parameters(std::make_tuple(level, args...)) {}

        std::string format() const override { return log(tuple_sequence()); }
    };

    /**
     * @brief 构造日志条目
     *
     * @tparam _meta_t 元信息类型
     * @tparam _args_t 参数类型
     * @param level 日志级别
     * @param args 参数
     * @return 堆上日志条目的指针
     */
    template<class _meta_t, class... _args_t>
    inline auto make_specific_new_item(uint8_t level, _args_t... args) {
        return new specific_item_t<_meta_t, _args_t...>(level, args...);
    }
}// namespace mechdancer::logger
