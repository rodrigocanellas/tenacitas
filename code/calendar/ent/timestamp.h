#ifndef TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_H
#define TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_H

#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>

namespace tenacitas {
namespace calendar {
namespace ent {

template<typename t_time_precision>
struct timestamp;

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif

// void
// amount_tests()
//{

//  {
//    amount<day> _n00;
//    std::cout << "n00 = " << _n00 << std::endl;
//  }
//  {
//    amount<day> _n01(300);
//    ++_n01;
//    std::cout << "n01 = " << _n01 << std::endl;
//  }
//  {
//    amount<day> _n02(300);
//    _n02++;
//    std::cout << "n02 = " << _n02 << std::endl;
//  }
//  {
//    amount<day> _n03(300);
//    _n03 += amount<day>(120);
//    std::cout << "n03 = " << _n03 << std::endl;
//  }
//  {
//    amount<day> _n04(300);
//    _n04 += amount<day>(120);
//    std::cout << "n04 = " << _n04 << std::endl;
//  }
//  {
//    amount<day> _n05(300);
//    amount<day> _n06 = _n05 + amount<day>(120);
//    std::cout << "n06 = " << _n06 << std::endl;
//  }
//}

// int
// main()
//{
//  {
//    std::cout << "wed - mon = " << weekday::wed - weekday::mon << std::endl;
//    std::cout << "mon - wed = " << weekday::mon - weekday::wed << std::endl;
//  }
//  {
//    std::cout << "minute::seconds() = " << minute::seconds() << std::endl;
//  }
//  {
//    std::cout << "hour::minutes() = " << hour::minutes() << std::endl;
//    std::cout << "hour::seconds() = " << hour::seconds() << std::endl;
//  }
//  {
//    std::cout << "day::hours() = " << day::hours() << std::endl;
//    std::cout << "day::minutes() = " << day::minutes() << std::endl;
//    std::cout << "day::seconds() = " << day::seconds() << std::endl;
//  }
//  {
//    std::cout << "weekday::days() = " << weekday::days() << std::endl;
//    std::cout << "weekday::hours() = " << weekday::hours() << std::endl;
//    std::cout << "weekday::minutes() = " << weekday::minutes() << std::endl;
//    std::cout << "weekday::seconds() = " << weekday::seconds() << std::endl;
//  }
//  {
//    std::cout << "month::days(month::feb, year(2020)) = "
//              << month::days(month::feb, year(2020)) << std::endl;
//    std::cout << "month::days(month::feb, year(2019)) = "
//              << month::days(month::feb, year(2019)) << std::endl;
//    std::cout << "month::hours(month::feb, year(2020)) = "
//              << month::hours(month::feb, year(2020)) << std::endl;
//    std::cout << "month::hours(month::feb, year(2019)) = "
//              << month::hours(month::feb, year(2019)) << std::endl;
//    std::cout << "month::minutes(month::feb, year(2020)) = "
//              << month::minutes(month::feb, year(2020)) << std::endl;
//    std::cout << "month::minutes(month::feb, year(2019)) = "
//              << month::minutes(month::feb, year(2019)) << std::endl;
//    std::cout << "month::seconds(month::feb, year(2020)) = "
//              << month::seconds(month::feb, year(2020)) << std::endl;
//    std::cout << "month::seconds(month::feb, year(2019)) = "
//              << month::seconds(month::feb, year(2019)) << std::endl;
//  }
//  {
//    std::cout << "year::days(year::feb, year(2020)) = "
//              << year::days(year(2020)) << std::endl;
//    std::cout << "year::days(year(2019)) = " << year::days(year(2019))
//              << std::endl;
//    std::cout << "year::hours(year(2020)) = " << year::hours(year(2020))
//              << std::endl;
//    std::cout << "year::hours(year(2019)) = " << year::hours(year(2019))
//              << std::endl;
//    std::cout << "year::minutes(year(2020)) = " << year::minutes(year(2020))
//              << std::endl;
//    std::cout << "year::minutes(year(2019)) = " << year::minutes(year(2019))
//              << std::endl;
//    std::cout << "year::seconds(year(2020)) = " << year::seconds(year(2020))
//              << std::endl;
//    std::cout << "year::seconds(year(2019)) = " << year::seconds(year(2019))
//              << std::endl;
//  }

//  {
//    std::cout << "*************** month" << std::endl;
//    timestamp<second> _now;
//    std::cout << "now = " << _now << " + 6 months = " << _now +
//    amount<month>(6)
//              << std::endl;

//    std::cout << "now = " << _now
//              << " + 12 months = " << _now + amount<month>(12) << std::endl;

//    std::cout << "now = " << _now
//              << " + 17 months = " << _now + amount<month>(17) << std::endl;

//    std::cout << "now = " << _now << " - 6 months = " << _now -
//    amount<month>(6)
//              << std::endl;

//    std::cout << "now = " << _now
//              << " - 12 months = " << _now - amount<month>(12) << std::endl;

//    std::cout << "now = " << _now
//              << " - 17 months = " << _now - amount<month>(17) << std::endl;
//  }

//  {
//    std::cout << "*************** year" << std::endl;
//    timestamp<second> _now;

//    std::cout << "now = " << _now << " + 1 year = " << _now + amount<year>(1)
//              << std::endl;
//    std::cout << "now = " << _now << " - 1 year = " << _now - amount<year>(1)
//              << std::endl;

//    _now += amount<year>(2);
//    std::cout << "now + 2 years = " << _now << std::endl;

//    _now -= amount<year>(4);
//    std::cout << "now - 4 years = " << _now << std::endl;
//  }

//  return 0;
//}
