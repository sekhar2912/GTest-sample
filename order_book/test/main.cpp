#include <FeedHandler.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "No input file\n";
    return -1;
  }
  std::cout << "Order Book Manager\n";
  FeedHandler f;
  f.ProcessMessage(argv[1]);
  return 0;
}
