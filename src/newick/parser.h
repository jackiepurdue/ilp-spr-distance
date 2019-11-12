#ifndef CSPR_PARSER_H
#define CSPR_PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "../phylotree/tree.h"
#include "lexer.h"

class parser {
 public:
  parser(const lexer& newick_lexer);
  shared_ptr<tree> parse();
 private:
  struct impl;
  std::shared_ptr<impl> _internals;
};

#endif