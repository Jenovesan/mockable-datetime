#ifndef TIME_TIME_H
#define TIME_TIME_H

#include <boost/functional/hash.hpp>
#include <fmt/ranges.h>
#include "datetime/time/components/hours.h"
#include "datetime/time/components/minutes.h"
#include "datetime/time/components/seconds.h"
#include "datetime/time/components/microseconds.h"
#include "datetime/time/components/nanoseconds.h"
#include "datetime/time/timezone.h"
#include "datetime/time/components/milliseconds.h"
#include "stringhelpers/stringhelpers.h"
#include "../src/util/macros.h"
#include "../../../src/time/basic_time.h"
#include "datetime/timedelta/timedelta.h"
#include <optional>


/**
 * Time with components: 'hour', 'minute', 'second', 'millisecond', 'microsecond',
 * and 'nanosecond'.
 */
class Time : public BasicTime
{
public:

    static std::optional<Time> mock_time;

    /**
     * 'Timezone' of the 'Time'.
     */
    Timezone timezone = default_timezone;

    /**
     * Default 'Timezone' of all 'Time' objects.
     *
     * All newly created 'Time' objects' 'timezone' will be set to this value.
     */
    static Timezone default_timezone;

    /**
     * Creates a 'Time' whose components match the values of 'hour', 'minute',
     * 'second', 'millisecond', 'microsecond', 'nanosecond', and 'timezone'.
     *
     * @param hour hour the 'Time' will be set to. (default 0)
     * @param minute minute the 'Time' will be set to. (default 0)
     * @param second second the 'Time' will be set to. (default 0)
     * @param millisecond millisecond the 'Time' will be set to. (default 0)
     * @param microsecond microsecond the 'Time' will be set to. (default 0)
     * @param nanosecond nanosecond the 'Time' will be set to. (default 0)
     * @param timezone timezone the 'Time' will be set to. (default default_timezone)
     */
    explicit Time(uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0, uint16_t millisecond = 0,
                  uint16_t microsecond = 0, uint16_t nanosecond = 0, Timezone timezone = default_timezone);

    /**
     * Creates a 'Time' from a std::string.
     *
     * @tparam TimeComponents should be of type 'Component'.
     *
     * @param string string representation of the date.
     * @param time_components 'Components' that correspond to each number in 'string'.
     */
    template<typename... Component>
    explicit Time(std::string_view string, Component... time_components);

    /**
     * Creates a 'Time' object from a 'TimeDelta'.
     *
     * @param time_delta 'TimeDelta' whose time components will used for 'Times' components.
     * @param timezone timezone the 'Time' will be set to. (default default_timezone)
     */
    Time(TimeDelta& time_delta, Timezone timezone = default_timezone);

    /**
     * Creates a 'Time' whose components' values match the current time.
     *
     * @param timezone 'Timezone' to set the current time to.
     *
     * @return created 'Time'.
     */
    static Time now(uint8_t hour_offset = 0, uint8_t minute_offset = 0, uint8_t second_offset = 0,
                    uint16_t millisecond_offset = 0, uint16_t microsecond_offset = 0, uint16_t
                    nanosecond_offset = 0, Timezone timezone = default_timezone);

    /**
     * Creates a 'Time' object with the max values for 'hour', 'minute', 'second', 'millisecond',
     * 'microsecond', 'nanosecond'.
     *
     * @return 'Time' object with the max components.
     */
    static Time max();

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment hours to increment between each time in the range.
     *
     * @return range of time within 'start' to 'end'.
     */
    static std::vector<Time> range(Time start, Time end, Hours increment = Hours(1));

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment minutes to increment between each time in the range.
     *
     * @return range of time within 'start' to 'end'.
     */
    static std::vector<Time> range(Time start, Time end, Minutes increment = Minutes(1));

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment seconds to increment between each time in the range.
     *
     * @return range of time within 'start' to 'end'.
     */
    static std::vector<Time> range(Time start, Time end, Seconds increment = Seconds(1));

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment milliseconds to increment between each time in the range.
     *
     * @return range of time within 'start' to 'end'.
     */
    static std::vector<Time> range(Time start, Time end, Milliseconds increment = Milliseconds(1));

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment microseconds to increment between each time in the range.
     *
     * @return range of time within 'start' to 'end'.
     */
    static std::vector<Time> range(Time start, Time end, Microseconds increment = Microseconds(1));

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment nanoseconds to increment between each time in the range.
     *
     * @return range of time within 'start' to 'end'.
     */
    static std::vector<Time> range(Time start, Time end, Nanoseconds increment = Nanoseconds(1));

    /**
     * Represents this 'Datetime' as a std::string.
     *
     * @param include_to Include the time components up until 'include_to' in the resulting
     * string (inclusive). (default TimeComponent::TIMEZONE)
     * @param delim_h_m_s 'char' separating hours, minutes, and seconds.
     * @param delim_ms_us_ns 'char' separating milliseconds, microseconds, and nanoseconds.
     * @param delim_tz delimiter between nanosecond and timezone.
     *
     * @example
     * Time time = Time(1, 2, 3, 4, 5, 6, TZ::EST);
     * std::string time_string = time.to_string();
     * std::cout << time_string;
     *
     * // output: 1:02:03.4.5.6+5:00
     *
     * @return resulting std::string.
     */
    virtual std::string to_string(TimeComponent include_to = TimeComponent::TIMEZONE,
                                  char delim_h_m_s = ':',
                                  char delim_ms_us_na = '.',
                                  char delim_tz = '+') const;

    /**
     * Adds 'time' and 'other'.
     *
     * @param time 'Time' to add.
     * @param other other 'Time' to add.
     *
     * @return 'TimeDelta' of 'time' and 'other' added. Accounts for day changes.
     */
    friend TimeDelta operator+(Time time, Time other);

    /**
     * Subtracts 'other' from 'time'.
     *
     * @param time 'Time' that 'other' subtracts from.
     * @param other 'Time' to subtract from 'time'.
     *
     * @return 'TimeDelta' of 'other' subtracted from 'this'. Accounts for day changes.
     */
    friend TimeDelta operator-(Time time, Time other);

    /**
     * Adds 'hours' to this 'Time'.
     *
     * @param days number of days to add to this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator+=(const Hours& hours);

    /**
     * Subtracts 'hours' from this 'Time'.
     *
     * @param days number of days to subtract from this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator-=(const Hours& hours);

    /**
     * Adds 'minutes' to this 'Time'.
     *
     * @param days number of days to add to this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator+=(const Minutes& minutes);

    /**
     * Subtracts 'minutes' from this 'Time'.
     *
     * @param days number of days to subtract from this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator-=(const Minutes& minutes);

    /**
     * Adds 'seconds' to this 'Time'.
     *
     * @param days number of days to add to this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator+=(const Seconds& seconds);

    /**
     * Subtracts 'seconds' from this 'Time'.
     *
     * @param days number of days to subtract from this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator-=(const Seconds& seconds);

    /**
     * Adds 'milliseconds' to this 'Time'.
     *
     * @param days number of days to add to this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator+=(const Milliseconds& milliseconds);

    /**
     * Subtracts 'milliseconds' from this 'Time'.
     *
     * @param days number of days to subtract from this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator-=(const Milliseconds& milliseconds);

    /**
      * Adds 'microseconds' to this 'Time'.
      *
      * @param days number of days to add to this 'Time'.
      *
      * @return reference to this modified 'Time'.
      */
    virtual Time& operator+=(const Microseconds& microseconds);

    /**
     * Subtracts 'microseconds' from this 'Time'.
     *
     * @param days number of days to subtract from this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator-=(const Microseconds& microseconds);

    /**
     * Adds 'nanoseconds' to this 'Time'.
     *
     * @param days number of days to add to this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator+=(const Nanoseconds& nanoseconds);

    /**
     * Subtracts 'nanoseconds' from this 'Time'.
     *
     * @param days number of days to subtract from this 'Time'.
     *
     * @return reference to this modified 'Time'.
     */
    virtual Time& operator-=(const Nanoseconds& nanoseconds);

    /**
     * Checks if 'this' is greater than 'other'.
     *
     * @param other 'Time' to compare to.
     *
     * @return 'true' if 'this' is greater than 'other', 'false' otherwise.
     */
    bool operator>(Time other) const;

    /**
     * Checks if 'this' is greater than or equal to 'other'.
     *
     * @param other 'Time' to compare to.
     *
     * @return 'true' if 'this' is greater than or equal to 'other', 'false' otherwise.
     */
    bool operator>=(Time other) const;

    /**
     * Checks if 'this' is less than 'other'.
     *
     * @param other 'Time' to compare to.
     *
     * @return 'true' if 'this' is less than 'other', 'false' otherwise.
     */
    bool operator<(Time other) const;

    /**
     * Checks if 'this' is less than or equal to 'other'.
     *
     * @param other 'Time' to compare to.
     *
     * @return 'true' if 'this' is less than or equal to 'other', 'false' otherwise.
     */
    bool operator<=(Time other) const;

    /**
     * Checks if 'this' is equal to 'other'.
     *
     * @param other 'Time' to compare to.
     *
     * @return 'true' if 'this' is equal to 'other', 'false' otherwise.
     */
    bool operator==(Time other) const;

    /**
     * Checks if 'this' is not equal to 'other'.
     *
     * @param other 'Time' to compare to.
     *
     * @return 'true' if 'this' is not equal to 'other', 'false' otherwise.
     */
    bool operator!=(Time other) const;

    /**
     * Adds 'hours' to 'time'.
     *
     * @param time the base 'Time' to add 'hours' to.
     * @param hours the number of hours to add.
     *
     * @return a new 'Time' with 'hours' added.
     */
    friend Time operator+(Time time, const Hours& hours);

    /**
     * Subtracts 'hours' from 'time'.
     *
     * @param time the base 'Time' to subtract 'hours' from.
     * @param hours the number of hours to subtract.
     *
     * @return a new 'Time' with 'hours' subtracted.
     */
    friend Time operator-(Time time, const Hours& hours);

    /**
     * Adds 'minutes' to 'time'.
     *
     * @param time the base 'Time' to add 'minutes' to.
     * @param minutes the number of minutes to add.
     *
     * @return a new 'Time' with 'minutes' added.
     */
    friend Time operator+(Time time, const Minutes& minutes);

    /**
     * Subtracts 'minutes' from 'time'.
     *
     * @param time the base 'Time' to subtract 'minutes' from.
     * @param minutes the number of minutes to subtract.
     *
     * @return a new 'Time' with 'minutes' subtracted.
     */
    friend Time operator-(Time time, const Minutes& minutes);

    /**
     * Adds 'seconds' to 'time'.
     *
     * @param time the base 'Time' to add 'seconds' to.
     * @param seconds the number of seconds to add.
     *
     * @return a new 'Time' with 'seconds' added.
     */
    friend Time operator+(Time time, const Seconds& seconds);

    /**
     * Subtracts 'seconds' from 'time'.
     *
     * @param time the base 'Time' to subtract 'seconds' from.
     * @param seconds the number of seconds to subtract.
     *
     * @return a new 'Time' with 'seconds' subtracted.
     */
    friend Time operator-(Time time, const Seconds& seconds);

    /**
     * Adds 'milliseconds' to 'time'.
     *
     * @param time the base 'Time' to add 'milliseconds' to.
     * @param milliseconds the number of milliseconds to add.
     *
     * @return a new 'Time' with 'milliseconds' added.
     */
    friend Time operator+(Time time, const Milliseconds& milliseconds);

    /**
     * Subtracts 'milliseconds' from 'time'.
     *
     * @param time the base 'Time' to subtract 'milliseconds' from.
     * @param milliseconds the number of milliseconds to subtract.
     *
     * @return a new 'Time' with 'milliseconds' subtracted.
     */
    friend Time operator-(Time time, const Milliseconds& milliseconds);

    /**
    * Adds 'microseconds' to 'time'.
    *
    * @param time the base 'Time' to add 'microseconds' to.
    * @param microseconds the number of microseconds to add.
    *
    * @return a new 'Time' with 'microseconds' added.
    */
    friend Time operator+(Time time, const Microseconds& microseconds);

    /**
     * Subtracts 'microseconds' from 'time'.
     *
     * @param time the base 'Time' to subtract 'microseconds' from.
     * @param microseconds the number of microseconds to subtract.
     *
     * @return a new 'Time' with 'microseconds' subtracted.
     */
    friend Time operator-(Time time, const Microseconds& microseconds);

    /**
     * Adds 'nanoseconds' to 'time'.
     *
     * @param time the base 'Time' to add 'nanoseconds' to.
     * @param nanoseconds the number of nanoseconds to add.
     *
     * @return a new 'Time' with 'nanoseconds' added.
     */
    friend Time operator+(Time time, const Nanoseconds& nanoseconds);

    /**
     * Subtracts 'nanoseconds' from 'time'.
     *
     * @param time the base 'Time' to subtract 'nanoseconds' from.
     * @param nanoseconds the number of nanoseconds to subtract.
     *
     * @return a new 'Time' with 'nanoseconds' subtracted.
     */
    friend Time operator-(Time time, const Nanoseconds& nanoseconds);

    /**
     * Sets the 'timezone' of this 'Time'.
     *
     * @param new_timezone 'Timezone' to set 'timezone' to.
     */
    void set_timezone(Timezone new_timezone);

    /**
     * Sets 'default_timezone' of the 'Time' class.
     *
     * @param timezone 'Timezone' to set 'default_timezone' to.
     */
    static void set_default_timezone(Timezone timezone);

    /**
     * Rounds the components of this 'Time', stopping at 'to'.
     *
     * @param to finish the rounding of this 'Time's' components at this 'Component'.
     *
     * @example
     * Time time = Time(1, 2, 33, 4, 5, 6);
     * time.round(Time::MINUTE);
     * std::cout << time;
     *
     * // output: 1:03:00.0.0.0
     */
    virtual Time& round(TimeComponent to);

    /**
     * Rounds up the components of this 'Time', stopping at 'to'.
     *
     * @param to finish the rounding up of this 'Time's' components at this 'Component'.
     */
    virtual Time& ceil(TimeComponent to);

    /**
     * Rounds down the components of this 'Time', stopping at 'to'.
     *
     * @param to finish the rounding down of this 'Time's' components at this 'Component'.
     */
    virtual Time& floor(TimeComponent to);

    /**
     * Outputs 'time' into 'os'.
     *
     * @param os std::ostream' to insert 'time' into.
     * @param time 'Time' to insert into 'os'.
     *
     * @return reference to 'os' after inserting 'time' into 'os'.
     */
    friend std::ostream& operator<<(std::ostream& os, const Time& time);

protected:

    /**
     * Adds hours to this 'Time'.
     *
     * @param hours_to_add number of hours to add.
     *
     * @return the number of days changed by the hours added.
     */
    virtual int64_t add_hours(int64_t hours_to_add);

    /**
     * Adds minutes to this 'Time'.
     *
     * @param minutes_to_add number of minutes to add.
     */
    virtual void add_minutes(int64_t minutes_to_add);

    /**
     * Adds seconds to this 'Time'.
     *
     * @param seconds_to_add number of seconds to add.
     */
    virtual void add_seconds(int64_t seconds_to_add);

    /**
      * Adds milliseconds to this 'Time'.
      *
      * @param milliseconds_to_add number of milliseconds to add.
      */
    virtual void add_milliseconds(int64_t milliseconds_to_add);

    /**
      * Adds microseconds to this 'Time'.
      *
      * @param microseconds_to_add number of microseconds to add.
      */
    virtual void add_microseconds(int64_t microseconds_to_add);

    /**
      * Adds nanoseconds to this 'Time'.
      *
      * @param nanoseconds_to_add number of nanoseconds to add.
      */
    virtual void add_nanoseconds(int64_t nanoseconds_to_add);

private:

    /**
     * Gets the times within a range.
     *
     * @param start starting time of the range.
     * @param end ending time of the range.
     * @param increment function which increments the current time during the loop getting each
     * consecutive time between 'start' and 'end'.
     *
     * @return range of times within 'start' to 'end'.
     */
    template<typename Func>
    requires std::is_invocable_v<Func, Time&>
    static std::vector<Time> range(Time& start, Time& end, Func increment);

    /**
     * Checks if this 'Time's' components are all valid.
     *
     * @return 'true' if all this 'Time's' components are all valid, 'false' otherwise.
     */
    bool is_valid_time() const;

    /**
     * Checks if 'hour' is valid.
     *
     * Checks if 'hour' is less than 60 and greater than or equal to 0.
     *
     * @return 'true' is 'month' is valid, 'false' otherwise.
     */
    bool is_valid_hour() const;

    /**
     * Checks if 'minute' is valid.
     *
     * Checks if 'minute' is less than 60 and greater than or equal to 0.
     *
     * @return 'true' is 'month' is valid, 'false' otherwise.
     */
    bool is_valid_minute() const;


    /**
     * Checks if 'second' is valid.
     *
     * Checks if 'second' is less than 60 and greater than or equal to 0.
     *
     * @return 'true' is 'month' is valid, 'false' otherwise.
     */
    bool is_valid_second() const;

    /**
     * Checks if 'millisecond' is valid.
     *
     * Checks if 'millisecond' is less than 1000 and greater than or equal to 0.
     *
     * @return 'true' is 'month' is valid, 'false' otherwise.
     */
    bool is_valid_millisecond() const;

    /**
     * Checks if 'microsecond' is valid.
     *
     * Checks if 'microsecond' is less than 1000 and greater than or equal to 0.
     *
     * @return 'true' is 'month' is valid, 'false' otherwise.
     */
    bool is_valid_microsecond() const;

    /**
     * Checks if 'hour' is valid.
     *
     * Checks if 'nanosecond' is less than 1000 and greater than or equal to 0.
     *
     * @return 'true' is 'nanosecond' is valid, 'false' otherwise.
     */
    bool is_valid_nanosecond() const;
};

template<typename... Component>
Time::Time(std::string_view string, Component... time_components)
{
    std::vector<std::string> time_components_strs = strh::split_alphabetical(string);

    ASSERT(time_components_strs.size() >= sizeof...(time_components),
           std::invalid_argument(fmt::format("components with size '{}' does not match date "
                                             "strings: '{}' with size '{}'",
                                             sizeof...(time_components),
                                             fmt::join(time_components_strs, ", "),
                                             time_components_strs.size())));

    size_t idx = 0;

    // Lambda to get each time component value from each time component string
    auto get_time_components_from_str = [&](const auto& time_component, size_t idx) {
        switch (time_component)
        {
        case TimeComponent::HOUR:
            hour = std::stoi(time_components_strs[idx]);
            break;
        case TimeComponent::MINUTE:
            if (time_components_strs[idx].front() == '0')
                minute = time_components_strs[idx].back()- '0'; // char to int
            else
                minute = std::stoi(time_components_strs[idx]);
            break;
        case TimeComponent::SECOND:
            if (time_components_strs[idx].front() == '0')
                second = time_components_strs[idx].back() - '0'; // char to int
            else
                second = std::stoi(time_components_strs[idx]);
            break;
        case TimeComponent::MILLISECOND:
            millisecond = std::stoi(time_components_strs[idx]);
            break;
        case TimeComponent::MICROSECOND:
            microsecond = std::stoi(time_components_strs[idx]);
            break;
        case TimeComponent::NANOSECOND:
            nanosecond = std::stoi(time_components_strs[idx]);
            break;
        case TimeComponent::TIMEZONE:
            if (time_components_strs[idx].front() == '0')
                timezone = Timezone(time_components_strs[idx].back() - '0');
            else
                timezone = Timezone(std::stoi(time_components_strs[idx]));
            break;
        }
    };

    // Call the lambda function on each time component
    (get_time_components_from_str(time_components, idx++), ...);

    ASSERT(is_valid_time(),
           std::invalid_argument(fmt::format("Time '{}' is invalid", Time::to_string())));
}

inline size_t hash_value(const Time& time)
{
    size_t seed = 0;
    boost::hash_combine(seed, time.hour);
    boost::hash_combine(seed, time.minute);
    boost::hash_combine(seed, time.second);
    boost::hash_combine(seed, time.millisecond);
    boost::hash_combine(seed, time.microsecond);
    boost::hash_combine(seed, time.nanosecond);
    boost::hash_combine(seed, time.timezone.utc_offset);
    return seed;
}

namespace std
{
template<>
struct hash<Time>
{
    size_t operator()(const Time& time) const
    {
        return hash_value(time);
    }
};
}

#endif //TIME_TIME_H
