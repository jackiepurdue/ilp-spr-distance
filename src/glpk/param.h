#include <memory>

#ifndef CSPR_PARAM_H
#define CSPR_PARAM_H

class param {
 public:
  param(std::string tree1, std::string tree2, int param, bool verbose);
  int solve(int param);
  void formulate();
  void delete_problem();
private:
  struct impl;
  std::shared_ptr<impl> _internals;
};

#endif
