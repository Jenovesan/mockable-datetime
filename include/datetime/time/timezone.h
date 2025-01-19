#ifndef DATETIME_TIMEZONE_H
#define DATETIME_TIMEZONE_H

#include <iostream>
#include <chrono>
#include "fmt/format.h"

/**
 * Timezone.
 *
 * Used in 'Time'.
 */
struct Timezone
{
public:
    /**
     * The difference in hours between the UTC's hour and the this 'Timezone's' hour.
     */
    int utc_offset;

    /**
     * Creates a 'Timezone' object with a given UTC offset.
     *
     * @param utc_offset the UTC offset of the 'Timezone'.
     */
    explicit Timezone(int utc_offset) :
            utc_offset(utc_offset) {}

    /**
     * Gets the difference in UTC offsets between this 'Timezone' and 'other'.
     *
     * @param other the 'Timezone' comparing to.
     *
     * @return the UTC offset
     *
     * @example
     * int est_cst_utc_offset_diff = TZ::EST.get_utc_offset_diff(TZ::CST);
     * std::cout << est_cst_utc_offset_diff;
     *
     * // output: -1
     *
     * int cst_est_utc_offset_diff = TZ::CST.get_utc_offset_diff(TZ::EST);
     * std::cout << cst_est_utc_offset_diff;
     *
     * //output: 1
     */
    int get_utc_offset_diff(Timezone other) const
    {
        return utc_offset - other.utc_offset;
    }

    /**
     * Checks if 'other' has the same 'utc_offset' as this 'Timezone'.
     *
     * @param other the 'Timezone' comparing to.
     *
     * @return 'true' if the utc_offset of this 'Timezone' is the same as 'other', 'false'
     * otherwise.
     */
    bool operator==(const Timezone& other) const
    {
        return utc_offset == other.utc_offset;
    }

    /**
     * Checks if 'other' has a different 'utc_offset' as this 'Timezone'.
     *
     * @param other the 'Timezone' comparing to.
     *
     * @return 'true' if the utc_offset of this 'Timezone' is different than 'other', 'false'
     * otherwise.
     */
    bool operator!=(const Timezone& other) const
    {
        return utc_offset != other.utc_offset;
    }

    /**
     * 'std::string' representation of 'this'.
     *
     * @return 'std::string' representation of 'this'.
     *
     * @example
     * Timezone est = TZ::EST;
     * std::string timezone_string = est.to_string();
     * std::cout << timezone_string;
     *
     * Output: 5:00
     */
    std::string to_string() const
    {
        return fmt::format("{}:00", utc_offset);
    }

    /**
     * Outputs 'timezone' into 'os'.
     *
     * @param os std::ostream' to insert 'timezone' into.
     * @param timezone 'Timezone' to insert into 'os'.
     *
     * @return reference to 'os' after inserting 'timezone' into 'os'.
     */
    friend std::ostream& operator<<(std::ostream& os, const Timezone& timezone)
    {
        return os << timezone.to_string();
    }
};

namespace TZ
{
    /**
     * The Universal Time Coordinated (UTC) timezone.
     */
    const Timezone UTC = Timezone(0);

    /**
     * The Pacific Standard Time (PST) timezone.
     */
    const Timezone PST = Timezone(8);

    /**
     * The Pacific Daylight Time (PDT) timezone.
     */
    const Timezone PDT = Timezone(7);

    /**
     * The Central Standard Time (CST) timezone.
     */
    const Timezone CST = Timezone(6);

    /**
    * The Central Daylight Time (CDT) timezone.
    */
    const Timezone CDT = Timezone(5);

    /**
     * Eastern Standard Time (EST) timezone.
     */
    const Timezone EST = Timezone(5);

    /**
    * Eastern Daylight Time (EDT) timezone.
    */
    const Timezone EDT = Timezone(4);

    namespace helpers
    {
        /**
         * Gets the 'Timezone' from a string.
         *
         * @param timezone_string name of a timezone from the result of std::localtime(...).
         *
         * @return 'Timezone' that corresponds with 'timezone_string'.
         *
         * @throws std::invalid_argument Thrown if no timezones matched 'timezone_string'.
         */
        static Timezone get_from_str(std::string_view timezone_string)
        {
            if (timezone_string == "Coordinated Universal Time")
                return UTC;
            else if (timezone_string == "Pacific Standard Time")
                return TZ::PST;
            else if (timezone_string == "Central Standard Time" || timezone_string == "Central America Standard Time" || timezone_string == "CST")
                return TZ::CST;
            else if (timezone_string == "Central Daylight Time")
                return TZ::CDT;
            else if (timezone_string == "Eastern Standard Time")
                return TZ::EST;
            else if (timezone_string == "Eastern Daylight Time")
                return TZ::EDT;
            else if (timezone_string == "Pacific Daylight Time")
                return TZ::PDT;
            else if (timezone_string == "UTC")
                return TZ::UTC;
            throw std::invalid_argument(fmt::format("'{}' is not a valid timezone string", timezone_string));
        }

/**
 * Gets the local 'Timezone' based on the user's location.
 *
 * @return local 'Timezone'.
 */
        static Timezone get_local_tz()
        {
            // Get the current system time
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            // Convert the system time to a std::time_t object
            std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
            // Convert the std::time_t object to a std::tm object
            std::tm *now_tm = std::localtime(&now_time_t);
            // Get the name of the local timezone
            char timezone_name[128];
            std::strftime(timezone_name, sizeof(timezone_name), "%Z", now_tm);

            return helpers::get_from_str(timezone_name);
        }
    }

/**
 * Local 'Timezone' based on user's location.
 */
    const Timezone LOCAL = helpers::get_local_tz();
}

#endif //DATETIME_TIMEZONE_H
