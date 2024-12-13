
#pragma once

#ifdef LUMEN_VISUAL_PROFILING_ENABLE

#include <cstddef>
#include <chrono>
#include <string>
#include <sstream>
#include <string_view> 
#include <fstream>
#include <mutex>
#include <thread>
#include <utility>

#if __cplusplus >= 202002L
        #include <source_location>
#endif // __cplusplus >= 202002L

namespace Lumen {

namespace VisualProfiling {

namespace Detail {

struct VisualProfilingResult {
        std::string_view profile_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time_point;
        std::thread::id thread_id;

        constexpr VisualProfilingResult(std::string_view profile_name) noexcept
        : profile_name{profile_name} {}
};

class VisualProfiler {
        const char *m_visual_profiling_result_file_name{"LumenVisualProfilingResult.json"};
        std::size_t m_profile_count{0};
        std::ofstream m_output_stream;
        std::mutex m_mutex;

        constexpr VisualProfiler(void) noexcept
        {
                this->OpenOutputStream();
                this->WriteProfileHeader();
        }

        constexpr void OpenOutputStream(void) noexcept
        {
                this->m_output_stream = std::ofstream{this->m_visual_profiling_result_file_name};
        }

        constexpr void WriteProfileHeader(void) noexcept
        {
                this->m_output_stream << "{\"otherData\":{},\"traceEvents\":[";
        }

        constexpr void WriteProfileFooter(void) noexcept
        {
                this->m_output_stream << "]}";
        }

public:
        static VisualProfiler &Instance(void) noexcept
        {
                static VisualProfiler visual_profiler{};
                return visual_profiler;
        }

        constexpr ~VisualProfiler(void) noexcept
        {
                this->WriteProfileFooter();
        }

        constexpr void WriteVisualProfileResult(const VisualProfilingResult &visual_profile_result) noexcept
        {
                std::stringstream string_stream{};

                if (this->m_profile_count > 0) {
                        string_stream << ",";
                }
                ++this->m_profile_count;

                const auto duration = (visual_profile_result.end_time_point - visual_profile_result.start_time_point);
                const auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(duration);
                const auto start_time_point_ms = std::chrono::time_point_cast<std::chrono::microseconds>(visual_profile_result.start_time_point);

                string_stream << "\n{"
                                 "\"cat\":\"function\","
                                 "\"dur\":" << duration_ms.count() << ","
                                 "\"name\":\"" << visual_profile_result.profile_name << "\","
                                 "\"ph\":\"X\","
                                 "\"pid\":0,"
                                 "\"tid\":" << visual_profile_result.thread_id << ","
                                 "\"ts\":" << start_time_point_ms.time_since_epoch().count()
                              << "}";

                std::lock_guard<std::mutex> lock_guard{this->m_mutex};
                this->m_output_stream << string_stream.str();
        }
};

class ProfileTimer {
private:
        VisualProfilingResult m_visual_profiling_result;
        bool m_stooped;

public:
        constexpr ProfileTimer(std::string_view profile_name) noexcept
        : m_visual_profiling_result{profile_name},
          m_stooped{false}
        {
                this->StartTimer();
        }

        constexpr ~ProfileTimer(void) noexcept
        {
                this->StopTimer();
        }

        constexpr void StartTimer(void) noexcept
        {
                this->m_visual_profiling_result.start_time_point = std::chrono::high_resolution_clock::now();
        }

        constexpr void StopTimer(void) noexcept
        {
                if (this->m_stooped) {
                        return;
                }
                this->m_visual_profiling_result.end_time_point = std::chrono::high_resolution_clock::now();
                this->m_visual_profiling_result.thread_id = std::this_thread::get_id();
                VisualProfiler::Instance().WriteVisualProfileResult(this->m_visual_profiling_result);
                this->m_stooped = true;
        }
};

} // namespace Detail
} // namespace VisualProfiling 
} // namespace Lumen

#define LUMEN_VISUAL_PROFILE_DETAIL_TIMER_IMPL(serial_number) lumen_visual_profile_timer_##serial_number

#define LUMEN_VISUAL_PROFILE_DETAIL_TIMER(serial_number) LUMEN_VISUAL_PROFILE_DETAIL_TIMER_IMPL(serial_number)

#define LUMEN_VISUAL_PROFILE_SCOPE(profile_name) \
        Lumen::VisualProfiling::Detail::ProfileTimer LUMEN_VISUAL_PROFILE_DETAIL_TIMER(__LINE__){profile_name}

#if __cplusplus < 202002L
        #define LUMEN_VISUAL_PROFILE_DETAIL_FUNCTION_NAME __FUNCTION__
#else // __cplusplus >= 202002L
        #define LUMEN_VISUAL_PROFILE_DETAIL_FUNCTION_NAME std::source_location::current().function_name()
#endif // __cplusplus >= 202002L

#define LUMEN_VISUAL_PROFILE_FUNCTION() \
        LUMEN_VISUAL_PROFILE_SCOPE(LUMEN_VISUAL_PROFILE_DETAIL_FUNCTION_NAME)

#else // !defined(LUMEN_VISUAL_PROFILING_ENABLE)

#define LUMEN_VISUAL_PROFILE_SCOPE(profile_name)
#define LUMEN_VISUAL_PROFILE_FUNCTION()

#endif // !defined(LUMEN_VISUAL_PROFILING_ENABLE)