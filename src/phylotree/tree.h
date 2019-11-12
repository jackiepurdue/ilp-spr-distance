#ifndef CSPR_TREE_H
#define CSPR_TREE_H

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <map>

class tree {

 public:
  tree();
  tree(std::string value,int tag);
  tree(std::string value, const tree &left, const tree &right,int tag);
  std::string root() const;
  int root_int() const;
  tree left() const;
  tree right() const;
  bool is_empty() const;
  std::vector<std::string> leaves() const;

  std::vector<std::string> tour() const;
  std::vector<int> tour_int() const;
  std::vector<int> tour_depth() const;
  std::map<std::string, int> leaves_index() const;
  void all_edges_int(std::set<std::set<int>> &edges) const;

  void path_from(int i,
                 int j,
                 std::set<std::set<int>> &edges) const;
  int parent(int target);
 private:
  struct impl;
  std::shared_ptr<impl> _internals;
  explicit tree(std::shared_ptr<impl> const & node, int tag);


};

#endif
