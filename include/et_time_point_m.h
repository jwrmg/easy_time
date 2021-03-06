#pragma once
#include <chrono>
#include <ostream>
#include <string>

#include "et_base.h"

namespace et
{
    using hrc = std::chrono::high_resolution_clock;
    using tp_hrc = std::chrono::time_point<hrc>;

    template<typename t>
    using tp = std::chrono::time_point<hrc, t>;

    using nanoseconds = std::chrono::nanoseconds;
    using microseconds = std::chrono::microseconds;
    using milliseconds = std::chrono::milliseconds;
    using seconds = std::chrono::seconds;
    using minutes = std::chrono::minutes;
    using hours = std::chrono::hours;

    struct time_point_m final : public base
    {
        tp_hrc now_point;
        time_point_m();
        time_point_m(tp_hrc point);
        time_point_m(long long nanoseconds);

        std::string to_string() const override;
        std::string flat_string() const;

        template<typename time_type>
        long long to() const;
        template<typename  time_type>
        tp<time_type> to_point() const;

        static time_point_m now();

        time_point_m operator+(const time_point_m& other) const;
        time_point_m operator+=(const time_point_m& other);
        time_point_m operator-(const time_point_m& other) const;
        time_point_m operator-=(const time_point_m& other);
        time_point_m operator*(const long long& other) const;
        time_point_m operator/(const long long& other) const;
        bool operator>(const time_point_m& other) const;
        bool operator>=(const time_point_m& other) const;
        bool operator<(const time_point_m& other) const;
        bool operator<=(const time_point_m& other) const;
        bool operator==(const time_point_m& other) const;

        // ostream operator overload
        friend std::ostream& operator<<(std::ostream& os, const et::time_point_m& point);
    };

    inline std::ostream& operator<<(std::ostream& os, const et::time_point_m& point)
    {
        return os << point.to_string();
    }

    /**
     * \brief Core Constructor. Every constructor delegates to this.
     * \param point Point in time. (std::chrono::time_point<hrc>)
     */
    inline time_point_m::time_point_m(tp_hrc point) { now_point = point; }

    /**
     * \brief Default Constructor. Uses the current time.
     */
    inline time_point_m::time_point_m() : time_point_m(hrc::now()) {}

    /**
     * \brief Converts MS to time_point_m.
     * \param milliseconds Time point in MS.
     */
    inline time_point_m::time_point_m(long long nanoseconds) : time_point_m(tp_hrc(et::nanoseconds(nanoseconds))) {}

    /**
     * \brief Converts the time_point_m to a readable string.
     * \return a string.
     */
    inline std::string time_point_m::to_string() const
    {
        std::string rv;
        rv += std::string("nano-seconds: ") += std::to_string(this->to<nanoseconds>()) += '\n';
        rv += std::string("micro-seconds: ") += std::to_string(this->to<microseconds>()) += '\n';
        rv += std::string("milli-seconds: ") += std::to_string(this->to<milliseconds>()) += '\n';
        rv += std::string("seconds: ") += std::to_string(this->to<seconds>()) += '\n';
        rv += std::string("minutes: ") += std::to_string(this->to<minutes>()) += '\n';
        rv += std::string("hours: ") += std::to_string(this->to<hours>()) += '\n';
        return rv;
    }

    inline std::string time_point_m::flat_string() const
    {
        std::string rv;
        rv += std::string("nano-seconds: ") += std::to_string(this->to<nanoseconds>()) += ' ';
        rv += std::string("micro-seconds: ") += std::to_string(this->to<microseconds>()) += ' ';
        rv += std::string("milli-seconds: ") += std::to_string(this->to<milliseconds>()) += ' ';
        rv += std::string("seconds: ") += std::to_string(this->to<seconds>()) += ' ';
        rv += std::string("minutes: ") += std::to_string(this->to<minutes>()) += ' ';
        rv += std::string("hours: ") += std::to_string(this->to<hours>()) += ' ';
        return rv;
    }

    inline time_point_m time_point_m::now()
    {
        // immediately get the time first.
        const auto& time_point = hrc::now();
        // then convert it.
        return { time_point };
    }

    inline time_point_m time_point_m::operator+(const time_point_m& other) const
    {
        const auto other_ms = std::chrono::duration_cast<et::nanoseconds>(other.now_point.time_since_epoch()).count();
        const auto this_ms = std::chrono::duration_cast<et::nanoseconds>(now_point.time_since_epoch()).count();
        return { this_ms + other_ms };
    }

    inline time_point_m time_point_m::operator+=(const time_point_m& other)
    {
        long long other_ns = std::chrono::duration_cast<et::nanoseconds>(other.now_point.time_since_epoch()).count();
        long long this_ns = std::chrono::duration_cast<et::nanoseconds>(now_point.time_since_epoch()).count();

        return *this = time_point_m((this_ns - other_ns) / static_cast<long long>(1e+6));
    }

    inline time_point_m time_point_m::operator-(const time_point_m& other) const
    {
        return std::chrono::duration_cast<et::nanoseconds>(this->now_point - other.now_point).count();
    }

    inline time_point_m time_point_m::operator-=(const time_point_m& other)
    {
        return *this = std::chrono::duration_cast<et::nanoseconds>(this->now_point - other.now_point).count();
    }

    inline time_point_m time_point_m::operator*(const long long& other) const
    {
        const auto this_ms = std::chrono::duration_cast<et::nanoseconds>(now_point.time_since_epoch()).count();
        return this_ms * other;
    }

    inline time_point_m time_point_m::operator/(const long long& other) const
    {
        const auto this_ms = std::chrono::duration_cast<et::nanoseconds>(now_point.time_since_epoch()).count();
        return this_ms / other;
    }

    inline bool time_point_m::operator>(const time_point_m& other) const
    {
        return now_point > other.now_point;
    }

    inline bool time_point_m::operator>=(const time_point_m& other) const
    {
        return now_point >= other.now_point;
    }

    inline bool time_point_m::operator<(const time_point_m& other) const
    {
        return now_point < other.now_point;
    }

    inline bool time_point_m::operator<=(const time_point_m& other) const
    {
        return now_point <= other.now_point;
    }

    inline bool time_point_m::operator==(const time_point_m& other) const
    {
        return now_point == other.now_point;
    }

    /**
     * \brief Converts the current time point to a given time type. And returns the count of that value.
     * \tparam time_type Accepted Types: et::nanoseconds, et::microseconds, et::milliseconds, et::seconds, et::minutes, et::hours.
     * \return Count of the time type.
     */
    template <typename time_type>
    long long time_point_m::to() const
    {
        return to_point<time_type>().time_since_epoch().count();
    }

    /**
    * \brief Converts the current time point to a given time type. And returns the count of that value.
    * \tparam time_type Accepted Types: et::nanoseconds, et::microseconds, et::milliseconds, et::seconds, et::minutes, et::hours.
    * \return Count of the time type.
    */
    template <typename time_type>
    tp<time_type> time_point_m::to_point() const
    {
        return std::chrono::time_point_cast<time_type>(now_point);
    }
}
