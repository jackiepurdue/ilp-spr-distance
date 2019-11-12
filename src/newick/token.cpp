//
// Created by jackie on 28/02/18.
//

#define stringify( name ) # name

#include <iostream>
#include "token.h"

token::token(token_type_enum tokenType, string stringValue) {
    this->_token_type = tokenType;
    this->_value = stringValue;
}

bool token::isEndOfFileToken() {
    return _token_type == token::token_type_enum::EOF_TOKEN;
}

bool token::same_token_type(const token &token) {
    return (token._token_type == this->_token_type);
}

bool token::same_value(const token &t) {
    return (t._value == this->_value);
}

bool token::isEqual(const token &token) {
    return same_token_type(token) && same_value(token);
}

token::token_type_enum token::token_type(){
    return this->_token_type;
}

string token::getValue(){
    return this->_value;
}