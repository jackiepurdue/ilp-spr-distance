#include "orig.h"
#include <iostream>
#include "../phylotree/tree.h"
#include "../newick/parser.h"
#include "../phylotree/sparse_table.h"
#include "../phylotree/euler_tree.h"
#include <glpk.h>
#include <cstring>

static int *ia;
static int *ja;
static double *ar;

struct orig::impl {
  //make a proper wrapper later
  std::string _s;
  std::string _t;
  bool _verbose;
  glp_prob * mip = glp_create_prob();
  int num_rows = 0;
  int num_cols = 0;

  impl(std::string tree1, std::string tree2, bool verbose)
      : _s(tree1), _t(tree2), _verbose(verbose) {
      glp_term_out(GLP_OFF);
  }

  void delete_problem() {
    delete(ia);
    delete(ja);
    delete(ar);
    glp_delete_prob(mip);
  }

  void formulate() {
    lexer lex1(_s);
    lexer lex2(_t);
    parser parser1(lex1);
    parser parser2(lex2);
    auto t1 = parser1.parse();
    auto t2 = parser2.parse();
    auto tour1 = euler_tree(*t1);
    auto tour2 = euler_tree(*t2);

    std::vector<std::pair<int, std::pair<int, int>>> its;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> ips;
    tour1.incompatible_triples(*t2, its);
    tour1.incompatible_paths(*t2, ips);

    std::set<std::set<int>> all_edge;
    t1->all_edges_int(all_edge);

    glp_set_prob_name(mip, "test");
    glp_set_obj_dir(mip, GLP_MIN);

    num_rows = ips.size() + its.size();
    auto sd = sizeof(double);
    glp_add_rows(mip, num_rows);

    int count = 0;

    for (auto t: ips) {
      count++;
      glp_set_row_name(mip, count, "P");
      glp_set_row_bnds(mip, count, GLP_LO, 1.0, 0.0);
    }
    for (auto t:its) {
      count++;
      glp_set_row_name(mip, count, "T");
      glp_set_row_bnds(mip, count, GLP_LO, 1.0, 0.0);
    }

    num_cols = all_edge.size();

    glp_add_cols(mip, num_cols);

    count = 0;
    for (auto aSet: all_edge) {
      count++;

      string name = "S";

      for (auto const &e : aSet) {
        if (std::to_string(e) == "-2") {
          name += "i";
          name += 't';
        } else if (std::to_string(e) == "-1") {
          name += "r";
          name += 't';
        } else {
          name += std::to_string(e);
          name += 't';
        }

      }
      name.pop_back();
      int namelen = name.length();
      char str_as_array[namelen + 1];
      strcpy(str_as_array, name.c_str());

      glp_set_col_kind(mip, count, GLP_BV);
      glp_set_col_name(mip, count, str_as_array);
      glp_set_obj_coef(mip, count, 1);
    }



    ia = new int[num_rows*num_cols*2];

    ja =new int[num_rows*num_cols*2];

    ar = new double[num_rows*num_cols*2];

    int cur_row = 0;
    int i = 0;
    if (ips.size() > 0) {
      auto ipsgroup = ips.begin();


      auto edgeiter = all_edge.begin();

      while (ipsgroup != ips.end()) {

        std::set<std::set<int>> ipspath;
        tour1.topath(*ipsgroup, ipspath);

        cur_row++;
        int cur_col = 0;
        auto ipsiter = ipspath.begin();
        while (edgeiter != all_edge.end()) {
          i++;
          cur_col++;
          ia[i] = cur_row;
          ja[i] = cur_col;
          if (*ipsiter == *edgeiter) {
            edgeiter++;
            ipsiter++;
            ar[i] = 1;
          } else {
            edgeiter++;
            ar[i] = 0;
          }
        }
        ipsgroup++;
        edgeiter = all_edge.begin();
      }

    }
    if (its.size() > 0) {
      auto edgeiter = all_edge.begin();
      auto itsgroup = its.begin();

      while (itsgroup != its.end()) {
        std::set<std::set<int>> itspath;
        tour1.topath(*itsgroup, itspath);
        auto itsiter = itspath.begin();
        cur_row++;
        int cur_col = 0;
        while (edgeiter != all_edge.end()) {
          i++;
          cur_col++;
          ia[i] = cur_row;
          ja[i] = cur_col;
          if (*itsiter == *edgeiter) {
            edgeiter++;
            itsiter++;
            ar[i] = 1;
          } else {
            edgeiter++;
            ar[i] = 0;
          }
        }
        itsgroup++;
        edgeiter = all_edge.begin();
      }
    }

    cur_row++;
    int cur_col = 0;

    for (int c = 0; c < all_edge.size(); c++) {
      cur_col++;
      i++;
      ia[i] = cur_row;
      ja[i] = cur_col;
      ar[i] = 1;

    }
  }

int solve(){

  glp_load_matrix(mip, num_cols * num_rows, ia, ja, ar);

  glp_simplex(mip, NULL);
  glp_intopt(mip, NULL);
  if (_verbose) {
    int sprdist = 0;
    int count2 = 0;
    printf("::  ");
    for (int j = 1; j <= num_cols; j++) {
      auto nme = glp_get_col_name(mip, j);
      printf("%1s   ", nme);

    }

    printf("\n");
    printf("::  ");
    for (int j = 1; j <= num_cols; j++) {
      auto x1 = glp_mip_col_val(mip, j);
      sprdist += x1;
      printf("%d      ", (int) x1);
    }


    printf("\n");

    for (int i = 1; i <= num_rows - 1; i++) {//********
      auto rnme = glp_get_row_name(mip, i);
      printf("%s : ", rnme);
      for (int j = 1; j <= num_cols; j++) {
        printf("%d      ", (int) ar[count2]);
        count2++;
      }
      printf("\n");
    }


  }

    int opt = glp_mip_obj_val(mip);

  return opt;
  }
};

orig::orig(std::string tree1, std::string tree2, bool verbose):
    _internals(std::make_shared<impl>(tree1,tree2,verbose)) {}


void orig::formulate() {
  _internals->formulate();
}

int orig::solve() {
  _internals->solve();
}

void orig::delete_problem() {
  _internals->delete_problem();
}
