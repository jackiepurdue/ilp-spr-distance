#include "euler_tree.h"
#include "sparse_table.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <map>

struct euler_tree::impl {

  tree _t;
  std::vector<int> _int_tour;
  sparse_table<int> _st;
  sparse_table<int> _st_depth;
  std::map<std::string, int> _leaf_indicies;
  std::vector<int> _depth_mapy;

  impl(const tree t)
      : _t(t),
        _int_tour(t.tour_int()),
        _st(sparse_table<int>(t.tour_int())),
        _st_depth(sparse_table<int>(t.tour_depth())),
        _leaf_indicies(t.leaves_index()),
        _depth_mapy(t.tour_depth()) {}


  void topath(std::pair<int, std::pair<int, int>> &triple, std::set<std::set<int>> &edges) {

    auto leaves_s = _leaf_indicies;

    int i = triple.first;
    int j = triple.second.first;
    int k = triple.second.second;
    _t.path_from(i, j, edges);
    _t.path_from(i, k, edges);

  }

  void topath(std::pair<std::pair<int, int>, std::pair<int, int>> &paths, std::set<std::set<int>> &edges) {

    auto leaves_s = _leaf_indicies;

    int i = paths.first.first;
    int j = paths.first.second;
    int k = paths.second.first;
    int l = paths.second.second;
    _t.path_from(i, j, edges);
    _t.path_from(k, l, edges);
  }

  void incompatible_triples(const tree &t, std::vector<std::pair<int, std::pair<int, int>>> &triples) {

    auto tour_s = euler_tree(_t);
    auto tour_t = euler_tree(t);

    std::vector<std::string> lS = _t.leaves();
    auto leaves_s = _leaf_indicies;
    auto leaves_t = t.leaves_index();
    for (int i = 0; i < lS.size(); ++i) {
      for (int j = i + 1; j < lS.size(); ++j) {
        for (int k = j + 1; k < lS.size(); ++k) {
          auto li = lS[i];
          auto lj = lS[j];
          auto lk = lS[k];
          auto left = std::min<int>( tour_s.leaf_map().at(lS[i]), tour_s.leaf_map().at(lS[j]));
          auto right = std::max<int>( tour_s.leaf_map().at(lS[i]), tour_s.leaf_map().at(lS[j]));
          auto mrcaij1 = most_recent_common_ancestor(left, right);

          left = std::min<int>( tour_s.leaf_map().at(lS[j]), tour_s.leaf_map().at(lS[k]));
          right = std::max<int>( tour_s.leaf_map().at(lS[j]), tour_s.leaf_map().at(lS[k]));
          auto mrcajk1 = most_recent_common_ancestor(left, right);


         left = std::min<int>( tour_s.leaf_map().at(lS[i]), tour_s.leaf_map().at(lS[k]));
          right = std::max<int>( tour_s.leaf_map().at(lS[i]), tour_s.leaf_map().at(lS[k]));
          auto mrcaik1 = most_recent_common_ancestor(left, right);


          left = std::min<int>( tour_t.leaf_map().at(lS[i]), tour_t.leaf_map().at(lS[j]));
          right = std::max<int>( tour_t.leaf_map().at(lS[i]), tour_t.leaf_map().at(lS[j]));
          auto mrcaij2 = tour_t.most_recent_common_ancestor(left, right);

          left = std::min<int>( tour_t.leaf_map().at(lS[j]), tour_t.leaf_map().at(lS[k]));
          right = std::max<int>( tour_t.leaf_map().at(lS[j]), tour_t.leaf_map().at(lS[k]));
          auto mrcajk2 = tour_t.most_recent_common_ancestor(left, right);


          left = std::min<int>( tour_t.leaf_map().at(lS[i]), tour_t.leaf_map().at(lS[k]));
          right = std::max<int>( tour_t.leaf_map().at(lS[i]), tour_t.leaf_map().at(lS[k]));
          auto mrcaik2 = tour_t.most_recent_common_ancestor(left, right);


          bool fIncTriple = false;

          if (mrcaij1 == mrcajk1) {
            if (mrcaij2 != mrcajk2) {
              fIncTriple = true;
            }
          } else if (mrcaij1 == mrcaik1) {
            if (mrcaij2 != mrcaik2) {
              fIncTriple = true;
            }
          } else {
            if (mrcaik2 != mrcajk2) {
              fIncTriple = true;
            }
          }

          if (fIncTriple == false) {
            continue;
          }

          std::pair<int, int> pp(_t.tour_int()[leaves_s[lS[j]]], _t.tour_int()[leaves_s[lS[k]]]);
          std::pair<int, std::pair<int, int> > ppp(_t.tour_int()[leaves_s[lS[i]]], pp);
          triples.push_back(ppp);
        }
      }
    }
  }

  void incompatible_paths(const tree &t,
                          std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &paths) const {
    auto tour = _t.tour_int();
    auto tour_s = euler_tree(_t);
    auto tour_t = euler_tree(t);
    std::vector<std::string> lS = _t.leaves();
    auto leaves_s = _leaf_indicies;
    auto leaves_t = t.leaves_index();

    for (int i = 0; i < lS.size(); ++i) {
      for (int j = i + 1; j < lS.size(); ++j) {
        for (int k = i + 1; k < lS.size(); ++k) {
          if (k == j) {
            continue;
          }
          for (int l = k + 1; l < lS.size(); ++l) {
            if (l == j) {
              continue;
            }
            if (are_disjoint(i, j, k, l, tour_s, lS, leaves_s) == false) {
              continue;
            }

            if (are_disjoint(i, j, k, l, tour_t, lS, leaves_t) == true) {
              continue;
            }
            std::pair<int, int> pp1(tour[leaves_s[lS[i]]], tour[leaves_s[lS[j]]]);
            std::pair<int, int> pp2(tour[leaves_s[lS[k]]], tour[leaves_s[lS[l]]]);
            std::pair<std::pair<int, int>, std::pair<int, int> > ppp2(pp1, pp2);
            paths.push_back(ppp2);
          }
        }
      }
    }

  }

  int most_recent_common_ancestor(int i, int j) {
    int depthindex = _st_depth.get_min(i, j);
    return _int_tour[depthindex];
  }

  int most_recent_common_depth(int i, int j) {
    auto depthindex = _st_depth.get_min(i, j);
    return _depth_mapy[depthindex];
  }


  bool are_disjoint(int i, int j, int k, int l, euler_tree &tree, std::vector<std::string> &lS, std::map<std::string, int> &leafmap) const {

    auto ii = lS[i];
    auto jj = lS[j];
    auto kk = lS[k];
    auto ll = lS[l];


    auto left = std::min<int>( leafmap.at(lS[i]), leafmap.at(lS[j]));
    auto right = std::max<int>( leafmap.at(lS[i]), leafmap.at(lS[j]));
    auto mrcaij = tree.most_recent_common_ancestor(left, right);

    left = std::min<int>( leafmap.at(lS[k]), leafmap.at(lS[l]));
    right = std::max<int>( leafmap.at(lS[k]), leafmap.at(lS[l]));
    auto mrcakl = tree.most_recent_common_ancestor(left, right);

    left = std::min<int>( leafmap.at(lS[j]), leafmap.at(lS[k]));
    right = std::max<int>( leafmap.at(lS[j]), leafmap.at(lS[k]));
    auto mrcajk = tree.most_recent_common_ancestor(left, right);

    left = std::min<int>( leafmap.at(lS[i]), leafmap.at(lS[k]));
    right = std::max<int>( leafmap.at(lS[i]), leafmap.at(lS[k]));
    auto mrcaik = tree.most_recent_common_ancestor(left, right);

    left = std::min<int>( leafmap.at(lS[i]), leafmap.at(lS[l]));
    right = std::max<int>( leafmap.at(lS[i]), leafmap.at(lS[l]));
    auto mrcail = tree.most_recent_common_ancestor(left, right);

    left = std::min<int>( leafmap.at(lS[j]), leafmap.at(lS[l]));
    right = std::max<int>( leafmap.at(lS[j]), leafmap.at(lS[l]));
    auto mrcajl = tree.most_recent_common_ancestor(left, right);


    left = std::min<int>( leafmap.at(lS[i]), leafmap.at(lS[j]));
    right = std::max<int>( leafmap.at(lS[i]), leafmap.at(lS[j]));
    auto mrcaijd = tree.most_recent_common_depth(left, right);

    left = std::min<int>( leafmap.at(lS[k]), leafmap.at(lS[l]));
    right = std::max<int>( leafmap.at(lS[k]), leafmap.at(lS[l]));
    auto mrcakld = tree.most_recent_common_depth(left, right);

    left = std::min<int>( leafmap.at(lS[j]), leafmap.at(lS[k]));
    right = std::max<int>( leafmap.at(lS[j]), leafmap.at(lS[k]));
    auto mrcajkd = tree.most_recent_common_depth(left, right);

    left = std::min<int>( leafmap.at(lS[i]), leafmap.at(lS[k]));
    right = std::max<int>( leafmap.at(lS[i]), leafmap.at(lS[k]));
    auto mrcaikd = tree.most_recent_common_depth(left, right);

    left = std::min<int>( leafmap.at(lS[i]), leafmap.at(lS[l]));
    right = std::max<int>( leafmap.at(lS[i]), leafmap.at(lS[l]));
    auto mrcaild = tree.most_recent_common_depth(left, right);

    left = std::min<int>( leafmap.at(lS[j]), leafmap.at(lS[l]));
    right = std::max<int>( leafmap.at(lS[j]), leafmap.at(lS[l]));
    auto mrcajld = tree.most_recent_common_depth(left, right);

    if(mrcaijd < mrcakld) {
      auto tmp = mrcaij;
      auto tmpd = mrcaijd;
      mrcaij = mrcakl;
    }
    if(mrcaij != mrcajl
        && mrcaij != mrcajk
        && mrcaij != mrcail
        && mrcaij !=mrcaik) {
      return true;
    } else {
      return false;
    }
  }

  std::map<std::string, int>

  print_leaf_map() const {
    return _leaf_indicies;
  };

};

euler_tree::euler_tree(const tree &tree) : _internals(std::make_shared<impl>(tree)) {}

void euler_tree::incompatible_triples(const tree &tree, std::vector<std::pair<int, std::pair<int, int>>> &triples) {
  return _internals->incompatible_triples(tree, triples);
}

std::map<std::string, int> euler_tree::leaf_map() const {
  return _internals->print_leaf_map();
}

void euler_tree::incompatible_paths(const tree &tree,
                                    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &triples) {
  return _internals->incompatible_paths(tree, triples);

}

void euler_tree::topath(std::pair<int, std::pair<int, int>> &triple, std::set<std::set<int>> &edges) {

  _internals->topath(triple, edges);

}

void euler_tree::topath(std::pair<std::pair<int, int>, std::pair<int, int>> &path, std::set<std::set<int>> &edges) {

  _internals->topath(path, edges);

}

int euler_tree::most_recent_common_ancestor(int i, int j) {
  return _internals->most_recent_common_ancestor(i, j);
}

int euler_tree::most_recent_common_depth(int i, int j) {
  return _internals->most_recent_common_depth(i, j);
}
