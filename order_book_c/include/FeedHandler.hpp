#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <Utility.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <OrderParsar.hpp>


class FeedHandler {

private:
  std::unique_ptr<OrderParsar> order_parser = std::make_unique<OrderParsar>();
  //std::vector<std::unique_ptr<Order>> vec_orderPtr;
  std::unordered_map<std::string, int> sym_count;
  std::unordered_map<std::string,std::vector<orderVol>> order_buy;
  std::unordered_map<std::string,std::unordered_map<std::string,std::vector<priceOrdVol>>> order_sell;
  
public:
  FeedHandler() = default;
  ~FeedHandler() = default;
  FeedHandler(const FeedHandler& fl) = default;
  FeedHandler& operator=(const FeedHandler& other) = default;
  
public:
  void ProcessMessage(const std::string message_file);
  void GenerateResult();
  std::unordered_map<std::string,int> orderCounts();
  std::vector<unsigned int> BiggestBuyOrder(std::string symbol = "DVAM1");
  std::tuple<double, unsigned int,unsigned int> BestSellAtTime(std::string symbol = "DVAM1" ,
                                  std::string timeStamp = "15:30:30");
private:
void buildPreComputedResult(std::unique_ptr<Order> &order);                                  
};

#endif
