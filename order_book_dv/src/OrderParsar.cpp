#include <OrderParsar.hpp>
#include <ErrorHandler.hpp>
#include <iostream>
#include <regex>
#include <string>
#include <vector>


Side OrderParsar::get_order_side(char order_side) {
	Side side{};
	if (order_side == 'S') {
		side = Side::SELL;
	}
	else if (order_side == 'B') {
		side = Side::BUY;
	}
	return side;
}

char OrderParsar::get_readable_order_action(Action action) {
	char order_action{};
	if (action == Action::NEW) {
		order_action = 'I';
	}
	else if (action == Action::MODIFY) {
		order_action = 'M';
	}
	else if (action == Action::CANCEL) {
		order_action = 'C';
	}
	return order_action;
}

char OrderParsar::get_readable_order_side(Side side) {
	char order_side{};
	if (side == Side::SELL) {
		order_side = 'S';
	}
	else if (side == Side::BUY) {
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

std::vector<std::string> tokenize(
	const std::string str,
	const std::regex re)
{
	std::sregex_token_iterator it{ str.begin(),
							str.end(), re, -1 };
	std::vector<std::string> tokenized{ it, {} };

	// Additional check to remove empty strings
	tokenized.erase(
		std::remove_if(tokenized.begin(),
			tokenized.end(),
			[](std::string const& s) {
		return s.size() == 0;
	}),
		tokenized.end());

	return tokenized;
}


std::vector<std::string>
OrderParsar::parse_order_message(const std::string &message) {
	std::vector<std::string> mRes{};
	const std::regex re(R"([\s|;]+)");
	if (!message.empty()) {
		const std::vector<std::string> tokenized = tokenize(message, re);
		return tokenized;
	}
	return mRes;
}

void OrderParsar::parse_order(const std::string &message,
	std::unique_ptr<Order> &order) {
	auto results = parse_order_message(message);
	size_t index{ 0 };
	if (!results.empty()) {

		std::string tempTime = results[index++];
		order->time_stamp = tempTime.substr(0, 8);
		order->symbol_ID = results[index++];
		order->order_ID = std::stoi(results[index++]);
		char c = *(results[index++]).c_str();
		order->operation = get_order_action(c);
		
		if (order->operation == Action::UNKNOWN) {
			ErrorHandler::GetInstance().AddErrrors("Message: " + message +
				": is corrupted\n");
			return;
		}
		char s = *(results[index++]).c_str();
		order->side = get_order_side(s);
		order->vol = std::stoi(results[index++]);
		order->price = std::stod(results[index++]);
	}
}

Action OrderParsar::get_order_message_action(const std::string &message) {
	Action order_action{};
	auto results = parse_order_message(message);
	if (!results.empty()) {
		order_action = get_order_action(*(results[3]).c_str());
	}
	if (order_action == Action::UNKNOWN) {
		ErrorHandler::GetInstance().AddErrrors("Message: " + message +
			": is corrupted\n");
	}
	return order_action;
}
