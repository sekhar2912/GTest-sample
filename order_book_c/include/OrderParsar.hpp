#include <Utility.hpp>
#include <iostream>
#include <string>
#include <vector>

#ifndef MESSAGE_PARSAR_H
#define MESSAGE_PARSAR_H

class OrderParsar {
public:
  static char get_readable_order_action(Action action);
  static char get_readable_order_side(Side side);

  void parse_order(const std::string &message, std::unique_ptr<Order> &order);
  Action get_order_message_action(const std::string &message);
  std::vector<std::string> parse_order_message(const std::string &message);

private:
  Side get_order_side(char order_side);
  Action get_order_action(char order_action);
};
#endif
