//
//#include <iostream>
//#include "../src/phylotree/tree.h"
//#include "../src/newick/parser.h"
//#include "../src/phylotree/sparse_table.h"
//#include "catch.h"
//#include "../src/phylotree/euler_tree.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <glpk.h>
//#include <cstring>
//void TEST_GLPK_PARAMETER(string tree1, string tree2, int spr,int parameter, bool verbose);
//void TEST_GLPK_EXPERIMENT(string tree1, string tree2, int spr);
//
//
//TEST_CASE("EXPERIMENTAL GLPK Tests", "glpk") {
//  // Register the function as a benchmark
//
//  TEST_GLPK_PARAMETER("(((x,((b1,b3),b2)),y),(f,(a,c)));","(((x,y),f),((a,((b1,b2),b3)),c));", 4, 3, false);
//  TEST_GLPK_PARAMETER("(((x,((b1,b3),b2)),y),(f,(a,c)));","(((x,y),f),((a,((b1,b2),b3)),c));", 1, 0, false);
//}
//
//
//
//
//
//
//void TEST_GLPK_PARAMETER(string tree1, string tree2, int param, int spr, bool verbose) {
//  lexer lex1(tree1);
//  lexer lex2(tree2);
//  parser parser1(lex1);
//  parser parser2(lex2);
//  auto t1 = parser1.parse();
//  auto t2 = parser2.parse();
//
//  auto tour1 = euler_tree(*t1);
//  auto tour2 = euler_tree(*t2);
//  auto its = tour1.incompatible_triples(*t2);
//  auto ips = tour1.incompatible_paths(*t2);
//
//  std::set<std::set<int>> all_edge;
//  t1->all_edges_int(all_edge);
//
//  auto mip = glp_create_prob();
//  glp_set_prob_name(mip, "test");
//  glp_set_obj_dir(mip, GLP_MIN);
//
//  auto num_rows = ips.size() + its.size() + 1;//1 for param
//
//  glp_add_rows(mip, num_rows);
//
//  int count = 0;
//  for (auto t: ips) {
//    count++;
//    glp_set_row_name(mip, count, "P");
//    glp_set_row_bnds(mip, count, GLP_LO, 1.0, 0.0);
//  }
//  for (auto t:its) {
//    count++;
//    glp_set_row_name(mip, count, "T");
//    glp_set_row_bnds(mip, count, GLP_LO, 1.0, 0.0);
//  }
//  count++;
//  glp_set_row_name(mip, count, "P");
//  glp_set_row_bnds(mip, count, GLP_UP, 0.0,param);
//
//  auto num_cols = all_edge.size();
//
//  glp_add_cols(mip, num_cols);
//
//  count = 0;
//  for (auto aSet: all_edge) {
//    count++;
//
//    string name = "S";
//
//    for (auto const &e : aSet) {
//      if (std::to_string(e) == "-2") {
//        name += "i";
//        name += 't';
//      } else if (std::to_string(e) == "-1") {
//        name += "r";
//        name += 't';
//      } else {
//        name += std::to_string(e);
//        name += 't';
//      }
//
//    }
//    name.pop_back();
//    int namelen = name.length();
//    char str_as_array[namelen + 1];
//    strcpy(str_as_array, name.c_str());
//
//    glp_set_col_kind(mip, count, GLP_BV);
//    glp_set_col_name(mip, count, str_as_array);
//    glp_set_obj_coef(mip, count, 1);
//  }
//
//
//  static int* ia;
//  ia = (int*)malloc(5000000 * sizeof(int));
//  static int* ja;
//  ja = (int*)malloc(5000000 * sizeof(int));
//  static double* ar;
//  ar = (double*)malloc(5000000 * sizeof(double));
//
//  int cur_row = 0;
//  int i = 0;
//  if (ips.size() > 0) {
//    auto ipsgroup = ips.begin();
//    auto ipsiter = ips.begin()->begin();
//
//    auto edgeiter = all_edge.begin();
//
//    while (ipsgroup != ips.end()) {
//      cur_row++;
//      int cur_col = 0;
//      while (edgeiter != all_edge.end()) {
//        i++;
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        if (*ipsiter == *edgeiter) {
//          edgeiter++;
//          ipsiter++;
//          ar[i] = 1;
//        } else {
//          edgeiter++;
//          ar[i] = 0;
//        }
//      }
//      ipsgroup++;
//      ipsiter = ipsgroup->begin();
//      edgeiter = all_edge.begin();
//    }
//
//  }
//  if (its.size() > 0) {
//    auto edgeiter = all_edge.begin();
//    auto itsgroup = its.begin();
//    auto itsiter = its.begin()->begin();
//
//    while (itsgroup != its.end()) {
//      cur_row++;
//      int cur_col = 0;
//      while (edgeiter != all_edge.end()) {
//        i++;
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        if (*itsiter == *edgeiter) {
//          edgeiter++;
//          itsiter++;
//          ar[i] = 1;
//        } else {
//          edgeiter++;
//          ar[i] = 0;
//        }
//      }
//      itsgroup++;
//      itsiter = itsgroup->begin();
//      edgeiter = all_edge.begin();
//    }
//  }
//
//  cur_row++;
//  int cur_col = 0;
//
//  for (int c = 0; c < all_edge.size(); c++) {
//
//    cur_col++;
//    i++;
//    ia[i] = cur_row;
//    ja[i] = cur_col;
//    ar[i] = 1;
//
//
//  }
//  cur_col = 0;
//
//
//  glp_load_matrix(mip, num_cols * num_rows, ia, ja, ar);
//  int suc = glp_write_lp(mip, NULL, "/home/jackie/sto/prj/code/constraint_spr/cmake-build-debug/prob.txt");
//  glp_simplex(mip, NULL);
//  glp_intopt(mip, NULL);
//
//  int sprdist = 0;
//  int count2 = 0;
//  printf("::  ");
//  for (int j = 1; j <= all_edge.size(); j++) {
//    auto nme = glp_get_col_name(mip, j);
//    printf("%1s   ", nme);
//
//  }
//
//  printf("\n");
//  printf("::  ");
//  for (int j = 1; j <= all_edge.size(); j++) {
//    auto x1 = glp_mip_col_val(mip, j);
//    printf("%d      ", (int) x1);
//  }
//
//  if(verbose) {
//    printf("\n");
//
//    for (int i = 1; i <= its.size() + ips.size() + 1; i++) {
//      auto rnme = glp_get_row_name(mip, i);
//      printf("%s : ", rnme);
//      for (int j = 1; j <= all_edge.size(); j++) {
//        printf("%d      ", (int) ar[count2]);
//        count2++;
//      }
//      printf("\n");
//    }
//
//
//  }
//
//
//  REQUIRE(glp_mip_obj_val(mip) == spr);
//  glp_delete_prob(mip);
//
//}
//
///*This formulation does not work because all paths in S do not have to have the same number of cuts in T*/
//void TEST_GLPK_EXPERIMENT(string tree1, string tree2, int spr) {
//  lexer lex1(tree1);
//  lexer lex2(tree2);
//  parser parser1(lex1);
//  parser parser2(lex2);
//  auto t1 = parser1.parse();
//  auto t2 = parser2.parse();
//
//  auto tour1 = euler_tree(*t1);
//  auto tour2 = euler_tree(*t2);
//
//  auto its_S = tour1.incompatible_triples(*t2);
//  auto its_T = tour2.incompatible_triples(*t1);
//  auto ppairs_s = tour1.all_edge_pairs();
//  auto ppairs_t = tour2.all_edge_pairs();
//
//  auto incd_s = tour1.leaf_incident_edges();
//  auto incd_t = tour2.leaf_incident_edges();
//
//  std::set<std::set<int>> all_edges_in_s;
//  t1->all_edges_int(all_edges_in_s);
//
//  std::set<std::set<int>> all_edges_in_t;
//  t2->all_edges_int(all_edges_in_t);
//
//  auto mip = glp_create_prob();
//  glp_set_prob_name(mip, "test");
//  glp_set_obj_dir(mip, GLP_MIN);
//
//  auto num_rows = 2 * its_S.size() + 2 * ppairs_s.size() + 2; // leaves choose 2 size
////  auto num_rows = 2 * its_S.size() + 4 * ppairs_s.size() + 2; // leaves choose 2 size
//
//  glp_add_rows(mip, num_rows);
//
//  int count = 0;
//  for (auto t:its_S) {
//    count++;
//    glp_set_row_name(mip, count, "triple_const_s: ");
//    glp_set_row_bnds(mip, count, GLP_LO, 1.0, 0.0);
//  }
//
//  for (auto t:its_T) {
//    count++;
//    glp_set_row_name(mip, count, "triple_const_t: ");
//    glp_set_row_bnds(mip, count, GLP_LO, 1.0, 0.0);
//  }
//
//  for (auto t:ppairs_s) {
//    count++;
//    glp_set_row_name(mip, count, "S_pair_const:   ");
//    glp_set_row_bnds(mip, count, GLP_FR, 0.0, 0.0);
//  }
//
//  for (auto t:ppairs_t) {
//    count++;
//    glp_set_row_name(mip, count, "T_pair_const:   ");
//    glp_set_row_bnds(mip, count, GLP_FX, 0.0, 0.0); //upper bound ignored
//  }
//
//
//
//  count++;
//  glp_set_row_name(mip, count, "P_trip_SUM:     ");
//  glp_set_row_bnds(mip, count, GLP_LO, 0, 0.0); //upper bound ignored from the number of trips
//
//  count++;
//  glp_set_row_name(mip, count, "T_S_diff:       ");
//  glp_set_row_bnds(mip, count, GLP_FX, 0.0, 0.0); //upper bound ignored //oth must have smae num paths
//
//
//  auto num_cols = all_edges_in_s.size() + all_edges_in_t.size() + ppairs_s.size();
//
//  glp_add_cols(mip, num_cols);
//
//  count = 0;
//  for (auto aSet: all_edges_in_s) {
//    count++;
//
//    string name = "S";
//
//    for (auto const &e : aSet) {
//      if(std::to_string(e) == "-2") {
//        name +="i";
//        name += 't';
//      } else if (std::to_string(e) == "-1"){
//        name +="r";
//        name += 't';
//      }
//      else {
//        name += std::to_string(e);
//        name += 't';
//      }
//
//    }
//
//    name.pop_back();
//
//    int namelen = name.length();
//    char str_as_array[namelen + 1];
//    strcpy(str_as_array, name.c_str());
//
//    glp_set_col_kind(mip, count, GLP_BV);
//    glp_set_col_name(mip, count, str_as_array);
//    glp_set_obj_coef(mip, count, 1);
//  }
//
//  for (auto aSet: all_edges_in_t) {
//    count++;
//
//    string name = "T";
//
//    for (auto const &e : aSet) {
//      if(std::to_string(e) == "-2") {
//        name +="i";
//        name += 't';
//      } else if (std::to_string(e) == "-1"){
//        name +="r";
//        name += 't';
//      }
//      else {
//        name += std::to_string(e);
//        name += 't';
//      }
//
//    }
//
//    name.pop_back();
//
//    int namelen = name.length();
//    char str_as_array[namelen + 1];
//    strcpy(str_as_array, name.c_str());
//
//    glp_set_col_kind(mip, count, GLP_BV);
//    glp_set_col_name(mip, count, str_as_array);
//    glp_set_obj_coef(mip, count, 1);
//  }
//
//  for (auto aSet: ppairs_s) {
//    count++;
//    string name = "P";
//
//    int namelen = name.length();
//    char str_as_array[namelen + 1];
//    strcpy(str_as_array, name.c_str());
//
//    glp_set_col_kind(mip, count, GLP_IV);
//    glp_set_col_bnds(mip, count, GLP_LO, 0.0, 0.0);
//    glp_set_col_name(mip, count, str_as_array);
//    glp_set_obj_coef(mip, count, 1);
//  }
//  static int *ia;
//  ia = (int *) malloc(50000000 * sizeof(int));
//  static int *ja;
//  ja = (int *) malloc(50000000 * sizeof(int));
//  static double *ar;
//  ar = (double *) malloc(50000000 * sizeof(double));
//
//  int cur_row = 0;
//  int cur_col = 0;
//  int i = 0;
//
//  if (its_S.size() > 0) {
//    auto edgeiter = all_edges_in_s.begin();
//    auto itsgroup = its_S.begin();
//    auto itsiter = its_S.begin()->begin();
//
//    while (itsgroup != its_S.end()) {
//      cur_row++;
//
//      while (edgeiter != all_edges_in_s.end()) {
//
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        if (*itsiter == *edgeiter) {
//          edgeiter++;
//          itsiter++;
//          ar[i] = 1;
//          i++;
//        } else {
//          edgeiter++;
//          ar[i] = 0;
//          i++;
//        }
//      }
//      itsgroup++;
//      itsiter = itsgroup->begin();
//      edgeiter = all_edges_in_s.begin();
//
//      for (int c = 0; c < (all_edges_in_t.size()); c++) {
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        ar[i] = 0;
//        i++;
//      }
//
//      for (int c = 0; c < (ppairs_s.size()); c++) {
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        ar[i] = 0;
//        i++;
//      }
//      cur_col = 0;
//    }
//  }
//
//  if (its_T.size() > 0) {
//    auto edgeitert = all_edges_in_t.begin();
//    auto itsgroupt = its_T.begin();
//    auto itsitert = its_T.begin()->begin();
//
//    while (itsgroupt != its_T.end()) {
//      cur_row++;
//
//      for (int c = 0; c < (all_edges_in_t.size()); c++) {
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        ar[i] = 0;
//        i++;
//      }
//
//      while (edgeitert != all_edges_in_t.end()) {
//
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        if (*itsitert == *edgeitert) {
//          edgeitert++;
//          itsitert++;
//          ar[i] = 1;
//          i++;
//        } else {
//          edgeitert++;
//          ar[i] = 0;
//          i++;
//        }
//      }
//      itsgroupt++;
//      itsitert = itsgroupt->begin();
//      edgeitert = all_edges_in_t.begin();
//
//
//      for (int c = 0; c < (ppairs_s.size()); c++) {
//        cur_col++;
//        ia[i] = cur_row;
//        ja[i] = cur_col;
//        ar[i] = 0;
//        i++;
//      }
//      cur_col = 0;
//    }
//  }
//
//
//
//  if (ppairs_s.size() > 0) {
//    int end_row_count_s = 0;
//    int end_row_count_t = 0;
//    auto all_edge_stream_s = all_edges_in_s.begin();
//    auto all_edge_stream_t = all_edges_in_t.begin();
//    auto pair_paths_it_s = ppairs_s.begin();
//    auto pair_paths_it_t = ppairs_t.begin();
//    auto pair_path_it_s = pair_paths_it_s->begin();
//    auto pair_path_it_t = pair_paths_it_t->begin();
//
//
//    for (int row = 0; row < ppairs_s.size() * 2; row++) {
//      cur_row++;
//      cur_col = 0;
//      if (cur_row > 2 * its_S.size()
//          && cur_row <= 2 * its_S.size() + ppairs_s.size()) {
//        while (all_edge_stream_s != all_edges_in_s.end()) {
//
//          cur_col++;
//          ia[i] = cur_row;
//          ja[i] = cur_col;
//          if (*pair_path_it_s == *all_edge_stream_s) {
//            all_edge_stream_s++;
//            pair_path_it_s++;
//            ar[i] = 1;
//            i++;
//          } else {
//            all_edge_stream_s++;
//            ar[i] = 0;
//            i++;
//          }
//        }
//
//        pair_paths_it_s++;
//        pair_path_it_s = pair_paths_it_s->begin();
//        all_edge_stream_s = all_edges_in_s.begin();
//
//        for (int c = 0; c < (all_edges_in_t.size()); c++) {
//          cur_col++;
//          ia[i] = cur_row;
//          ja[i] = cur_col;
//          ar[i] = 0;
//          i++;
//        }
//
//      } else { //otherwise do the ts
//
//        for (int c = 0; c < (all_edges_in_s.size()); c++) {
//          cur_col++;
//          ia[i] = cur_row;
//          ja[i] = cur_col;
//          ar[i] = 0;
//          i++;
//        }
//
//        while (all_edge_stream_t != all_edges_in_t.end()) {
//
//          cur_col++;
//          ia[i] = cur_row;
//          ja[i] = cur_col;
//          if (*pair_path_it_t == *all_edge_stream_t) {
//            all_edge_stream_t++;
//            pair_path_it_t++;
//            ar[i] = 1;
//            i++;
//          } else {
//            all_edge_stream_t++;
//            ar[i] = 0;
//            i++;
//          }
//        }
//
//        pair_paths_it_t++;
//        pair_path_it_t = pair_paths_it_t->begin();
//        all_edge_stream_t = all_edges_in_t.begin();
//      }
//
//      if (cur_row > 2 * its_S.size()
//          && cur_row <= 2 * its_S.size() + ppairs_s.size()) {
//
//
//
//        for (int c = 0; c < ppairs_s.size(); c++) {
//          cur_col++;
//          ia[i] = cur_row;
//          ja[i] = cur_col;
//          if (end_row_count_s == c) {
//            ar[i] = -1;
//            i++;////////////////////////////////////////////
//          } else {
//            ar[i] = 0;
//            i++;////////////////////////////////////////////
//          }
//        }
//        end_row_count_s++;
//
//      } else {
//
//        for (int c = 0; c < ppairs_t.size(); c++) {
//          cur_col++;
//          ia[i] = cur_row;
//          ja[i] = cur_col;
//          if (end_row_count_t == c) {
//            ar[i] = -1;
//            i++;////////////////////////////////////////////
//          } else {
//            ar[i] = 0;
//            i++;////////////////////////////////////////////
//          }
//        }
//        end_row_count_t++;
//
//      }
//
//    }
//
//  }
//  cur_col = 0;
//
//
//  //sum of paths cut
//
//  if (ppairs_s.size() > 0) { // change conditions
//
//    cur_row++;
//
//    for (int c = 0; c < all_edges_in_s.size() + all_edges_in_t.size(); c++) {
//
//      cur_col++;
//      ia[i] = cur_row;
//      ja[i] = cur_col;
//      ar[i] = 0;
//      i++;
//
//    }
//
//
//
//    for (int c = 0; c < (ppairs_s.size()); c++) {
//      cur_col++;
//      ia[i] = cur_row;
//      ja[i] = cur_col;
//      ar[i] = 1;
//      i++;
//    }
//    cur_col = 0;
//
//  }
//
//
//  glp_load_matrix(mip, i - 1, ia, ja, ar);
//  int suc = glp_write_lp(mip, NULL, "/home/jackie/sto/prj/code/cspr_e_gec_glpk/prob.txt");
//  int suc2 = glp_write_sol(mip, "/home/jackie/sto/prj/code/cspr_e_gec_glpk/sol.txt");
//  glp_simplex(mip, NULL);
//  glp_intopt(mip, NULL);
//
//  int sprdist = 0;
//  int count2 = 0;
//  printf("::                 ");
//  for (int j = 1; j <= all_edges_in_s.size() + all_edges_in_t.size() + ppairs_s.size(); j++) {
//    auto nme = glp_get_col_name(mip, j);
//
//    if (nme[0] == 'P' || nme[0] == 'M')
//      printf("%1s      ", nme);
//    else
//
//
//
//      printf("%1s   ", nme);
//
//  }
//
//  printf("\n");
//  printf("::                  ");
//  for (int j = 1; j <= all_edges_in_s.size() + all_edges_in_t.size() + ppairs_s.size(); j++) {
//    auto x1 = glp_mip_col_val(mip, j);
//    printf("%d      ", (int) x1);
//    if(*glp_get_col_name(mip,j)=='S')
//      sprdist+=x1;
//  }
//
//  printf("\n");
//
//  for (int i = 1; i <=  2 * its_S.size() + 2 * ppairs_s.size() + 2; i++) {
//    auto rnme = glp_get_row_name(mip, i);
//    printf("%s : ", rnme);
//    for (int j = 1; j <= all_edges_in_s.size() + all_edges_in_t.size() + ppairs_s.size(); j++) {
//
//      if (j == -1) {
//
//        auto nme = glp_get_col_name(mip, j+2);
//        printf("%s  ", nme);
//      }
//      else if (j == 0) {
//        auto x1 = glp_mip_col_val(mip, j+1);
//        printf("%.1f ", x1);
//      }
//      else {
//        printf("%d      ", (int) ar[count2]);
//        count2++;
//      }
//
//    }
//    printf("\n");
//  }
//
//  map<string, int>::iterator it;
//
//  REQUIRE(sprdist == spr);
//
//  glp_delete_prob(mip);
//
//}
//