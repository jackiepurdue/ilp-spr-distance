#ifndef EULER_TREE_H
#define EULER_TREE_H

#include <vector>
#include <string>
#include <memory>
#include <set>
#include "tree.h"

class euler_tree {

public:
  euler_tree(const tree& t);
  void incompatible_triples(const tree &tree, std::vector<std::pair<int, std::pair<int,int>>> &triples);
  void incompatible_paths(const tree &tree, std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> &triples);
  std::map<std::string, int> leaf_map() const;
  int parent() const;
  int most_recent_common_ancestor(int i, int j);
  int most_recent_common_depth(int i, int j);
  void topath(
              std::pair<int, std::pair<int, int>> &triple,
              std::set<std::set<int>> &edges);
  void topath(std::pair<std::pair<int, int>, std::pair<int, int>> &path, std::set<std::set<int>> &edges);
private:
  struct impl;
  std::shared_ptr<impl> _internals;


};

#endif
