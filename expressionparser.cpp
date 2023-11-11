#include "expressionparser.h"

#include <stack>
#include <cmath>

#define CHECK_ERROR if(hasError) return;

bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

int asDigit(char c) {
    return c - '0';
}

bool isLetter(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' &&  c <= 'Z';
}

Expression Expression::invalid = Expression(false);

ExpressionParser::ExpressionParser()
{

}

Expression ExpressionParser::parse(const std::string &str)
{
    lex(str);
    return parse();
}

void ExpressionParser::lex(const std::string &str)
{
    int pos = 0;
    while(pos < str.size()) {
        if(isSpace(str[pos])) {
            ++pos;
            continue;
        }

        if(isDigit(str[pos])) {
            double value = 0;
            while(pos < str.size() && isDigit(str[pos])) {
                value *= 10;
                value += asDigit(str[pos]);
                ++pos;
            }

            if(pos < str.size() && str[pos] == '.') {
                ++pos;
                double fraction = 0;
                while(pos < str.size() && isDigit(str[pos])) {
                    fraction /= 10;
                    fraction += asDigit(str[pos]) / 10.0;
                    ++pos;
                }

                value += fraction;
            }

            tokens.push_back(Token { TokenType::Const, value });
            continue;
        }

        if(str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || str[pos] == '/' || str[pos] == '^') {
            tokens.push_back(Token { TokenType::Operation, str[pos] });
            ++pos;
            continue;
        }

        if(str[pos] == '(') {
            tokens.push_back(Token { TokenType::Lpar, str[pos] });
            ++pos;
            continue;
        }

        if(str[pos] == ')') {
            tokens.push_back(Token { TokenType::Rpar, str[pos] });
            ++pos;
            continue;
        }

        if(isLetter(str[pos])) {
            std::string id;
            while(pos < str.size() && isLetter(str[pos])) {
                id.push_back(str[pos]);
                ++pos;
            }

            tokens.push_back(Token { TokenType::Id, id });
            continue;
        }

        ++pos;
    }
}

Expression ExpressionParser::parse()
{
    expression();
    if(hasError || position < tokens.size()) {
        return Expression::invalid;
    }

    return Expression(constants, operations);
}

void ExpressionParser::expression()
{
    if(position >= tokens.size()) {
        return;
    }
    if(tokens[position].type == TokenType::Lpar) {
        ++position;
        expression();
        CHECK_ERROR

        if(position >= tokens.size() || tokens[position].type != TokenType::Rpar) {
            error();
        }
        ++position;
        return;
    }

    term();
    CHECK_ERROR
}

void ExpressionParser::term()
{
    factor();
    CHECK_ERROR

    while(position < tokens.size() && (std::any_cast<char>(tokens[position].value) == '+' || std::any_cast<char>(tokens[position].value) == '-')){
        Operation operation;
        if(std::any_cast<char>(tokens[position].value) == '+') {
            operation = Operation::Plus;
        } else if(std::any_cast<char>(tokens[position].value) == '-') {
            operation = Operation::Minus;
        }

        ++position;
        factor();
        CHECK_ERROR

        operations.push_back(operation);
    }
}

void ExpressionParser::factor()
{
    power();
    CHECK_ERROR


    while(position < tokens.size() && (std::any_cast<char>(tokens[position].value) == '*' || std::any_cast<char>(tokens[position].value) == '/')){
        Operation operation;
        if(std::any_cast<char>(tokens[position].value) == '*') {
            operation = Operation::Multiply;
        } else if(std::any_cast<char>(tokens[position].value) == '/') {
            operation = Operation::Divide;
        }

        ++position;
        power();
        CHECK_ERROR

        operations.push_back(operation);
    }
}

void ExpressionParser::power()
{
    primary();
    CHECK_ERROR

    if(position >= tokens.size()) {
        return;
    }

    while(position < tokens.size() && std::any_cast<char>(tokens[position].value) == '^'){
        Operation operation = Operation::Power;
        ++position;
        primary();

        operations.push_back(operation);
    }
}

void ExpressionParser::primary()
{
    if(position >= tokens.size()) {
        error();
        return;
    }
    if(tokens[position].type == TokenType::Operation && std::any_cast<char>(tokens[position].value) == '-') {
        ++position;
        expression();
        CHECK_ERROR
        operations.push_back(Operation::Neg);
    } else if(tokens[position].type == TokenType::Const) {
        constants.push_back(std::any_cast<double>(tokens[position].value));
        operations.push_back(Operation::Ldconst);
        ++position;
    } else if(tokens[position].type == TokenType::Id) {
        if(std::any_cast<std::string>(tokens[position].value) == "x") {
            operations.push_back(Operation::Ldx);
            ++position;
        } else if(std::any_cast<std::string>(tokens[position].value) == "y") {
            operations.push_back(Operation::Ldy);
            ++position;
        } else if(std::any_cast<std::string>(tokens[position].value) == "sin") {
            ++position;
            if(position >= tokens.size()) {
                error();
            }
            expression();
            CHECK_ERROR
            operations.push_back(Operation::Sin);
        } else if(std::any_cast<std::string>(tokens[position].value) == "cos") {
            ++position;
            if(position >= tokens.size()) {
                error();
            }
            expression();
            CHECK_ERROR
            operations.push_back(Operation::Cos);
        } else {
            error();
        }
    } else if(tokens[position].type == TokenType::Lpar) {
        expression();
        CHECK_ERROR
    } else {
        error();
    }
}

void ExpressionParser::error()
{
    hasError = true;
}

double Expression::evaluate(double x, double y)
{
    std::stack<double> stack;
    int constIdx = 0;
    for(int i = 0; i < operations.size(); ++i) {
        switch(operations[i]) {
            case Operation::Plus:
            case Operation::Minus:
            case Operation::Multiply:
            case Operation::Divide:
            case Operation::Power: {
                auto right = stack.top();
                stack.pop();
                auto left = stack.top();
                stack.pop();
                switch(operations[i]) {
                case Operation::Plus:
                    stack.push(left + right);
                    break;
                case Operation::Minus:
                    stack.push(left - right);
                    break;
                case Operation::Multiply:
                    stack.push(left * right);
                    break;
                case Operation::Divide:
                    stack.push(left / right);
                    break;
                case Operation::Power:
                    stack.push(pow(left, right));
                    break;
                }

                break;
            }
            case Operation::Ldconst:
                stack.push(constants[constIdx++]);
                break;
            case Operation::Ldx:
                stack.push(x);
                break;
            case Operation::Ldy:
                stack.push(y);
                break;
            case Operation::Neg: {
                auto val = -stack.top();
                stack.pop();
                stack.push(val);
                break;
            }
            case Operation::Sin: {
                auto val = stack.top();
                stack.pop();
                stack.push(sin(val));
                break;
            }
            case Operation::Cos:{
                auto val = stack.top();
                stack.pop();
                stack.push(cos(val));
                break;
            }
        }
    }

    auto val = stack.top();
    stack.pop();
    return val;
}

#undef CHECK_ERROR
