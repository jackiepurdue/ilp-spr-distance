#include "glpk/param.h"
#include <iostream>
static std::string S = "";
static std::string T = "";
static int PARAM = 7;

void readin() {
  std::string line;
  int count = 0;
  while (std::cin >> line) {
    if (!std::cin.eof()) {
      if (count == 0) {
        S = line;
        std::cout << "S: " << S << std::endl;
      } else if (count == 1) {
        T = line;
        std::cout << "T: " << T << std::endl;
      } else if (count == 2) {
        PARAM = std::stoi(line);
        std::cout << "Depth: " << PARAM << std::endl;
      }

      count++;
    } else {
      break;
    }
  }

}


int main() {

  readin();
  auto s = S;
  auto t = T;
  std::cout << "Computing Matrix" << std::endl;
  auto sprparam = param(s, t, PARAM, false);
  sprparam.formulate();
  std::cout << "Solving with GLPK:" << std::endl;
  auto ans = sprparam.solve(PARAM);
  sprparam.delete_problem();
  std::cout << "SPR distance = " << ans << std::endl;
  return 0;

}