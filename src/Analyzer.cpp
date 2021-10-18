#include <ctime>
#include <map>
#include <cstring>
#include <iostream>
#include "Analyzer.hpp"

using namespace  std;

WeekDay getDay(time_t t) {
    
 const std::tm* time_out = std::localtime(&t);
return static_cast<WeekDay>(time_out->tm_wday);

    
}



std::map<WeekDay, int> Analyzer::solve(int productId, const std::vector<Order>& orders) {
    std::map<WeekDay, int> dayQty;
    for (int i = 0; i < 7; i++)
        dayQty[static_cast<WeekDay>(i)] = 0;
    
    for (auto order : orders) {
        WeekDay day = getDay(order.creationDate);
        for (auto item : order.items) {
            if (item.productId == productId)
                dayQty[day] += item.quantity;
        }
    }

    return dayQty;
}
