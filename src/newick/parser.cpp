#include "parser.h"
#include "../phylotree/tree.h"
#include <iostream>
struct parser::impl {

  impl(lexer newick_lexer)
      : _newick_lexer(newick_lexer), _lookahead(token::token_type_enum::EOF_TOKEN, "") {}
  lexer _newick_lexer;
  token _lookahead;

  void update_lookahead(token::token_type_enum type) {
    if (_lookahead.token_type() != type)
      throw invalid_argument("");
    update_lookahead();
  }

  void update_lookahead() {
    _lookahead = *_newick_lexer.scan();
  }

  std::string produce_label() {
    if (_lookahead.token_type() == token::token_type_enum::COLON_TOKEN) {
      return "unlabeled_leaf"; // no label
    } else {
      return _lookahead.getValue();
    }
  }

  std::string produce_length() {
    if (_lookahead.token_type() == token::token_type_enum::STRING_TOKEN || _lookahead.token_type() == token::token_type_enum::NUMBER_TOKEN) {
      update_lookahead();
    }
    if (_lookahead.token_type() == token::token_type_enum::COLON_TOKEN) {
      update_lookahead(token::token_type_enum::COLON_TOKEN);
      return _lookahead.getValue();
    } else
      return "";
  }

  std::vector<shared_ptr<tree>> produce_children(int *tag) {
    vector<shared_ptr<tree>> v;
    (*tag)++;
    v.push_back(produce_subtree(tag));

    //  auto t1 = produce_subtree();
    try {
      if (_lookahead.token_type() != token::token_type_enum::R_BRACKET_TOKEN) {
        if (_lookahead.token_type() != token::token_type_enum::COMMA_TOKEN)//to keep it if no number
          update_lookahead();
        update_lookahead(token::token_type_enum::COMMA_TOKEN);
        (*tag)++;
        v.push_back(produce_subtree(tag));
      }
      //trees = new tree_pair(lc, rc);
      if (_lookahead.token_type() == token::token_type_enum::STRING_TOKEN || _lookahead.token_type() == token::token_type_enum::NUMBER_TOKEN)
        update_lookahead();
      return v;
    }
    catch (invalid_argument) {
      throw;
    }
  }

  shared_ptr<tree> produce_subtree(int *tag) {
    int saved_tag = *tag;
    shared_ptr<tree> left;
    shared_ptr<tree> right;
    shared_ptr<tree> final;
    std::string label = "";
    std::string length = "";
    if (_lookahead.token_type() == token::token_type_enum::L_BRACKET_TOKEN) {
      update_lookahead(token::token_type_enum::L_BRACKET_TOKEN);
      std::vector<shared_ptr<tree>> children = produce_children(tag);
      update_lookahead(token::token_type_enum::R_BRACKET_TOKEN);
      label = "";

      left = children.at(0);
      if (left.get() != nullptr)
        label += left.get()->root();

      right = children.at(1);
      label = label + "_";
      if (right.get() != nullptr)
        label = label + right.get()->root();
    } else if (_lookahead.token_type() == token::token_type_enum::STRING_TOKEN || _lookahead.token_type() == token::token_type_enum::COLON_TOKEN
        || _lookahead.token_type() == token::token_type_enum::NUMBER_TOKEN) {
      label = produce_label();
    } else if (_lookahead.token_type() == token::token_type_enum::COMMA_TOKEN) {
      //this is for no labels fall through
    } else if (_lookahead.token_type() == token::token_type_enum::R_BRACKET_TOKEN) {
      throw invalid_argument("");
    }

    length = produce_length();
    if (left.get() == nullptr && right.get() == nullptr)
      return make_shared<tree>(label, tree(), tree(), saved_tag);
    else if (left.get() == nullptr && right.get() != nullptr)
      return make_shared<tree>(label, tree(), *(right.get()), saved_tag);
    else if (left.get() != nullptr && right.get() == nullptr)
      return make_shared<tree>(label, *(left.get()), tree(), saved_tag);
    else if (left.get() != nullptr && right.get() != nullptr)
      return make_shared<tree>(label, *(left.get()), *(right.get()), saved_tag);
  }

};

parser::parser(const lexer &newick_lexer)
    : _internals(std::make_shared<impl>(newick_lexer)) {}

shared_ptr<tree> parser::parse() {

  _internals->update_lookahead();
  if (_internals->_lookahead.token_type() == token::token_type_enum::EOF_TOKEN)
    return nullptr;
  int start_tag = 0;
  auto rooted_tree = make_shared<tree>("i", tree("_", tree(),tree(),-1), *(_internals->produce_subtree(&start_tag)), -2);
  try {
    //_internals->update_lookahead(Token::TokenType::semiColonToken);
    return rooted_tree;
  }
  catch (std::invalid_argument) {
    throw;
  }
}
