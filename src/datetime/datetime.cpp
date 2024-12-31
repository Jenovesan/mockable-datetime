#include <utility>

#include "datetime/datetime/datetime.h"
#include "datetime/timedelta/timedelta.h"

Datetime Datetime::now(uint8_t  day_offset, uint8_t hour_offset, uint8_t minute_offset,
                       uint8_t second_offset, uint16_t millisecond_offset,
                       uint16_t microsecond_offset, uint16_t nanosecond_offset, Timezone timezone)
{
    return Datetime(Date::today(day_offset, timezone),
                    Time::now(hour_offset, minute_offset, second_offset, millisecond_offset,
                              microsecond_offset, nanosecond_offset, timezone));
}

int64_t Datetime::add_hours(int64_t hours_to_add)
{
    int64_t day_change = Time::add_hours(hours_to_add);

    if (day_change > 0)
        add_days(day_change);
    if (day_change < 0)
        subtract_days(-day_change);

    return day_change;
}

Date Datetime::date() const
{
    return Date(year, month, day);
}

Time Datetime::time() const
{
    return Time(hour, minute, second, millisecond, microsecond, nanosecond, timezone);
}


bool Datetime::operator>(Datetime other) const
{
    other.set_timezone(timezone);

    return Date::operator>(other.date())
        || Date::operator==(other.date()) && Time::operator>(other.time());
}

bool Datetime::operator>=(Datetime other) const
{
    other.set_timezone(timezone);

    return Date::operator>(other.date())
        || Date::operator==(other.date()) && Time::operator>=(other.time());
}

bool Datetime::operator<(Datetime other) const
{
    other.set_timezone(timezone);

    return Date::operator<(other.date())
        || Date::operator==(other.date()) && Time::operator<(other.time());
}

bool Datetime::operator<=(Datetime other) const
{
    other.set_timezone(timezone);

    return Date::operator<(other.date())
        || Date::operator==(other.date()) && Time::operator<=(other.time());
}

bool Datetime::operator==(Datetime other) const
{
    other.set_timezone(timezone);

    return Date::operator==(other.date()) && Time::operator==(other.time());
}

bool Datetime::operator!=(Datetime other) const
{
    other.set_timezone(timezone);

    return Date::operator!=(other.date()) || Time::operator!=(other.time());
}

Datetime &Datetime::operator+=(const Days& days)
{
    Date::operator+=(days);
    return *this;
}

Datetime &Datetime::operator-=(const Days& days)
{
    Date::operator-=(days);
    return *this;
}

Datetime &Datetime::operator+=(const Hours& hours)
{
    Time::operator+=(hours);
    return *this;
}

Datetime &Datetime::operator-=(const Hours& hours)
{
    Time::operator-=(hours);
    return *this;
}

Datetime &Datetime::operator+=(const Minutes& minutes)
{
    Time::operator+=(minutes);
    return *this;
}

Datetime &Datetime::operator-=(const Minutes& minutes)
{
    Time::operator-=(minutes);
    return *this;
}

Datetime &Datetime::operator+=(const Seconds& seconds)
{
    Time::operator+=(seconds);
    return *this;
}

Datetime &Datetime::operator-=(const Seconds& seconds)
{
    Time::operator-=(seconds);
    return *this;
}

Datetime &Datetime::operator+=(const Milliseconds& milliseconds)
{
    Time::operator+=(milliseconds);
    return *this;
}

Datetime &Datetime::operator-=(const Milliseconds& milliseconds)
{
    Time::operator-=(milliseconds);
    return *this;
}

Datetime &Datetime::operator+=(const Microseconds& microseconds)
{
    Time::operator+=(microseconds);
    return *this;
}

Datetime &Datetime::operator-=(const Microseconds& microseconds)
{
    Time::operator-=(microseconds);
    return *this;
}

Datetime &Datetime::operator+=(const Nanoseconds& nanoseconds)
{
    Time::operator+=(nanoseconds);
    return *this;
}

Datetime &Datetime::operator-=(const Nanoseconds& nanoseconds)
{
    Time::operator-=(nanoseconds);
    return *this;
}

Datetime &Datetime::operator++()
{
    Date::operator++();
    return *this;
}

Datetime &Datetime::operator--()
{
    Date::operator--();
    return *this;
}

std::string Datetime::to_string(TimeComponent include_to,
                                char delim_date,
                                char delim_date_and_time,
                                char delim_h_m_s,
                                char delim_ms_us_ns,
                                char delim_tz) const
{
    return
      Date::to_string(delim_date)
    + delim_date_and_time
    + Time::to_string(include_to, delim_h_m_s, delim_ms_us_ns, delim_tz);
}

std::ostream& operator<<(std::ostream& os, const Datetime& datetime)
{
    return os << datetime.to_string();
}

Datetime Datetime::from_ms(size_t timestamp, Timezone to_timezone, Timezone from_timezone)
{
    // Year.
    uint16_t year = 1970;  // Unix epoch year.
    while (timestamp >= MILLISECONDS_PER_NON_LEAP_YEAR)
    {
        size_t ms_in_year = is_leap_year(year)
            ? MILLISECONDS_PER_LEAP_YEAR
            : MILLISECONDS_PER_NON_LEAP_YEAR;

        if (timestamp < ms_in_year) break;  // Prevent overshooting the end of the year.

        timestamp -= ms_in_year;
        year++;
    }

    // Month.
    uint8_t month = 1;
    while (timestamp >= max_days_in_month(month, year) * MILLISECONDS_PER_DAY)
    {
        timestamp -= max_days_in_month(month, year) * MILLISECONDS_PER_DAY;
        month++;
    }

    // Days.
    // We'll later add 1 to 'day' because we cannot have day = 0. Minimum day is 1.
    uint8_t day = timestamp / MILLISECONDS_PER_DAY;
    timestamp %= MILLISECONDS_PER_DAY;

    // Hours.
    uint8_t hour = timestamp / MILLISECONDS_PER_HOUR;
    timestamp %= MILLISECONDS_PER_HOUR;

    // Minutes.
    uint8_t minute = timestamp / MILLISECONDS_PER_MINUTE;
    timestamp %= MILLISECONDS_PER_MINUTE;

    // Seconds.
    uint8_t second = timestamp / MILLISECONDS_PER_SECOND;
    timestamp %= MILLISECONDS_PER_SECOND;

    // Milliseconds.
    uint16_t millisecond = timestamp;


    Datetime ret =
        Datetime(year, month, day+1, hour, minute, second, millisecond, 0, 0, from_timezone);

    ret.set_timezone(to_timezone);

    return ret;
}

Datetime operator+(Datetime datetime, const Hours& hours)
{
    datetime += hours;
    return datetime;
}

Datetime operator-(Datetime datetime, const Hours& hours)
{
    datetime -= hours;
    return datetime;
}

Datetime operator+(Datetime datetime, const Minutes& minutes)
{
    datetime += minutes;
    return datetime;
}

Datetime operator-(Datetime datetime, const Minutes& minutes)
{
    datetime -= minutes;
    return datetime;
}

Datetime operator+(Datetime datetime, const Seconds& seconds)
{
    datetime += seconds;
    return datetime;
}

Datetime operator-(Datetime datetime, const Seconds& seconds)
{
    datetime -= seconds;
    return datetime;
}

Datetime operator+(Datetime datetime, const Milliseconds& milliseconds)
{
    datetime += milliseconds;
    return datetime;
}

Datetime operator-(Datetime datetime, const Milliseconds& milliseconds)
{
    datetime -= milliseconds;
    return datetime;
}

Datetime operator+(Datetime datetime, const Microseconds& microseconds)
{
    datetime += microseconds;
    return datetime;
}

Datetime operator-(Datetime datetime, const Microseconds& microseconds)
{
    datetime -= microseconds;
    return datetime;
}

Datetime operator+(Datetime datetime, const Nanoseconds& nanoseconds)
{
    datetime += nanoseconds;
    return datetime;
}

Datetime operator-(Datetime datetime, const Nanoseconds& nanoseconds)
{
    datetime -= nanoseconds;
    return datetime;
}

Datetime operator+(Datetime datetime, Time other)
{
    other.set_timezone(datetime.timezone);
    datetime += other;
    return datetime;
}

Datetime operator-(Datetime datetime, Time other)
{
    other.set_timezone(datetime.timezone);
    datetime -= other;
    return datetime;
}

Datetime& Datetime::operator+=(Time time)
{
    TimeDelta time_delta = this->time() + std::move(time);
    nanosecond = time_delta.nanosecond;
    microsecond = time_delta.microsecond;
    millisecond = time_delta.millisecond;
    second = time_delta.second;
    minute = time_delta.minute;
    hour = time_delta.hour;
    (*this) += Days(time_delta.days);
    return *this;
}

Datetime& Datetime::operator-=(Time time)
{
    TimeDelta time_delta = this->time() - std::move(time);
    nanosecond = time_delta.nanosecond;
    microsecond = time_delta.microsecond;
    millisecond = time_delta.millisecond;
    second = time_delta.second;
    minute = time_delta.minute;
    hour = time_delta.hour;
    (*this) -= Days(-time_delta.days);
    return *this;
}

size_t Datetime::to_ms(Timezone timezone) const
{
    // Create datetime copy to convert incase we need to change timezone.
    Datetime datetime = Datetime(*this);
    datetime.set_timezone(timezone);

    size_t ret = 0;

    // Years.
    for (uint16_t y = 1970; y < datetime.year; ++y)
    {
        ret += (is_leap_year(y) ? MILLISECONDS_PER_LEAP_YEAR : MILLISECONDS_PER_NON_LEAP_YEAR);
    }

    // Months.
    for (uint8_t m = 1; m < datetime.month; ++m)
    {
        ret += max_days_in_month(m, datetime.year) * MILLISECONDS_PER_DAY;
    }

    // Days.
    ret += (datetime.day - 1) * MILLISECONDS_PER_DAY;

    // Hours.
    ret += datetime.hour * MILLISECONDS_PER_HOUR;

    // Minutes.
    ret += datetime.minute * MILLISECONDS_PER_MINUTE;

    // Seconds.
    ret += datetime.second * MILLISECONDS_PER_SECOND;

    // Milliseconds.
    ret += datetime.millisecond;

    return ret;
}

const size_t Datetime::MILLISECONDS_PER_DAY = MILLISECONDS_PER_HOUR * 24;
const size_t Datetime::MILLISECONDS_PER_NON_LEAP_YEAR = MILLISECONDS_PER_DAY * 365;
const size_t Datetime::MILLISECONDS_PER_LEAP_YEAR = MILLISECONDS_PER_NON_LEAP_YEAR + MILLISECONDS_PER_DAY;

Datetime& Datetime::operator+=(TimeDelta time_delta)
{
    (*this) += Days(time_delta.days);
    (*this) += Time(time_delta, timezone);
    return *this;
}

Datetime& Datetime::operator-=(TimeDelta time_delta)
{
    (*this) -= Days(time_delta.days);
    (*this) -= Time(time_delta, timezone);
    return *this;
}

Datetime operator+(Datetime datetime, TimeDelta time_delta)
{
    datetime += std::move(time_delta);
    return datetime;
}

Datetime operator-(Datetime datetime, TimeDelta time_delta)
{
    datetime -= std::move(time_delta);
    return datetime;
}

TimeDelta operator-(Datetime datetime, Datetime other)
{
    TimeDelta date_delta = datetime.date() - other.date();
    TimeDelta time_delta = datetime.time() - other.time();
    return TimeDelta(date_delta.days, time_delta.hour, time_delta.minute, time_delta.second,
                     time_delta.millisecond, time_delta.microsecond, time_delta.nanosecond);
}

int Datetime::total_minutes() const
{
    return BasicTime::total_minutes();
}

int Datetime::total_seconds() const
{
    return BasicTime::total_seconds();
}

int64_t Datetime::total_milliseconds() const
{
    return BasicTime::total_milliseconds();
}

int64_t Datetime::total_microseconds() const
{
    return BasicTime::total_microseconds();
}

int64_t Datetime::total_nanoseconds() const
{
    return BasicTime::total_nanoseconds();
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Days increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Hours increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Minutes increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Seconds increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Milliseconds increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Microseconds increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

std::vector<Datetime> Datetime::range(Datetime start, Datetime end, Nanoseconds increment)
{
    return range(start, end,
                 [&increment](Datetime& datetime)
                 {
                     datetime += increment;
                 });
}

Datetime& Datetime::round(TimeComponent to)
{
    Time::round(to);
    return *this;
}

Datetime& Datetime::ceil(TimeComponent to)
{
    Time::ceil(to);
    return *this;
}

Datetime& Datetime::floor(TimeComponent to)
{
    Time::floor(to);
    return *this;
}

template<typename Func>
requires std::is_invocable_v<Func, Datetime&>
std::vector<Datetime> Datetime::range(Datetime& start, Datetime& end, Func increment)
{
    std::vector<Datetime> ret;
    Datetime current_datetime = std::move(start);
    while (current_datetime <= end)
    {
        ret.push_back(current_datetime);
        increment(current_datetime);
    }
    return ret;
}

