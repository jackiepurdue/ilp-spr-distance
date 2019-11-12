//
// Created by jackie on 28/02/18.
//

#ifndef CSPR_TOKEN_H
#define CSPR_TOKEN_H

#include <string>

using namespace std;

class token {


public:
    enum class State { startState, numberState, stringState, commentState};
    enum class token_type_enum { L_BRACKET_TOKEN, R_BRACKET_TOKEN, COLON_TOKEN, SEMI_COLON_TOKEN,
        COMMA_TOKEN, STRING_TOKEN, NUMBER_TOKEN, EOF_TOKEN };
    token(token_type_enum tokenType, string stringValue);
    token_type_enum token_type();
    bool isEndOfFileToken();
    bool same_token_type(const token &token);
    bool same_value(const token &t);
    bool isEqual(const token &token);
    string getValue();
private:
    string _value;
    token_type_enum _token_type;
};

#endif
