#include "gtest/gtest.h"
#include <datetime/datetime.h>

int main(int argc, char **argv)
{
        Date::mock_date = Date(2022, 1, 1);
        Time::mock_time = Time(1);
        std::cout << Datetime::now() << std::endl;
        Date::mock_date.reset();
        Time::mock_time.reset();
        std::cout << Datetime::now() << std::endl;
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}