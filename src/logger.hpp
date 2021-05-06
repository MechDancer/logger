#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include "fmt/chrono.h"
#include "fmt/core.h"

#include <memory>

/**
 * @brief 日志前端核心功能
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
        virtual uint8_t level() const = 0;
        virtual std::string format() const = 0;
        virtual void display() const = 0;
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

        constexpr static auto meta = _meta_t::meta();

        std::chrono::system_clock::time_point _stamp;
        std::tuple<uint8_t, std::decay_t<_args_t>...> _parameters;

        auto ms() const {
            using namespace std::chrono;
            const auto tse = _stamp.time_since_epoch();
            const auto s = duration_cast<seconds>(tse);
            return duration_cast<microseconds>(tse - s).count();
        }

        template<size_t... _index>
        auto format(std::index_sequence<_index...>) const {
            return fmt::format(meta.data(), std::get<0>(_parameters), _stamp, ms(), std::get<_index + 1>(_parameters)...);
        }

        template<size_t... _index>
        auto display(std::index_sequence<_index...>) const {
            fmt::print(meta.data(), std::get<0>(_parameters), _stamp, ms(), std::get<_index + 1>(_parameters)...);
        }

    public:
        template<class... __args_t>
        specific_item_t(uint8_t level, __args_t... args)
            : _stamp(clock::now()),
              _parameters(std::make_tuple(level, std::forward<__args_t>(args)...)) {}

        uint8_t level() const override { return std::get<0>(_parameters); }
        std::string format() const override { return format(tuple_sequence()); }
        void display() const override { display(tuple_sequence()); }
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
    inline auto make_specific_new_item(uint8_t level, _args_t &&...args) {
        return std::unique_ptr<log_item_t>(new specific_item_t<_meta_t, _args_t...>(level, std::forward<_args_t>(args)...));
    }
}// namespace mechdancer::logger

#endif// LOGGER_LOGGER_HPP
