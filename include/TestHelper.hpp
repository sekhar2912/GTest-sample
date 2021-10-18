#ifndef TESTHELPER_HPP
#define TESTHELPER_HPP

#include "Analyzer.hpp"
#include <ctime>

inline time_t toTime(std::string t) {
    tzset();
    char buf[128];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, t.c_str(), sizeof(buf));
    
    std::tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(buf, "%FT%T%z", &tm);
    time_t ts = std::mktime(&tm) - timezone;
    localtime_r(&ts, &tm);
    return ts;
}


inline WeekDay _getDay(time_t t) {
    const std::tm* time_out = std::localtime(&t);
    return static_cast<WeekDay>(time_out->tm_wday);
}

inline bool testResults(Analyzer analyzer, int id, std::vector<Order> input) {
    std::map<WeekDay, int> actuals, expected;
    
    for (int i = 0; i < 7; i++)
        expected[static_cast<WeekDay>(i)] = 0;
    
    for (auto order : input) {
        WeekDay day = _getDay(order.creationDate);
        for (auto item : order.items) {
            if (item.productId == id)
                expected[day] += item.quantity;
        }
    }
    
    actuals = analyzer.solve(id, input);
    if (expected.size() == actuals.size())
        return std::equal(expected.begin(), expected.end(),
                      actuals.begin());
    return false;
}

#endif // TESTHELPER_HPP
