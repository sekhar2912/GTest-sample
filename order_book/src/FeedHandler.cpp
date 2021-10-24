#include <FeedHandler.hpp>
#include <ErrorHandler.h>
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
        //vec_orderPtr.emplace_back(std::move(orderP));
        
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
  int def_val{1};
  std::string symbol_ID = order.get()->symbol_ID;
  std::string timeStamp = order.get()->time_stamp;

  if (sym_count.count(symbol_ID) == 0)
      sym_count[order.get()->symbol_ID] = def_val;
  else
      sym_count.at(order.get()->symbol_ID) += 1;
  
  
  
  if(order.get()->side == Side::BUY)
  {
    orderVol current{order.get()->order_ID,order.get()->vol};
    if(order_buy.count(symbol_ID) == 0)
    {
      std::vector<orderVol> tempVec{current};
      order_buy[symbol_ID] = tempVec;
    }
    else
    {
      std::vector<orderVol> temp = order_buy[symbol_ID];
      temp.push_back(current);
      order_buy[symbol_ID] = temp;

    }
  }
  
  if(order.get()->side == Side::SELL)  
  {
    std::vector<priceOrdVol> tempPriceVec;
    std::unordered_map<std::string,std::vector<priceOrdVol>> inner_timestampmap;
    
    priceOrdVol cpov;
    cpov.order_ID = order.get()->order_ID;
    cpov.price = order.get()->price;
    cpov.vol = order.get()->vol;

    if(order_sell.count(symbol_ID) == 0)
    {
      tempPriceVec.push_back(cpov);
      inner_timestampmap[order.get()->time_stamp] = tempPriceVec;
    }
    else
    {
      inner_timestampmap = order_sell[symbol_ID];
      
      if(inner_timestampmap.count(order.get()->time_stamp) == 0)
      {
        // Not Present. Need to insert a new entry ( key and value)
        tempPriceVec.push_back(cpov);
        inner_timestampmap[order.get()->time_stamp] = tempPriceVec;
      }
      else
      {
        // Append to the existing timestamp key
        tempPriceVec = inner_timestampmap[order.get()->time_stamp];
        tempPriceVec.push_back(cpov);
        inner_timestampmap[order.get()->time_stamp] = tempPriceVec;
      }

    }
    order_sell[symbol_ID] = inner_timestampmap;
  }

}


std::unordered_map<std::string,int> FeedHandler::orderCounts()
{
  return sym_count;
}


std::vector<unsigned int> FeedHandler::BiggestBuyOrder(std::string symbol)
{
    auto mtempOrderVol = order_buy[symbol];
    std::sort(mtempOrderVol.begin(), mtempOrderVol.end());
    int len = mtempOrderVol.size();
    std::vector<unsigned int> mRet =  { mtempOrderVol.at(len).order_ID,
                                    mtempOrderVol.at(len -1).order_ID,
                                    mtempOrderVol.at(len-2).order_ID};

    return mRet;

}

std::tuple<double, unsigned int,unsigned int> FeedHandler::BestSellAtTime(std::string symbol,
                                  std::string timeStamp )
{
  if (order_sell.count(symbol) > 0)
  {
    auto inner_timestampmap = order_sell[symbol];

   

    if (inner_timestampmap.count(timeStamp) > 0)
    {
      auto priceVec = inner_timestampmap[timeStamp];
      std::sort(priceVec.begin(), priceVec.end());
      int size = priceVec.size();
      std::tuple<double, unsigned int,unsigned int> mRet(priceVec.at(size).price, priceVec.at(size).vol,
      priceVec.at(size).order_ID);
      return mRet;
    }
  }
}

void FeedHandler::GenerateResult()
{
  std::string symbol{}, tm{};
  std::unordered_map<std::string,int> oc = orderCounts();
  std::vector<unsigned int> olist = BiggestBuyOrder(symbol);
  std::tuple<double, unsigned int,unsigned int> pov =  BestSellAtTime(symbol,tm);

}