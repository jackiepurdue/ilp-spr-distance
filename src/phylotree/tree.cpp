#include "tree.h"
#include <assert.h>
#include <algorithm>
struct tree::impl {
  impl(std::string value, const std::shared_ptr<impl> left, const std::shared_ptr<impl> right, int tag)
      : _value(value), _left(left), _right(right), _tag(tag) {}
  impl(std::string value, int tag)
      : _value(value) {}
  std::string _value;
  std::shared_ptr<impl> _left;
  std::shared_ptr<impl> _right;
  int _tag;
  void post_order(std::vector<std::string> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      post_order(s, i.get()->_left);
      post_order(s, i.get()->_right);
      s.push_back(i->_value);
    }
  }

  void post_order_int(std::vector<int> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      post_order_int(s, i.get()->_left);
      post_order_int(s, i.get()->_right);
      s.push_back(i->_tag);
    }
  }

  void in_order_int(std::vector<int> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      s.push_back(i->_tag);
      in_order_int(s, i.get()->_left);
      in_order_int(s, i.get()->_right);
    }
  }

  void post_order_depth(std::vector<int> &s, std::shared_ptr<impl> i, int depth) {
    if (i.get() == nullptr)
      return;
    else {
      post_order_depth(s, i.get()->_left, depth + 1);
      post_order_depth(s, i.get()->_right, depth + 1);
      s.push_back(depth);
    }
  }

  void tour(std::vector<std::string> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      s.push_back(i->_value);
      tour(s, i.get()->_left);
      if (i.get()->_left != nullptr)
        s.push_back(i->_value);
      tour(s, i.get()->_right);
      if (i.get()->_right != nullptr)
        s.push_back(i->_value);
    }
  }

  void tour_int(std::vector<int> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      s.push_back(i->_tag);
      tour_int(s, i.get()->_left);
      if (i.get()->_left != nullptr)
        s.push_back(i->_tag);
      tour_int(s, i.get()->_right);
      if (i.get()->_right != nullptr)
        s.push_back(i->_tag);
    }
  }

  void tour_depth(std::vector<int> &s, int depth, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      s.push_back(depth);
      tour_depth(s, depth + 1, i.get()->_left);
      if (i.get()->_left != nullptr)
        s.push_back(depth);
      tour_depth(s, depth + 1, i.get()->_right);
      if (i.get()->_right != nullptr)
        s.push_back(depth);
    }
  }

  void leaves(std::vector<std::string> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      leaves(s, i.get()->_left);
      leaves(s, i.get()->_right);
      if (i.get()->_left == nullptr && i.get()->_right == nullptr)
        s.push_back(i->_value);
    }
  }

  void leaves_int(std::vector<int> &s, std::shared_ptr<impl> i) {
    if (i.get() == nullptr)
      return;
    else {
      leaves_int(s, i.get()->_left);
      leaves_int(s, i.get()->_right);
      if (i.get()->_left == nullptr && i.get()->_right == nullptr)
        s.push_back(i->_tag);
    }
  }

  std::vector<std::string> tour(std::vector<std::string> &v) {
    tour(v, std::make_shared<impl>(*this));
    return v;
  }

  std::vector<int> tour_int(std::vector<int> &v) {
    tour_int(v, std::make_shared<impl>(*this));
    return v;
  }

  std::vector<int> tour_depth(std::vector<int> &v) {
    tour_depth(v, 0, std::make_shared<impl>(*this));
    return v;
  }

  std::vector<std::string> leaves(std::vector<std::string> &v) {
    leaves(v, std::make_shared<impl>(*this));
    return v;
  }

  tree most_recent_common_ancestor(const tree &root, std::string i, std::string j) const {
    if (root.is_empty()) {
      return tree();
    }
    if (root.root() == i || root.root() == j) {
      return root;
    }
    auto left = most_recent_common_ancestor(root.left(), i, j);
    auto right = most_recent_common_ancestor(root.right(), i, j);
    if (!left.is_empty() && !right.is_empty()) {
      return root;
    }
    if (left.is_empty() && right.is_empty()) {
      return tree();
    }
    return (!left.is_empty()) ? left : right;
  }

  std::pair<tree, int> most_recent_common_ancestor_depth(const tree &root, std::string i, std::string j, int depth) const {
    if (root.is_empty()) {
      return std::make_pair(tree(), depth);
    }
    if (root.root() == i || root.root() == j) {
      return std::make_pair(root, depth);
    }
    auto left = most_recent_common_ancestor_depth(root.left(), i, j, depth + 1);
    auto right = most_recent_common_ancestor_depth(root.right(), i, j, depth + 1);
    if (!left.first.is_empty() && !right.first.is_empty()) {
      return std::make_pair(root, depth);
    }
    if (left.first.is_empty() && right.first.is_empty()) {
      return std::make_pair(tree(), depth);
    }
    return (!left.first.is_empty()) ? left : right;
  }

  tree path_from(const tree &root, std::string i, std::string j,
                 std::set<std::pair<std::string, std::string>> &edges) const {
    if (root.is_empty()) {
      return tree();
    }
    if (root.root() == i || root.root() == j) {

      return root;
    }
    auto left = path_from(root.left(), i, j, edges);
    if (!left.is_empty()) {
      edges.insert(std::pair<std::string, std::string>(root.root(), root.left().root()));
    }
    auto right = path_from(root.right(), i, j, edges);
    if (!right.is_empty()) {
      edges.insert(std::pair<std::string, std::string>(root.root(), root.right().root()));
    }
    if (!left.is_empty() && !right.is_empty()) {

      return tree();
    }
    if (left.is_empty() && right.is_empty()) {
      return tree();
    }
    return (!left.is_empty()) ? left : right;
  }

  tree path_from(const tree &root, std::string i, std::string j,
                 std::set<std::set<int>> &edges) const {
    if (root.is_empty()) {
      return tree();
    }
    if (root.root() == i || root.root() == j) {

      return root;
    }
    auto left = path_from(root.left(), i, j, edges);
    if (!left.is_empty()) {
      edges.insert({root._internals.get()->_tag, root.left()._internals.get()->_tag});
    }
    auto right = path_from(root.right(), i, j, edges);
    if (!right.is_empty()) {
      edges.insert({root._internals.get()->_tag, root.right()._internals.get()->_tag});
    }
    if (!left.is_empty() && !right.is_empty()) {

      return tree();
    }
    if (left.is_empty() && right.is_empty()) {
      return tree();
    }
    return (!left.is_empty()) ? left : right;
  }

  tree path_from_int(const tree &root, int i, int j,
                 std::set<std::set<int>> &edges) const {

    if (root.is_empty()) {
      return tree();
    }
    if (root.root_int() == i || root.root_int() == j) {

      return root;
    }
    auto left = path_from_int(root.left(), i, j, edges);
    if (!left.is_empty()) {
      edges.insert({root._internals.get()->_tag, root.left()._internals.get()->_tag});
    }
    auto right = path_from_int(root.right(), i, j, edges);
    if (!right.is_empty()) {
      edges.insert({root._internals.get()->_tag, root.right()._internals.get()->_tag});
    }
    if (!left.is_empty() && !right.is_empty()) {

      return tree();
    }
    if (left.is_empty() && right.is_empty()) {
      return tree();
    }
    return (!left.is_empty()) ? left : right;
  }

  void all_edges(std::set<std::pair<std::string, std::string>> &edges) const {
    all_edges(edges, std::make_shared<impl>(*this));
  }

  void all_edges(std::set<std::pair<std::string, std::string>> &edges, std::shared_ptr<impl> i) const {
    if (i.get() == nullptr)
      return;
    else {

      all_edges(edges, i->_left);

      if (i->_right.get() != nullptr) {
        edges.insert(std::pair<std::string, std::string>(i->_value, i->_right->_value));
      }
      all_edges(edges, i->_right);

      if (i->_left.get() != nullptr) {
        edges.insert(std::pair<std::string, std::string>(i->_value, i->_left->_value));
      }
      return;
    }
  }

  void all_edges_int(std::set<std::set<int>> &edges) const {
    all_edges_int(edges, std::make_shared<impl>(*this));
  }

  void all_edges_int(std::set<std::set<int>> &edges, std::shared_ptr<impl> i) const {
    if (i.get() == nullptr)
      return;
    else {

      all_edges_int(edges, i->_left);

      if (i->_right.get() != nullptr) {
        edges.insert(std::set<int>{i->_tag, i->_right->_tag});
      }
      all_edges_int(edges, i->_right);

      if (i->_left.get() != nullptr) {
        edges.insert(std::set<int>{i->_tag, i->_left->_tag});
      }
      return;
    }
  }

  bool parent(tree tree, int target, int &result) {
    /* base cases */
    if (tree.is_empty())
      return false;

    auto ans = tree.root_int();
    if (ans == target)

      return true;

    /* If target is present in either left or right subtree of this node,
       then print this node */

    if (parent(tree.left(), target, result)) {
      result = _tag;
      return false;
    }


    /* Else return false */
    return false;
  }

};

tree::tree() {}

tree::tree(std::string value, int tag)
    : _internals(std::make_shared<impl>(value, tag)) {
}

tree::tree(std::string value, const tree &left, const tree &right, int tag)
    : _internals(std::make_shared<impl>(value, left._internals, right._internals, tag)) {}

std::string tree::root() const {
  assert(!is_empty());
  return _internals->_value;
}

int tree::root_int() const {
  assert(!is_empty());
  return _internals->_tag;
}

tree tree::right() const {
  assert(!is_empty());
  if (_internals->_right != nullptr) {
    return tree(_internals->_right, _internals->_right->_tag);
  } else {
    return tree(_internals->_right, -1);
  }
}

tree tree::left() const {
  assert(!is_empty());
  if (_internals->_left != nullptr) {
    return tree(_internals->_left, _internals->_left->_tag);
  } else {
    return tree(_internals->_left, -1);
  }

}

bool tree::is_empty() const {
  return !_internals;
}


std::vector<std::string> tree::leaves() const {
  std::vector<std::string> v;
  return _internals->leaves(v);
}

tree::tree(std::shared_ptr<impl> const &node, int tag) : _internals(node) {};


void tree::path_from(int i, int j, std::set<std::set<int>> &edges) const {
  _internals->path_from_int(*this, i, j, edges);
}



void tree::all_edges_int(std::set<std::set<int>> &edges) const {
  _internals->all_edges_int(edges);
}

std::vector<std::string> tree::tour() const {
  std::vector<std::string> v;
  return _internals->tour(v);
}

std::vector<int> tree::tour_int() const {
  std::vector<int> v;
  return _internals->tour_int(v);
}

std::vector<int> tree::tour_depth() const {
  std::vector<int> v;
  return _internals->tour_depth(v);
}


std::map<std::string, int> tree::leaves_index() const {

  std::map<std::string, int> v;
  auto tour = this->tour_int();
  auto name_tour = this->tour();

  for (int i = 1; i < tour.size() - 1; i++) {

    if (tour[i - 1] == tour[i + 1]) {
      v.insert(std::make_pair(name_tour[i], i));
    }
  }
  return v;
}

int tree::parent(int target) {
  int ans;
  _internals.get()->parent(*this, target, ans);
  return ans;
}

