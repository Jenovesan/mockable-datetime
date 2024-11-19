#include "datetime/time/time.h"
#include "fmt/format.h"
#include <chrono>
#include "datetime/timedelta/timedelta.h"

Timezone Time::default_timezone = TZ::EST;

std::optional<Time> Time::mock_time;

Time Time::now(uint8_t hour_offset, uint8_t minute_offset, uint8_t second_offset,
               uint16_t millisecond_offset, uint16_t microsecond_offset,
               uint16_t nanosecond_offset, Timezone timezone)
{
    Time time;
    if (mock_time.has_value())
    {
        time = mock_time.value();
    }
    else
    {
        // Get the current local time
        std::time_t now = std::time(nullptr);
        std::tm local_tm = *std::localtime(&now);

        // Extract the different time components
        int hour = local_tm.tm_hour;
        int minute = local_tm.tm_min;
        int second = local_tm.tm_sec;

        auto currentTimePoint = std::chrono::high_resolution_clock::now() + std::chrono::hours(hour);

        // Convert the time point to nanoseconds
        auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTimePoint);

        // Extract the nanoseconds count
        auto nanosecondsCount = nanoseconds.time_since_epoch().count();

        // Calculate milliseconds, microseconds, and remaining nanoseconds
        int millisecond = static_cast<int>(nanosecondsCount / 1'000'000 % 1'000);
        int microsecond = static_cast<int>(nanosecondsCount / 1'000 % 1'000);
        int nanosecond = static_cast<int>(nanosecondsCount % 1'000);

        time = Time(
            hour + hour_offset,
            minute + minute_offset,
            second + second_offset,
            millisecond + millisecond_offset,
            microsecond + microsecond_offset,
            nanosecond + nanosecond_offset,
            TZ::LOCAL
        ); // Gets the time in local time, so Timezone must be TZ::LOCAL
    }

    time.set_timezone(timezone); // Set the local time to 'timezone'.


    return time;
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
    return os << time.to_string();
}

Time& Time::operator+=(const Hours& hours)
{
    add_hours(hours.value);
    return *this;
}

Time& Time::operator-=(const Hours& hours)
{
    add_hours(-hours.value);
    return *this;
}

Time& Time::operator+=(const Minutes& minutes)
{
    add_minutes(minutes.value);
    return *this;
}

Time& Time::operator-=(const Minutes& minutes)
{
    add_minutes(-minutes.value);
    return *this;
}

Time& Time::operator+=(const Seconds& seconds)
{
    add_seconds(seconds.value);
    return *this;
}

Time& Time::operator-=(const Seconds& seconds)
{
    add_seconds(-seconds.value);
    return *this;
}

Time& Time::operator+=(const Milliseconds& milliseconds)
{
    add_milliseconds(milliseconds.value);
    return *this;
}

Time& Time::operator-=(const Milliseconds& milliseconds)
{
    add_milliseconds(-milliseconds.value);
    return *this;
}

Time& Time::operator+=(const Microseconds& microseconds)
{
    add_microseconds(microseconds.value);
    return *this;
}

Time& Time::operator-=(const Microseconds& microseconds)
{
    add_microseconds(-microseconds.value);
    return *this;
}

Time& Time::operator+=(const Nanoseconds& nanoseconds)
{
    add_nanoseconds(nanoseconds.value);
    return *this;
}

Time& Time::operator-=(const Nanoseconds& nanoseconds)
{
    add_nanoseconds(-nanoseconds.value);
    return *this;
}

int64_t Time::add_hours(int64_t hours_to_add)
{
    int64_t hours = static_cast<int64_t>(hour) + hours_to_add;
    hour = static_cast<uint8_t>(hours % HOURS_PER_DAY);
    if (hours < 0)
    {
        hour += HOURS_PER_DAY;
        return (hours - 23) / HOURS_PER_DAY;
    }
    else
        return static_cast<int64_t>(hours) / HOURS_PER_DAY;
}

void Time::add_minutes(int64_t minutes_to_add)
{
    int64_t new_total_minutes = total_minutes() + minutes_to_add;
    int64_t new_total_hours = static_cast<int>(new_total_minutes / static_cast<int64_t>(MINUTES_PER_HOUR));
    int64_t hour_change = new_total_hours - hour;
    int new_minute = static_cast<int>(new_total_minutes % static_cast<int64_t>(MINUTES_PER_HOUR));
    if (new_minute < 0)
    {
        new_minute += MINUTES_PER_HOUR;
        hour_change--;
    }
    minute = new_minute;
    add_hours(hour_change);
}

void Time::add_seconds(int64_t seconds_to_add)
{
    int64_t new_total_seconds = total_seconds() + seconds_to_add;
    int64_t new_total_minutes = static_cast<int>(new_total_seconds / static_cast<int64_t>(SECONDS_PER_MINUTE));
    int64_t minute_change = new_total_minutes - total_minutes();
    int new_second = static_cast<int>(new_total_seconds % static_cast<int64_t>(SECONDS_PER_MINUTE));
    if (new_second < 0)
    {
        new_second += SECONDS_PER_MINUTE;
        minute_change--;
    }
    second = new_second;
    add_minutes(minute_change);
}

void Time::add_milliseconds(int64_t milliseconds_to_add)
{
    int64_t new_total_milliseconds = total_milliseconds() + milliseconds_to_add;
    int new_total_seconds = static_cast<int>(new_total_milliseconds / static_cast<int64_t>
    (MILLISECONDS_PER_SECOND));
    int second_change = new_total_seconds - total_seconds();
    int new_millisecond = static_cast<int>(new_total_milliseconds % static_cast<int64_t>
    (MILLISECONDS_PER_SECOND));
    if (new_millisecond < 0)
    {
        new_millisecond += MILLISECONDS_PER_SECOND;
        second_change--;
    }
    millisecond = new_millisecond;
    add_seconds(static_cast<int>(second_change));
}

void Time::add_microseconds(int64_t microseconds_to_add)
{
    int64_t new_total_microseconds = total_microseconds() + microseconds_to_add;
    int64_t new_total_milliseconds = new_total_microseconds / static_cast<int64_t>(MICROSECONDS_PER_MILLISECOND);
    int64_t millisecond_change = new_total_milliseconds - total_milliseconds();
    int new_microsecond = static_cast<int>(new_total_microseconds % static_cast<int64_t>
    (MICROSECONDS_PER_MILLISECOND));
    if (new_microsecond < 0)
    {
        new_microsecond += MICROSECONDS_PER_MILLISECOND;
        millisecond_change--;
    }
    microsecond = new_microsecond;
    add_milliseconds(static_cast<int>(millisecond_change));
}

void Time::add_nanoseconds(int64_t nanoseconds_to_add)
{
    int64_t new_total_nanoseconds = total_nanoseconds() + nanoseconds_to_add;
    int64_t new_total_microseconds = new_total_nanoseconds / static_cast<int64_t>(NANOSECONDS_PER_MICROSECOND);
    int64_t microsecond_change = new_total_microseconds - total_microseconds();
    int new_nanosecond = static_cast<int>(new_total_nanoseconds % static_cast<int64_t>
    (NANOSECONDS_PER_MICROSECOND));
    if (new_nanosecond < 0)
    {
        new_nanosecond += NANOSECONDS_PER_MICROSECOND;
        microsecond_change--;
    }
    nanosecond = new_nanosecond;
    add_microseconds(static_cast<int>(microsecond_change));
}

void Time::set_default_timezone(Timezone timezone)
{
    default_timezone = timezone;
}

void Time::set_timezone(Timezone new_timezone)
{
    (*this) += Hours(timezone.get_utc_offset_diff(new_timezone));
    timezone = new_timezone;
}

Time::Time(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond, uint16_t microsecond,
           uint16_t nanosecond, Timezone timezone) :
    BasicTime(hour, minute, second, millisecond, microsecond, nanosecond),
    timezone(timezone)
{
    ASSERT(is_valid_time(),
           std::invalid_argument(fmt::format("Time '{}' is invalid", Time::to_string())));
}

Time& Time::round(TimeComponent to)
{
    if (to == TimeComponent::NANOSECOND)
        return *this;

    if (nanosecond >= NANOSECONDS_PER_MICROSECOND / 2)
        add_microseconds(1);
    nanosecond = 0;

    if (to == TimeComponent::MICROSECOND)
        return *this;

    if (microsecond >= MICROSECONDS_PER_MILLISECOND / 2)
        add_milliseconds(1);
    microsecond = 0;

    if (to == TimeComponent::MILLISECOND)
        return *this;

    if (millisecond >= MILLISECONDS_PER_SECOND / 2)
        add_seconds(1);
    millisecond = 0;

    if (to == TimeComponent::SECOND)
        return *this;

    if (second >= SECONDS_PER_MINUTE / 2)
        add_minutes(1);
    second = 0;

    if (to == TimeComponent::MINUTE)
        return *this;

    if (minute >= MINUTES_PER_HOUR / 2)
        add_hours(1);
    minute = 0;

    return *this;
}

Time& Time::ceil(TimeComponent to)
{
    if (to == TimeComponent::NANOSECOND)
        return *this;

    if (nanosecond > 0)
        add_microseconds(1);
    nanosecond = 0;

    if (to == TimeComponent::MICROSECOND)
        return *this;

    if (microsecond > 0)
        add_milliseconds(1);
    microsecond = 0;

    if (to == TimeComponent::MILLISECOND)
        return *this;

    if (millisecond > 0)
        add_seconds(1);
    millisecond = 0;

    if (to == TimeComponent::SECOND)
        return *this;

    if (second > 0)
        add_minutes(1);
    second = 0;

    if (to == TimeComponent::MINUTE)
        return *this;

    if (minute > 0)
        add_hours(1);
    minute = 0;

    return *this;
}

Time& Time::floor(TimeComponent to)
{
    if (to == TimeComponent::NANOSECOND)
        return *this;
    nanosecond = 0;
    if (to == TimeComponent::MICROSECOND)
        return *this;
    microsecond = 0;
    if (to == TimeComponent::MILLISECOND)
        return *this;
    millisecond = 0;
    if (to == TimeComponent::SECOND)
        return *this;
    second = 0;
    if (to == TimeComponent::MINUTE)
        return *this;
    minute = 0;
    return *this;
}

bool Time::is_valid_time() const
{
    return is_valid_hour() && is_valid_minute() && is_valid_second() && is_valid_millisecond()
           && is_valid_microsecond() && is_valid_nanosecond();
}

bool Time::is_valid_hour() const
{
    return hour < HOURS_PER_DAY && hour >= 0;
}

bool Time::is_valid_minute() const
{
    return minute < MINUTES_PER_HOUR && minute >= 0;
}

bool Time::is_valid_second() const
{
    return second < SECONDS_PER_MINUTE && second >= 0;
}

bool Time::is_valid_millisecond() const
{
    return millisecond < MILLISECONDS_PER_SECOND && millisecond >= 0;
}

bool Time::is_valid_microsecond() const
{
    return microsecond < MICROSECONDS_PER_MILLISECOND && microsecond >= 0;
}

bool Time::is_valid_nanosecond() const
{
    return nanosecond < NANOSECONDS_PER_MICROSECOND && nanosecond >= 0;
}

Time operator+(Time time, const Hours& hours)
{
    time += hours;
    return time;
}

Time operator-(Time time, const Hours& hours)
{
    time -= hours;
    return time;
}

Time operator+(Time time, const Minutes& minutes)
{
    time += minutes;
    return time;
}

Time operator-(Time time, const Minutes& minutes)
{
    time -= minutes;
    return time;
}

Time operator+(Time time, const Seconds& seconds)
{
    time += seconds;
    return time;
}

Time operator-(Time time, const Seconds& seconds)
{
    time -= seconds;
    return time;
}

Time operator+(Time time, const Milliseconds& milliseconds)
{
    time += milliseconds;
    return time;
}

Time operator-(Time time, const Milliseconds& milliseconds)
{
    time -= milliseconds;
    return time;
}

Time operator+(Time time, const Microseconds& microseconds)
{
    time += microseconds;
    return time;
}

Time operator-(Time time, const Microseconds& microseconds)
{
    time -= microseconds;
    return time;
}

Time operator+(Time time, const Nanoseconds& nanoseconds)
{
    time += nanoseconds;
    return time;
}

Time operator-(Time time, const Nanoseconds& nanoseconds)
{
    time -= nanoseconds;
    return time;
}

TimeDelta operator+(Time time, Time other)
{
    other.set_timezone(time.timezone);

    time += Nanoseconds(other.nanosecond);
    time += Microseconds(other.microsecond);
    time += Milliseconds(other.millisecond);
    time += Seconds(other.second);
    time += Minutes(other.minute);
    int64_t day_change = time.add_hours(other.hour);

    return TimeDelta(day_change, time.hour, time.minute, time.second, time.millisecond,
                     time.microsecond, time.nanosecond);
}

TimeDelta operator-(Time time, Time other)
{
    other.set_timezone(time.timezone);

    int64_t day_change = time.add_hours(-other.hour);
    time -= Minutes(other.minute);
    time -= Seconds(other.second);
    time -= Milliseconds(other.millisecond);
    time -= Microseconds(other.microsecond);
    time -= Nanoseconds(other.nanosecond);

    return TimeDelta(day_change, time.hour, time.minute, time.second, time.millisecond,
                     time.microsecond, time.nanosecond);
}

bool Time::operator>(Time other) const
{
    other.set_timezone(timezone);
    return BasicTime::operator>(other);
}

bool Time::operator>=(Time other) const
{
    other.set_timezone(timezone);
    return BasicTime::operator>=(other);
}

bool Time::operator==(Time other) const
{
    other.set_timezone(timezone);
    return BasicTime::operator==(other);
}

bool Time::operator!=(Time other) const
{
    other.set_timezone(timezone);
    return BasicTime::operator!=(other);
}

bool Time::operator<=(Time other) const
{
    other.set_timezone(timezone);
    return BasicTime::operator<=(other);
}

bool Time::operator<(Time other) const
{
    other.set_timezone(timezone);
    return BasicTime::operator<(other);
}

Time::Time(TimeDelta& time_delta, Timezone timezone) :
    BasicTime(time_delta.hour, time_delta.minute, time_delta.second, time_delta.millisecond,
              time_delta.microsecond, time_delta.nanosecond), timezone(timezone) {}

std::vector<Time> Time::range(Time start, Time end, Hours increment)
{
    return range(start, end,
                 [&increment](Time& time)
                 {
                     time += increment;
                 });
}

std::vector<Time> Time::range(Time start, Time end, Minutes increment)
{
    return range(start, end,
                 [&increment](Time& time)
                 {
                     time += increment;
                 });
}

std::vector<Time> Time::range(Time start, Time end, Seconds increment)
{
    return range(start, end,
                 [&increment](Time& time)
                 {
                     time += increment;
                 });
}

std::vector<Time> Time::range(Time start, Time end, Milliseconds increment)
{
    return range(start, end,
                 [&increment](Time& time)
                 {
                     time += increment;
                 });
}

std::vector<Time> Time::range(Time start, Time end, Microseconds increment)
{
    return range(start, end,
                 [&increment](Time& time)
                 {
                     time += increment;
                 });
}

std::vector<Time> Time::range(Time start, Time end, Nanoseconds increment)
{
    return range(start, end,
                 [&increment](Time& time)
                 {
                     time += increment;
                 });
}

std::string Time::to_string(TimeComponent include_to,
                            char delim_h_m_s,
                            char delim_ms_us_na,
                            char delim_tz) const
{
    std::stringstream ss;
    ss << BasicTime::to_string(include_to, delim_h_m_s, delim_ms_us_na);

    if (include_to == TimeComponent::TIMEZONE)
        ss << delim_tz << timezone.to_string();

    return ss.str();
}

Time Time::max()
{
    return Time(23, 59, 59, 999, 999, 999);
}

template<typename Func>
requires std::is_invocable_v<Func, Time&>
std::vector<Time> Time::range(Time& start, Time& end, Func increment)
{
    std::vector<Time> ret;
    Time current_time = std::move(start);
    while (current_time <= end)
    {
        ret.push_back(current_time);
        increment(current_time);
    }
    return ret;
}
