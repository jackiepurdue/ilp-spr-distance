#include "lexer.h"
#include "token.h"
#include <iostream>
#include <vector>


lexer::lexer(string nw_string) {
    this->_nw_string = nw_string;
    this->string_iterator = (this->_nw_string).begin();
}

unique_ptr<token>  lexer::scan() {

    state current_state = lexer::state::START_STATE;
    string str = "";
    while (string_iterator != _nw_string.end()) {
        switch (current_state) {
            case state::START_STATE:
                if (isspace(*string_iterator)) break;
                else if (isalpha(*string_iterator) || (*string_iterator) == '_')
                    current_state = lexer::state::STRING_STATE;
                else if (isdigit(*string_iterator) || (*string_iterator) == '.')
                    current_state = lexer::state::NUMBER_STATE;
                else {
                    switch (*string_iterator) {
                        case '(':
                            ++string_iterator;
                            return unique_ptr<token> {new token(token::token_type_enum::L_BRACKET_TOKEN, "(")};
                        case ')':
                            ++string_iterator;
                            return unique_ptr<token> {new token(token::token_type_enum::R_BRACKET_TOKEN, ")")};
                        case ';':
                            ++string_iterator;
                            return unique_ptr<token> {new token(token::token_type_enum::SEMI_COLON_TOKEN, ";")};
                        case ':':
                            ++string_iterator;
                            return unique_ptr<token> {new token(token::token_type_enum::COLON_TOKEN, ":")};
                        case ',':
                            ++string_iterator;
                            return unique_ptr<token> {new token(token::token_type_enum::COMMA_TOKEN, ",")};
                        default:
                            break;
                    }
                }
                break;
            case state::STRING_STATE:
                str += *string_iterator;
                ++string_iterator;
                if(!((*string_iterator) == '_' || isalpha(*string_iterator) || isdigit(*string_iterator))) {// ||
                    current_state = state::START_STATE;
                    return unique_ptr<token> {new token(token::token_type_enum::STRING_TOKEN, str)};
                }
                break;
            case state::NUMBER_STATE:
                str += *string_iterator;
                ++string_iterator;
                if(!((*string_iterator) == '.' || isdigit(*string_iterator))) {// ||
                    current_state = state::START_STATE;
                    return unique_ptr<token> {new token(token::token_type_enum::NUMBER_TOKEN, str)};
                }
                break;
        }
    }

    return unique_ptr<token> {new token(token::token_type_enum::EOF_TOKEN, "<EOF>")};
}