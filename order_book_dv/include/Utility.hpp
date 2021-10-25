#ifndef UTILITY_H
#define UTILITY_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <cmath>
#include <regex>
#include <vector>

enum class Side
{
    UNKNOWN,
    BUY,
    SELL
};
enum class Action
{
    UNKNOWN,
    NEW,
    MODIFY,
    CANCEL
};

struct Order
{
    Side side{Side::UNKNOWN};
    Action operation{Action::UNKNOWN};
    unsigned int order_ID{};
    unsigned int vol{};
    double price{};
    std::string time_stamp{};
    std::string symbol_ID{};
};

struct orderVol
{  
    public:

    unsigned int order_ID{};
    unsigned int vol{};

    bool operator<(const orderVol &rhs) const
    {
        return vol < rhs.vol;
    }
};



struct priceOrdVol
{
  unsigned int order_ID{};
  unsigned int vol{};
  double price{};

  bool operator<(const priceOrdVol& t) const
  {
      return std::isless(this->price,t.price);
  }

};

#endif
