#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <ctime>
#include <map>
#include <string>
#include <vector>

struct Item {
    int productId;
    std::string name;
    int quantity;
    double unitPrice;
};

struct Order {
    int orderId;
    time_t creationDate;
    std::vector<Item> items;
};

enum class WeekDay {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

WeekDay getDay(time_t t);

class Analyzer {
public:
    std::map<WeekDay, int> solve(int productId,
        const std::vector<Order>& orders);
};

#endif // ANALYZER_HPP
