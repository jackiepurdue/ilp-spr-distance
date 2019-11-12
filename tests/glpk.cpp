#define CATCH_CONFIG_MAIN

#include <iostream>
#include "../src/phylotree/tree.h"
#include "../src/phylotree/sparse_table.h"
#include "catch.h"
#include "../src/glpk/param.h"
#include "../src/glpk/orig.h"


//TODO:: Make sure no infinite loops:: handle all parsing errors
//TODO:: Refactor code/tests
//TODO:: invalid reads from valgrind


void REQUIRE_GLPK_PARAM(std::string s, std::string t, int parama, int ans, bool verbose) {


  auto sprparam = param(s, t, parama, false);
  sprparam.formulate();

  auto sprdist = sprparam.solve(parama);
  REQUIRE(sprdist == ans);

  sprparam.delete_problem();
}

void REQUIRE_GLPK_NORMAL(std::string s, std::string t, int ans, bool verbose) {
  auto sprparam = orig(s, t, false);
  sprparam.formulate();
  int sprdist = sprparam.solve();
  REQUIRE(sprdist == ans);
  sprparam.delete_problem();
}

TEST_CASE("GLPK Tests", "glpk") {


  REQUIRE_GLPK_PARAM("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                     "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 4, 4, false);

  REQUIRE_GLPK_NORMAL(
      "((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                      "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));"
      , 4, false);

  REQUIRE_GLPK_PARAM("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                     "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 4, 4, false);
  REQUIRE_GLPK_PARAM("(((x,((b1,b3),b2)),y),(f,(a,c)));",
                     "(((x,y),f),((a,((b1,b2),b3)),c));", 3, 3, false);
  REQUIRE_GLPK_PARAM("(A:1,(C:2,D:3));", "((A:1,C:2),D:3);", 1, 1, false);
  REQUIRE_GLPK_PARAM("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                     "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 4, 4, false);
  REQUIRE_GLPK_PARAM("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                     "((((7,8),((2,(11,1)),(3,4))),(6,5)),((12,(10,9)),((14,13),(15,16))));", 2, 2, false);
  REQUIRE_GLPK_PARAM("(A,(B,(C,(D,E))));", "(E,(D,(C,(A,B))));", 3, 3, true);
  REQUIRE_GLPK_PARAM("(A:1,(C:2,D:3));", "((A:1,C:2),D:3);", 1, 1, false);
  REQUIRE_GLPK_PARAM("((((A,B),C),D),E);", "((B,(C,(A,D))),E);", 2, 2, false);

  REQUIRE_GLPK_NORMAL("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                      "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 4, false);
  REQUIRE_GLPK_NORMAL("(((x,((b1,b3),b2)),y),(f,(a,c)));",
                      "(((x,y),f),((a,((b1,b2),b3)),c));", 3, false);
  REQUIRE_GLPK_NORMAL("(A:1,(C:2,D:3));", "((A:1,C:2),D:3);", 1, false);
  REQUIRE_GLPK_NORMAL("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                      "((((2,1),(((11,12),4),(8,(3,(6,5))))),7),((14,(10,9)),(13,(15,16))));", 4, false);
  REQUIRE_GLPK_NORMAL("((((1,2),(3,4)),((5,6),(7,8))),(((9,10),(11,12)),((13,14),(15,16))));",
                      "((((7,8),((2,(11,1)),(3,4))),(6,5)),((12,(10,9)),((14,13),(15,16))));", 2, false);

  REQUIRE_GLPK_NORMAL("(A,(B,(C,(D,E))));", "(E,(D,(C,(A,B))));", 3, true);
  REQUIRE_GLPK_NORMAL("(A:1,(C:2,D:3));", "((A:1,C:2),D:3);", 1, false);
  REQUIRE_GLPK_NORMAL("((((A,B),C),D),E);", "((B,(C,(A,D))),E);", 2, false);

}