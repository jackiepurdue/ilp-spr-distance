#include <memory>

#ifndef CSPR_NORMAL_H
#define CSPR_NORMAL_H

class orig {
 public:
  orig(std::string tree1, std::string tree2, bool verbose);
  int solve();
  void formulate();
  void delete_problem();
private:
  struct impl;
  std::shared_ptr<impl> _internals;
};

#endif
