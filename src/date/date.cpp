#include "datetime/date/date.h"
#include <chrono>
#include <utility>
#include "datetime/timedelta/timedelta.h"

std::optional<Date> Date::mock_date;

Date Date::today(int day_offset, Timezone timezone)
{
    Date ret;
    if (mock_date.has_value())
    {
        ret = mock_date.value();
    }
    else
    {
        // Get the current time
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        // Convert the system time to a std::tm struct
        std::tm *now_tm = std::localtime(&t);

        int year = now_tm->tm_year + 1900;
        int month = now_tm->tm_mon + 1;
        int day = now_tm->tm_mday;

        ret = Date(year, month, day);

        // Adjust date for non-local timezone
        if (timezone != TZ::LOCAL)
        {
            int hour = Time::now(0, 0, 0, 0, 0, 0, TZ::LOCAL).hour + TZ::LOCAL.get_utc_offset_diff(timezone);
            if (hour >= Time::HOURS_PER_DAY)
                ret.add_days(1);
            else if (hour < 0)
                ret.subtract_days(1);
        }
    }

    // Create date
    ret += Days(day_offset);

    // Check if date is valid
    ASSERT(ret.is_valid_date(),
           std::invalid_argument(fmt::format("{} is an invalid date", ret.to_string())));

    return ret;
}

Date& Date::operator+=(const Days& days)
{
    if (days.value < 0)
        subtract_days(-days.value);
    else
        add_days(days.value);
    return *this;
}

Date& Date::operator-=(const Days& days)
{
    if (days.value < 0)
        add_days(-days.value);
    else
        subtract_days(days.value);
    return *this;
}

Date& Date::operator++()
{
    add_days(1);
    return *this;
}

Date& Date::operator--()
{
    subtract_days(1);
    return *this;
}

bool Date::operator>(const Date& other) const
{
    return year > other.year
        || year == other.year && month > other.month
        || year == other.year && month == other.month && day > other.day;
}

bool Date::operator>=(const Date& other) const
{
    return year > other.year
        || year == other.year && month > other.month
        || year == other.year && month == other.month && day >= other.day;
}

bool Date::operator<(const Date& other) const
{
    return year < other.year
        || year == other.year && month < other.month
        || year == other.year && month == other.month && day < other.day;
}

bool Date::operator<=(const Date& other) const
{
    return year < other.year
        || year == other.year && month < other.month
        || year == other.year && month == other.month && day <= other.day;
}

bool Date::operator==(const Date& other) const
{
    return year == other.year
        && month == other.month
        && day == other.day;
}

bool Date::operator!=(const Date& other) const
{
    return year != other.year
        || month != other.month
        || day != other.day;
}

bool Date::is_leap_year(uint16_t year)
{
    if (year % 4 != 0)
        return false;
    else if (year % 100 != 0)
        return true;
    else if (year % 400 != 0)
        return false;
    return true;
}

size_t Date::max_days_in_month(uint8_t month_idx, std::optional<uint16_t> year)
{
    ASSERT(month_idx >= 1 && month_idx <= 12,
           std::runtime_error(fmt::format("'{}' is not a valid month", month_idx)));

    switch (month_idx) {
    case 2:
        return year.has_value() && is_leap_year(year.value()) ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        return 31;
    }
}

void Date::add_days(size_t days_to_add)
{
    while (days_to_add > 0)
    {
        size_t days_till_next_month = (max_days_in_month(month, year) - day) + 1;
        if (days_to_add < days_till_next_month)
        {
            day += static_cast<int>(days_to_add);
            return;
        }
        else
        {
            month++;
            if (month > MONTHS_PER_YEAR)
            {
                year++;
                month = 1;
            }
            day = 1;
            days_to_add -= days_till_next_month;
        }
    }

    ASSERT(is_valid_date(),
           std::runtime_error(fmt::format("'{}' is not a valid date", this->to_string())));
}

void Date::subtract_days(size_t days_to_subtract)
{
    while (days_to_subtract > 0)
    {
        size_t days_till_prev_month = day;
        if (days_to_subtract < days_till_prev_month)
        {
            day -= static_cast<int>(days_to_subtract);
            return;
        }
        else
        {
            month--;
            if (month < 1)
            {
                year--;
                month = 12;
            }
            day = static_cast<int>(max_days_in_month(month, year));
            days_to_subtract -= days_till_prev_month;
        }
    }

    ASSERT(is_valid_date(),
           std::runtime_error(fmt::format("'{}' is not a valid date", this->to_string())));
}

const int Date::MONTHS_PER_YEAR = 12;

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    return os
        << date.year
        << '-'
        << strh::align(std::to_string(date.month), strh::Alignment::LEFT, 2, '0')
        << '-'
        << strh::align(std::to_string(date.day), strh::Alignment::LEFT, 2, '0');
}

Date::DayOfWeek Date::day_of_week() const
{
    int _year = year;
    int _month = month;

    // Adjust month and year for Zeller's congruence formula
    if (_month < 3)
    {
        _month += 12;
        _year -= 1;
    }

    // Calculate anchor day (January 1st, 1900 was a Monday)
    int anchor_day = 1 + (365 + 365 + 365 + 366) % 7; // January 1st, 1900 was a Monday

    // Calculate day of the week using Zeller's congruence formula
    int h = (
                day + (13 * (_month + 1) / 5)
                + _year + (_year / 4)
                - (_year / 100)
                + (_year / 400)
                + anchor_day
            ) % 7;

    // Convert Sunday-Saturday numbering to Monday-Sunday numbering
    size_t day_of_week = (h + 6) % 7;

    switch (day_of_week) {
    case 0:
        return MONDAY;
    case 1:
        return TUESDAY;
    case 2:
        return WEDNESDAY;
    case 3:
        return THURSDAY;
    case 4:
        return FRIDAY;
    case 5:
        return SATURDAY;
    case 6:
        return SUNDAY;
    default:
        throw std::runtime_error(
            fmt::format("day_of_week '{}' is invalid. Must be between 0 and 6", day_of_week));
    }
}

bool Date::is_weekday() const
{
    return !is_weekend();
}

bool Date::is_weekend() const
{
    switch (day_of_week()) {
    case SATURDAY:
    case SUNDAY:
        return true;
    default:
        return false;
    }
}

std::string Date::to_string(char delim) const
{
    return std::to_string(year)
           + delim
           + strh::align(std::to_string(month), strh::Alignment::LEFT, 2, '0')
           + delim
           + strh::align(std::to_string(day), strh::Alignment::LEFT, 2, '0');
}

bool Date::is_valid_date() const
{
    return is_valid_year() && is_valid_month() && is_valid_day();
}

Date::Date(uint16_t year, uint8_t month, uint8_t day) :
    year(year),
    month(month),
    day(day)
{
    ASSERT(is_valid_date(),
           std::invalid_argument(fmt::format("'{}' is an invalid date", Date::to_string())));
}

bool Date::is_valid_year() const
{
    // Even though year would technically be valid, we don't expect to have a date passed
    // these values.
    return year <= 2100 && year >= EPOCH.year;
}

bool Date::is_valid_month() const
{
    return month <= 12 && month >= 1;
}

bool Date::is_valid_day() const
{
    return day <= max_days_in_month(month, year) && day >= 1;
}

Date Date::operator+(const Days& days) const
{
    Date date = (*this);
    date += days;
    return date;
}

Date Date::operator-(const Days &days) const
{
    Date date = (*this);
    date -= days;
    return date;
}

Date Date::tomorrow(Timezone timezone)
{
    return Date::today(1, timezone);
}

size_t Date::get_total_days() const
{
    size_t ret = 0;

    // Years.
    for (uint16_t y = EPOCH.year; y < year; ++y)
    {
        ret += (is_leap_year(y) ? DAYS_PER_LEAP_YEAR : DAYS_PER_NON_LEAP_YEAR);
    }

    // Months.
    for (uint8_t m = 1; m < month; ++m)
    {
        ret += max_days_in_month(m, year);
    }

    // Days.
    ret += day - 1;

    return ret;
}

TimeDelta operator-(Date date, Date other)
{
    return TimeDelta(static_cast<int64_t>(date.get_total_days() - other.get_total_days()));
}

const uint16_t Date::DAYS_PER_NON_LEAP_YEAR = 365;

const uint16_t Date::DAYS_PER_LEAP_YEAR = 366;

const Date Date::EPOCH = Date(1970, 1, 1);

std::vector<Date> Date::range(Date start, Date end, Days increment)
{
    std::vector<Date> ret;
    Date current_date = std::move(start);
    while (current_date <= end)
    {
        ret.push_back(current_date);
        current_date += increment;
    }
    return ret;
}
