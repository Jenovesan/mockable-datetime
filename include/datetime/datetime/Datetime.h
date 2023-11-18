#ifndef DATETIME_DATETIME_H
#define DATETIME_DATETIME_H

#include "datetime/date/Date.h"
#include "datetime/time/Time.h"

/**
 * 'Date' and a 'Time' with components: 'year', 'month', 'day', 'hour', 'minute',
 * 'second', 'millisecond', 'microsecond', and 'nanosecond'.
 */
class Datetime : public Date, public Time
{
public:
    /**
     * Creates a 'Datetime' whose date values match 'date'.
     * 'time' will be set to the default 'Time'.
     *
     * @param date 'Date' to set the date values of 'Datetime' to.
     */
    explicit Datetime(const Date& date) :
        Date(date.year, date.month, date.day),
        Time() {}

    /**
     * Creates a 'Datetime' whose date values match 'date' and whose time values match 'time'.
     *
     * @param date 'Date' to set the date values of 'Datetime' to.
     * @param time 'Time' to set the time values of 'Datetime' to.
     */
    explicit Datetime(const Date& date, const Time& time) :
        Date(date.year, date.month, date.day),
        Time(time.hour, time.minute, time.second, time.millisecond, time.microsecond,
             time.nanosecond, time.timezone) {}

    /**
     * Creates a 'Datetime' based on the given arguments.
     *
     * @param year year value that 'Datetime' will be set to. (default Date::EPOCH.year)
     * @param month month value that 'Datetime' will be set to. (default Date::EPOCH.month)
     * @param day day value that 'Datetime' will be set to. (default Date::EPOCH.day)
     * @param hour hour value that 'Datetime' will be set to. (default 0)
     * @param minute minute value that 'Datetime' will be set to. (default 0)
     * @param second second value that 'Datetime' will be set to. (default 0)
     * @param millisecond millisecond value that 'Datetime' will be set to. (default 0)
     * @param microsecond microsecond value that 'Datetime' will be set to. (default 0)
     * @param nanosecond nanosecond value that 'Datetime' will be set to. (default 0)
     * @param timezone timezone that 'Datetime' will be set to. (default Time::default_timezone)
     */
    explicit Datetime(uint16_t year = EPOCH.year, uint8_t month = EPOCH.month,
                      uint8_t day = EPOCH.day, uint8_t hour = 0, uint8_t minute = 0,
                      uint8_t second = 0, uint16_t millisecond = 0, uint16_t microsecond = 0,
                      uint16_t nanosecond = 0, Timezone timezone = Time::default_timezone)
        :
        Date(year, month, day),
        Time(hour, minute, second, millisecond, microsecond, nanosecond, timezone) {}

    /**
     * Creates a 'Datetime' whose components' values match the current date and time.
     *
     * @param timezone 'Timezone' to set the current date and time to.
     *
     * @return created 'Datetime'.
     */
    static Datetime now(uint8_t day_offset = 0, uint8_t hour_offset = 0, uint8_t minute_offset =0,
                        uint8_t second_offset = 0, uint16_t millisecond_offset = 0,
                        uint16_t microsecond_offset = 0, uint16_t nanosecond_offset = 0,
                        Timezone timezone = default_timezone);

    /**
     * Constructs a datetime object from a millisecond unix timestamp.
     *
     * @param timestamp millisecond unix timestamp.
     * @param to_timezone timezone that 'Datetime' will be set to.
     * @param from_timezone timezone of 'timestamp'.
     *
     * @return the datetime object converted from the 'timestamp'.
     */
    static Datetime from_ms(size_t timestamp,
                            Timezone to_timezone = default_timezone,
                            Timezone from_timezone = TZ::UTC);

    /**
     * Converts 'this' to UNIX ms timestamp.
     *
     * @param timezone the timezone to convert the ms timestamp to.
     *
     * @return 'this' as a UNIX ms timestamp.
     */
    size_t to_ms(std::optional<Timezone> timezone = {}) const;

    /**
     * Creates a 'Datetime' from a std::string.
     *
     * @tparam DateComponent1 should be of type 'Date::Component'.
     * @tparam DateComponent2 should be of type 'Date::Component'.
     * @tparam DateComponent3 should be of type 'Date::Component'.
     * @tparam TimeComponents should be of type 'Time::Component'.
     *
     * @param string string representation of the date and time.
     * @param date_component1 'Date' component that the first number in 'string' represents.
     * @param date_component2 'Date' component that the second number in 'string'
     * represents.
     * @param date_component3 'Date' component that the third number in 'string' represents.
     * @param time_components 'Time' components that correspond to each number in 'string'.
     */
    template<typename DateComponent1, typename DateComponent2, typename DateComponent3,
        typename... TimeComponents>
    explicit Datetime(std::string_view string, DateComponent1 date_component1,
                      DateComponent2 date_component2, DateComponent3 date_component3,
                      TimeComponents... time_components) :
    // First 10 characters in 'string' will be for the date, remaining will be for
    // the time.
        Date(string.substr(0, 10), date_component1, date_component2, date_component3),
        Time(string.substr(10, string.length()),
             std::forward<TimeComponents>(time_components)...) {}

    /**
     * Creates a new 'Date' whose components' values match the date values of this
     * 'Datetime'.
     *
     * @return newly created 'Date'.
     */
    Date date() const;

    /**
     * Creates a new 'Time' whose components' values match the time values of this
     * 'Datetime'.
     *
     * @return newly created 'Time'.
     */
    Time time() const;

    /**
     * Represents this 'Datetime' as a std::string.
     *
     * Represents this 'Datetime' as a std::string with format
     * %Y-%m-%d %-H:%M:%S.%ms.%f.%ns
     *
     * @example
     * Datetime datetime = Datetime(2000, 1, 2, 3, 4, 5, 6, 7, 8);
     * std::string datetime_string = date.to_string();
     * std::cout << datetime_string;
     *
     * // output: 2000-01-02 3:04:05.6.7.8
     *
     * @return resulting std::string.
     */
    std::string to_string(char separate_components = ' ', char separate_time = ':') const override;

    /**
     * Checks if 'this' is greater than 'other'.
     *
     * @param other 'Datetime' to compare to.
     *
     * @return 'true' if 'this' is greater than this 'other', 'false' otherwise.
     */
    bool operator>(Datetime other) const;

    /**
     * Checks if 'this' is greater than or equal to 'other'.
     *
     * @param other 'Datetime' to compare to.
     *
     * @return 'true' if 'this' is greater than or equal to this 'other', 'false' otherwise.
     */
    bool operator>=(Datetime other) const;

    /**
     * Checks if 'this' is less than 'other'.
     *
     * @param other 'Datetime' to compare to.
     *
     * @return 'true' if 'this' is less than this 'other', 'false' otherwise.
     */
    bool operator<(Datetime other) const;

    /**
     * Checks if 'this' is less than or equal to 'other'.
     *
     * @param other 'Datetime' to compare to.
     *
     * @return 'true' if 'this' is less than or equal to 'other', 'false' otherwise.
     */
    bool operator<=(Datetime other) const;

    /**
     * Checks if 'this' is equal to 'other'.
     *
     * @param other 'Datetime' to compare to.
     *
     * @return 'true' if 'this' is equal to 'other', 'false' otherwise.
     */
    bool operator==(Datetime other) const;

    /**
     * Checks if 'this' is not equal to 'other'.
     *
     * @param other 'Datetime' to compare to.
     *
     * @return 'true' if 'this' is not equal to 'other', 'false' otherwise.
     */
    bool operator!=(Datetime other) const;

    /**
     * Adds a day to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator++() override;

    /**
     * Subtracts a day from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator--() override;

    /**
     * Adds 'time' to this 'Datetime'.
     *
     * @param time the other 'Time' added to this 'Datetime'.
     *
     * @return reference to this modified 'Datetime'.
     */
    Datetime& operator+=(Time time);

    /**
     * Subtracts 'time' to this 'Datetime'.
     *
     * @param time the other 'Time' subtracted from this 'Datetime'.
     *
     * @return reference to this modified 'Datetime'.
     */
    Datetime& operator-=(Time time);

    /**
     * Adds 'time_delta' to 'datetime'.
     *
     * @param datetime the 'Datetime' to have 'time_delta' added to.
     * @param time_delta the 'TimeDelta' to add to 'datetime'.
     *
     * @return 'datetime' with 'time_delta' added.
     */
    friend Datetime operator+(Datetime datetime, TimeDelta time_delta);

    /**
     * Subtracts 'time_delta' from 'datetime'.
     *
     * @param datetime the 'Datetime' to have 'time_delta' subtracted from.
     * @param time_delta the 'TimeDelta' to subtract from 'datetime'.
     *
     * @return 'datetime' with 'time_delta' subtracted.
     */
    friend Datetime operator-(Datetime datetime, TimeDelta time_delta);

    /**
     * Adds 'time_delta' to 'this'.
     *
     * @param time_delta the 'TimeDelta' to add to 'this'.
     *
     * @return 'this' with 'time_delta' added.
     */
    Datetime& operator+=(TimeDelta time_delta);

    /**
     * Subtracts 'time_delta' from 'this'.
     *
     * @param time_delta the 'TimeDelta' to subtract from 'this'.
     *
     * @return 'this' with 'time_delta' subtracted.
     */
    Datetime& operator-=(TimeDelta time_delta);

    /**
     * Adds 'days' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Day& days) override;

    /**
     * Subtracts 'days' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Day& days) override;

    /**
     * Adds 'hours' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Hours& hours) override;

    /**
     * Subtracts 'hours' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Hours& hours) override;

    /**
     * Adds 'minutes' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Minutes& minutes) override;

    /**
     * Subtracts 'minutes' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Minutes& minutes) override;

    /**
     * Adds 'seconds' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Seconds& seconds) override;

    /**
     * Subtracts 'seconds' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Seconds& seconds) override;

    /**
     * Adds 'milliseconds' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Milliseconds& milliseconds) override;

    /**
     * Subtracts 'milliseconds' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Milliseconds& milliseconds) override;

    /**
     * Adds 'microseconds' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Microseconds& microseconds) override;

    /**
     * Subtracts 'microseconds' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Microseconds& microseconds) override;

    /**
     * Adds 'nanoseconds' to this 'Datetime'.
     *
     * @param days number of days to add to this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator+=(const Nanoseconds& nanoseconds) override;

    /**
     * Subtracts 'nanoseconds' from this 'Datetime'.
     *
     * @param days number of days to subtract from this 'Datetime'.
     *
     * @return A reference to this modified 'Datetime'.
     */
    Datetime& operator-=(const Nanoseconds& nanoseconds) override;

    /**
     * Adds 'hours' to 'datetime'.
     *
     * @param datetime the base 'Datetime' to add 'hours' to.
     * @param hours the number of hours to add.
     *
     * @return a new 'Datetime' with 'hours' added.
     */
    friend Datetime operator+(Datetime datetime, const Hours& hours);

    /**
     * Subtracts 'hours' from 'datetime'.
     *
     * @param datetime the base 'Datetime' to subtract 'hours' from.
     * @param hours the number of hours to subtract.
     *
     * @return a new 'Datetime' with 'hours' subtracted.
     */
    friend Datetime operator-(Datetime datetime, const Hours& hours);

    /**
     * Adds 'minutes' to 'datetime'.
     *
     * @param datetime the base 'Datetime' to add 'minutes' to.
     * @param minutes the number of minutes to add.
     *
     * @return a new 'Datetime' with 'minutes' added.
     */
    friend Datetime operator+(Datetime datetime, const Minutes& minutes);

    /**
     * Subtracts 'minutes' from 'datetime'.
     *
     * @param datetime the base 'Datetime' to subtract 'minutes' from.
     * @param minutes the number of minutes to subtract.
     *
     * @return a new 'Datetime' with 'minutes' subtracted.
     */
    friend Datetime operator-(Datetime datetime, const Minutes& minutes);

    /**
     * Adds 'seconds' to 'datetime'.
     *
     * @param datetime the base 'Datetime' to add 'seconds' to.
     * @param seconds the number of seconds to add.
     *
     * @return a new 'Datetime' with 'seconds' added.
     */
    friend Datetime operator+(Datetime datetime, const Seconds& seconds);

    /**
     * Subtracts 'seconds' from 'datetime'.
     *
     * @param datetime the base 'Datetime' to subtract 'seconds' from.
     * @param seconds the number of seconds to subtract.
     *
     * @return a new 'Datetime' with 'seconds' subtracted.
     */
    friend Datetime operator-(Datetime datetime, const Seconds& seconds);

    /**
      * Adds 'milliseconds' to 'datetime'.
      *
      * @param datetime the base 'Datetime' to add 'milliseconds' to.
      * @param milliseconds the number of milliseconds to add.
      *
      * @return a new 'Datetime' with 'milliseconds' added.
      */
    friend Datetime operator+(Datetime datetime, const Milliseconds& milliseconds);

    /**
     * Subtracts 'milliseconds' from 'datetime'.
     *
     * @param datetime the base 'Datetime' to subtract 'milliseconds' from.
     * @param milliseconds the number of milliseconds to subtract.
     *
     * @return a new 'Datetime' with 'milliseconds' subtracted.
     */
    friend Datetime operator-(Datetime datetime, const Milliseconds& milliseconds);

    /**
    * Adds 'microseconds' to 'datetime'.
    *
    * @param datetime the base 'Datetime' to add 'microseconds' to.
    * @param microseconds the number of microseconds to add.
    *
    * @return a new 'Datetime' with 'microseconds' added.
    */
    friend Datetime operator+(Datetime datetime, const Microseconds& microseconds);

    /**
     * Subtracts 'microseconds' from 'datetime'.
     *
     * @param datetime the base 'Datetime' to subtract 'microseconds' from.
     * @param microseconds the number of microseconds to subtract.
     *
     * @return a new 'Datetime' with 'microseconds' subtracted.
     */
    friend Datetime operator-(Datetime datetime, const Microseconds& microseconds);

    /**
     * Adds 'nanoseconds' to 'datetime'.
     *
     * @param datetime the base 'Datetime' to add 'nanoseconds' to.
     * @param nanoseconds the number of nanoseconds to add.
     *
     * @return a new 'Datetime' with 'nanoseconds' added.
     */
    friend Datetime operator+(Datetime datetime, const Nanoseconds& nanoseconds);

    /**
    * Subtracts 'nanoseconds' from 'datetime'.
    *
    * @param datetime the base 'Datetime' to subtract 'nanoseconds' from.
    * @param nanoseconds the number of nanoseconds to subtract.
    *
    * @return a new 'Datetime' with 'nanoseconds' subtracted.
    */
    friend Datetime operator-(Datetime datetime, const Nanoseconds& nanoseconds);

    /**
     * Adds this 'Datetime' and 'other' 'Datetime'.
     *
     * @param datetime the base 'Datetime' to add 'other' to.
     * @param other the other 'Datetime' added to this 'Datetime'.
     *
     * @return new 'Datetime' object of this 'Datetime' added with 'other' 'Datetime'.
     */
    friend Datetime operator+(Datetime datetime, Time other);

    /**
     * Subtracts 'other' from 'datetime'.
     *
     * @param datetime 'Datetime' 'other' is subtracting from.
     * @param other 'Datetime' subtracting from 'datetime'.
     *
     * @return 'TimeDelta' of the 'other' subtracted from 'datetime'.
     */
    friend TimeDelta operator-(Datetime datetime, Datetime other);

    /**
     * Subtracts this 'Datetime' and 'other' 'Datetime'.
     *
     * @param datetime the base 'Datetime' to subtract 'other' from.
     * @param other the other 'Datetime' subtracted to this 'Datetime'.
     *
     * @return new 'Datetime' object of this 'Datetime' subtracted with 'other' 'Datetime'.
     */
    friend Datetime operator-(Datetime datetime, Time other);

    /**
     * Outputs 'datetime' into 'os'.
     *
     * @param os std::ostream' to insert 'datetime' into.
     * @param datetime 'Datetime' to insert into 'os'.
     *
     * @return reference to 'os' after inserting 'date' into 'os'.
     */
    friend std::ostream& operator<<(std::ostream& os, const Datetime& datetime);

private:

    /**
     * Adds hours to this 'Datetime'.
     *
     * @param hours_to_add number of hours to add.
     *
     * @return the number of days changed by the hours added.
     */
    int64_t add_hours(int64_t hours_to_add) override;

    /**
     * Milliseconds in a day.
     */
    static const size_t MILLISECONDS_PER_DAY;

    /**
     * Milliseconds in a year that is not a leap year.
     */
    static const size_t MILLISECONDS_PER_NON_LEAP_YEAR;

    /**
     * Milliseconds in a year that is a leap year.
     */
    static const size_t MILLISECONDS_PER_LEAP_YEAR;

    // These methods are no longer intuitive, so hide from user.

    int total_minutes() const override;

    int total_seconds() const override;

    int64_t total_milliseconds() const override;

    int64_t total_microseconds() const override;

    int64_t total_nanoseconds() const override;
};

#endif //DATETIME_DATETIME_H
