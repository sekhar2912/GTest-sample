#include "boost/lexical_cast.hpp"
#include <OrderParsar.hpp>
#include <ErrorHandler.h>
#include <boost/algorithm/string.hpp>

Side OrderParsar::get_order_side(char order_side) {
  Side side{};
  if (order_side == 'S') {
    side = Side::SELL;
  } else if (order_side == 'B') {
    side = Side::BUY;
  }
  return side;
}

char OrderParsar::get_readable_order_action(Action action) {
  char order_action{};
  if (action == Action::NEW) {
    order_action = 'I';
  } else if (action == Action::MODIFY) {
    order_action = 'M';
  } else if (action == Action::CANCEL) {
    order_action = 'C';
  }
  return order_action;
}

char OrderParsar::get_readable_order_side(Side side) {
  char order_side{};
  if (side == Side::SELL) {
    order_side = 'S';
  } else if (side == Side::BUY) {
    order_side = 'B';
  }
  return order_side;
}

Action OrderParsar::get_order_action(char order_action) {
  Action action{};
  switch (order_action) {
  case 'I':
    action = Action::NEW;
    break;
  case 'A':
    action = Action::MODIFY;
    break;
  case 'C':
    action = Action::CANCEL;
    break;
  default:
    action = Action::UNKNOWN;
    break;
  }
  return action;
}

std::vector<std::string>
OrderParsar::parse_order_message(const std::string &message) {
  std::vector<std::string> results;
  if (!message.empty()) {
    boost::split(results, message, [](char c) { return c == ';'; });
  }
  return std::move(results);
}

void OrderParsar::parse_order(const std::string &message,
                              std::unique_ptr<Order> &order) {
  auto results = parse_order_message(message);
  size_t index{0};
  if (!results.empty()) {
     
    std::string tempTime = boost::lexical_cast<std::string>(results[index++]);
    order->time_stamp = tempTime.substr(0, 8);
    order->symbol_ID = boost::lexical_cast<unsigned int>(results[index++]);
    order->order_ID = boost::lexical_cast<unsigned int>(results[index++]);

    order->operation =
        get_order_action(boost::lexical_cast<char>(results[index++]));
    if (order->operation == Action::UNKNOWN) {
      ErrorHandler::GetInstance().AddErrrors("Message: " + message +
                                             ": is corrupted\n");
      return;
    } 
    order->side = get_order_side(boost::lexical_cast<char>(results[index++]));
    order->vol = boost::lexical_cast<unsigned int>(results[index++]);
    order->price = boost::lexical_cast<double>(results[index++]);
  }
}

Action OrderParsar::get_order_message_action(const std::string &message) {
  Action order_action{};
  auto results = parse_order_message(message);
  if (!results.empty()) {
    order_action = get_order_action(boost::lexical_cast<char>(results[3]));
  }
  if (order_action == Action::UNKNOWN) {
    ErrorHandler::GetInstance().AddErrrors("Message: " + message +
                                           ": is corrupted\n");
  }
  return order_action;
}
