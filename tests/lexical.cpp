#include <iostream>
#include <string>
#include "../src/newick/lexer.h"
#include "../src/newick/parser.h"
#include "catch.h"
void REQUIRE_PARSE_NOTHROW(std::string s);
void REQUIRE_PARSE_THROWS(std::string s);

TEST_CASE("Parsing", "parse") {

  REQUIRE_PARSE_NOTHROW("((((((((T22,T5),(T29,T39)),T14),T25),((((T3,T36),T31),(((((T2,T8),T23),T32),((T33,T37),T24)),((T19,T7),((((T15,T18),(T30,T34)),(((((T35,T4),T16),((T20,T26),((T17,T21),T6))),(((T38,T9),T13),T28)),T27)),T11)))),T10)),T1),T12),T0);");
  REQUIRE_PARSE_NOTHROW("(A:1,(C:2,D:3));");
  REQUIRE_PARSE_NOTHROW("(A:1,(C:2,D:3));");
  REQUIRE_PARSE_NOTHROW("(A:0.1,(C:0.3,D:0.4):0.5);");
  REQUIRE_PARSE_NOTHROW("(:0.1,(:0.3,:0.4):0.5);");
  REQUIRE_PARSE_NOTHROW("(A,(C,D));");
  REQUIRE_PARSE_NOTHROW("(A,(C,D)E)F;");
  REQUIRE_PARSE_NOTHROW("(JACKIE:112.12,(WAS:2.12,HERE:3.12312));");
  REQUIRE_PARSE_NOTHROW("(A,(C,D:1.1)E)F;");
  REQUIRE_PARSE_NOTHROW("(A:2,(:2,D:1.1)E)F;");
  REQUIRE_PARSE_NOTHROW("(((sea_lion:11.99700,seal:12.00300):7.52973,"
                        "((monkey:100.85930,cat:47.14069):20.59201,weasel:18.87953):2.09460):3.87382,dog:25.46154);");


  REQUIRE_PARSE_THROWS(")(");
}

TEST_CASE("Lexing (,,(,);", "lex") {

  lexer test("(,,(,);");

  auto t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);
  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);
}

TEST_CASE("Lexing (name_a,name_b,(name_c,name_d)name_e)name_f;", "lex") {

  //make these tests smarter
  lexer test("(name_a,name_b,(name_c,name_d)name_e)name_f;");
  auto t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::SEMI_COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

}

TEST_CASE("Lexing (:000.123,:0.1,(:0.12,:0.10):0.10);", "lex") {

  lexer test("(:000.123,:0.1,(:0.12,:0.10):0.10);");

  auto t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::SEMI_COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);
}

TEST_CASE("Lexing ((name_b:0.10,(name_c:0.10,name_d:0.10)name_e:0.10)name_f:0.10)name_a;", "lex") {

  lexer test("((name_b:0.10,(name_c:0.10,name_d:0.10)name_e:0.10)name_f:0.10)name_a;");

  auto t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::L_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COMMA_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::NUMBER_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::R_BRACKET_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::STRING_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::SEMI_COLON_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);
}

TEST_CASE("Lexing empty string", "lex") {

  lexer test("");

  auto t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

  t = test.scan();
  REQUIRE(t->token_type() == token::token_type_enum::EOF_TOKEN);

}

void REQUIRE_PARSE_THROWS(std::string s) {
  cout << "Asserting throw on: " << s << endl;
  lexer lexer(s);
  auto p = parser(lexer);
  REQUIRE_THROWS(p.parse());
}

void REQUIRE_PARSE_NOTHROW(std::string s) {
  cout << "Asserting no throw on: " << s << endl;
  lexer lexer(s);
  parser parser(lexer);
  // auto p = parser.parse();
  REQUIRE_NOTHROW(parser.parse());
}
