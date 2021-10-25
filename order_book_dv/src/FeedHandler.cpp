#include <FeedHandler.hpp>
#include <ErrorHandler.hpp>
#include <fstream>
#include <algorithm>

void FeedHandler::ProcessMessage(const std::string message_file)
{
	std::string line;
	std::ifstream msg_file(message_file);
	if (msg_file.is_open())
	{
		while (getline(msg_file, line))
		{
			auto orderP = std::make_unique<Order>();
			order_parser->parse_order(line, orderP);
			buildPreComputedResult(orderP);
		}
		ErrorHandler::GetInstance().PrintErrors();
		msg_file.close();

		GenerateResult();
	}
	else
	{
		std::cout << "Could not open the file\n";
	}
}

void FeedHandler::buildPreComputedResult(std::unique_ptr<Order> &order)
{
	int def_val{ 1 };
	std::string symbol_ID = order.get()->symbol_ID;
	std::string timeStamp = order.get()->time_stamp;
	Action act = order.get()->operation;


	if (act != Action::CANCEL && act != Action::UNKNOWN && act != Action::MODIFY)
	{
		if (sym_count.count(symbol_ID) == 0)
			sym_count[order.get()->symbol_ID] = def_val;
		else
			sym_count.at(order.get()->symbol_ID) += 1;
	}
	else if (act == Action::CANCEL)
	{
		int count = sym_count[order.get()->symbol_ID];
		count = count - 1;
		sym_count[order.get()->symbol_ID] = count;
		if (count == 0)
			sym_count.erase(order.get()->symbol_ID);
	}


	if (act != Action::UNKNOWN)
	{
		if (order.get()->side == Side::BUY)
		{
			orderVol current{ order.get()->order_ID, order.get()->vol };
			if (order_buy.count(symbol_ID) == 0)
			{
				std::vector<orderVol> tempVec{ current };
				order_buy[symbol_ID] = tempVec;
			}
			else
			{
				if (act == Action::CANCEL || act == Action::MODIFY)
				{
					// Perform search and deletion
					int mCurrentOrder = order.get()->order_ID;
					auto inner_vec = order_buy[symbol_ID];
					for (auto it = inner_vec.begin();
						it != inner_vec.end(); ++it)
					{
						if (it->order_ID == mCurrentOrder)
						{
							inner_vec.erase(it);
							break;
						}
					}
					if (act == Action::MODIFY)
					{
						inner_vec.push_back(current);

					}
					order_buy[symbol_ID] = inner_vec;
				}
				else
				{
					std::vector<orderVol> temp = order_buy[symbol_ID];
					temp.push_back(current);
					order_buy[symbol_ID] = temp;
				}
			}
		}
		else if (order.get()->side == Side::SELL)
		{
			std::vector<priceOrdVol> tempPriceVec;
			std::unordered_map<std::string, std::vector<priceOrdVol>> inner_timestampmap;
			bool cm = false;
			if (act == Action::CANCEL || act == Action::MODIFY) cm = true;
			priceOrdVol cpov;
			cpov.order_ID = order.get()->order_ID;
			cpov.price = order.get()->price;
			cpov.vol = order.get()->vol;

			if (order_sell.count(symbol_ID) == 0)
			{
				tempPriceVec.push_back(cpov);
				inner_timestampmap[order.get()->time_stamp] = tempPriceVec;
			}
			else
			{
				inner_timestampmap = order_sell[symbol_ID];

				if (inner_timestampmap.count(order.get()->time_stamp) == 0 && !cm)
				{
					// Not Present. Need to insert a new entry ( key and value)
					tempPriceVec.push_back(cpov);
					inner_timestampmap[order.get()->time_stamp] = tempPriceVec;
				}
				else
				{
					tempPriceVec = inner_timestampmap[order.get()->time_stamp];
					if (act == Action::CANCEL || act == Action::MODIFY)
					{
						int mCurrentOrder = order.get()->order_ID;
						for (auto it = tempPriceVec.begin(); it != tempPriceVec.end(); ++it)
						{
							if (it->order_ID == mCurrentOrder)
							{
								if (act == Action::CANCEL) {
									tempPriceVec.erase(it);
									break;
								}
								else
								{
									it->price = order.get()->price;
									it->vol = order.get()->vol;

								}
							}
						}
					}
					else
					{
						// Append to the existing timestamp key
						tempPriceVec.push_back(cpov);
						inner_timestampmap[order.get()->time_stamp] = tempPriceVec;
					}
				}
			}
			order_sell[symbol_ID] = inner_timestampmap;
		}
	}
}


std::unordered_map<std::string, int> FeedHandler::orderCounts()
{
	return sym_count;
}

std::vector<unsigned int> FeedHandler::BiggestBuyOrder(std::string symbol)
{
	auto mtempOrderVol = order_buy[symbol];
	std::sort(mtempOrderVol.begin(), mtempOrderVol.end());
	int len = mtempOrderVol.size();
	int counter = 0;
	std::vector<unsigned int> mRet;
	for (int i = 0; i < len; i++)
	{
		counter++;
		if (counter > 3) break;
		mRet.push_back(mtempOrderVol.at(i).order_ID);
	}
	return mRet;

}

std::tuple<double, unsigned int, unsigned int> FeedHandler::BestSellAtTime(std::string symbol,
	std::string timeStamp)
{
	std::tuple<double, unsigned int, unsigned int> mRetVal;
	if (order_sell.count(symbol) > 0)
	{
		auto inner_timestampmap = order_sell[symbol];

		if (inner_timestampmap.count(timeStamp) > 0)
		{
			auto priceVec = inner_timestampmap[timeStamp];
			std::sort(priceVec.begin(), priceVec.end());
			int pos = priceVec.size() - 1;
			std::tuple<double, unsigned int, unsigned int> mRet(priceVec.at(pos).price, priceVec.at(pos).vol,
				priceVec.at(pos).order_ID);
			return mRet;
		}
	}
	return mRetVal;
}

void FeedHandler::GenerateResult()
{
	std::unordered_map<std::string, int> oc = orderCounts();
	std::vector<unsigned int> olist = BiggestBuyOrder();
	std::tuple<double, unsigned int, unsigned int> pov = BestSellAtTime();
}