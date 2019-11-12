#include <iostream>
#include "../src/phylotree/tree.h"
#include "../src/newick/parser.h"
#include "../src/phylotree/sparse_table.h"
#include "catch.h"
#include "../src/phylotree/euler_tree.h"


void REQUIRE_INCOMPATIBLE_TRIPLES(string treeS,
                                  string treeT,
                                  int n) {
  lexer lexerS(treeS);
  parser parser2(lexerS);
  lexer lexerT(treeT);
  parser parserT(lexerT);
  std::vector<std::pair<int, std::pair<int,int>>> triples;
  auto p = parser2.parse();
  auto T = parserT.parse();
  auto tour1 = euler_tree(*p);
  auto tour2 = euler_tree(*T);
  tour1.incompatible_triples(*T, triples);
  REQUIRE(triples.size() == n);
}
//
void REQUIRE_INCOMPATIBLE_PATHS(string treeS,
                                string treeT,
                                int n) {
  lexer lexerS(treeS);
  parser parser2(lexerS);
  lexer lexerT(treeT);
  parser parserT(lexerT);
  std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> paths;
  auto p = parser2.parse();
  auto T = parserT.parse();
  auto tour1 = euler_tree(*p);
  auto tour2 = euler_tree(*T);
  tour1.incompatible_paths(*T, paths);
  REQUIRE(paths.size() == n);
}

TEST_CASE("Tree tests", "tree") {

  REQUIRE_INCOMPATIBLE_TRIPLES("((3,(1,2)),4);", "((1,(4,3)),2);", 4);
  REQUIRE_INCOMPATIBLE_PATHS("((3,(1,2)),4);", "((1,(4,3)),2);", 4);
  REQUIRE_INCOMPATIBLE_TRIPLES("(((1,((2,3),4)),5),(6,(7,8)));", "(((1,5),6),((7,((2,4),3)),8));", 35);

  REQUIRE_INCOMPATIBLE_PATHS("(((1,((2,3),4)),5),(6,(7,8)));", "(((1,5),6),((7,((2,4),3)),8));", 70);

  REQUIRE_INCOMPATIBLE_TRIPLES("((1,2),(3,(4,5)));", "((1,3),(4,(2,5)));", 8);
  REQUIRE_INCOMPATIBLE_PATHS("((1,2),(3,(4,5)));", "((1,3),(4,(2,5)));", 12);

  REQUIRE_INCOMPATIBLE_TRIPLES("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                               "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 193);

  REQUIRE_INCOMPATIBLE_PATHS("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                               "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 946);
}
