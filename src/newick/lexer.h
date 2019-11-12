#ifndef CSPR_LEXER_H
#define CSPR_LEXER_H

#include <string>
#include <bits/unique_ptr.h>
#include "token.h"

using namespace std;

class lexer {
public:
    lexer(string nw_string);
    unique_ptr<token> scan();
    enum class state {
        START_STATE, NUMBER_STATE, STRING_STATE, commentState
    };

private:
    string _nw_string;
    string::iterator string_iterator;
};


#endif
