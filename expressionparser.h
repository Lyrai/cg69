#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <string>
#include <queue>
#include <vector>
#include <any>

enum class Operation {
    Ldconst,
    Ldx,
    Ldy,
    Plus,
    Minus,
    Multiply,
    Divide,
    Power,
    Neg,
    Sin,
    Cos
};

enum class TokenType {
    Id,
    Const,
    Operation,
    Lpar,
    Rpar
};

class Token {
public:
    TokenType type;
    std::any value;
};

class Expression {
public:
    static Expression invalid;
    double evaluate(double x, double y);
    bool isValid() { return _isValid; }
    explicit Expression(const std::vector<double>& constants, const std::vector<Operation>& operations): constants(constants), operations(operations), _isValid(true) { }

private:
    std::vector<double> constants;
    std::vector<Operation> operations;
    bool _isValid;
    explicit Expression(bool valid): _isValid(valid) { }
};

class ExpressionParser
{
public:
    ExpressionParser();

    Expression parse(const std::string& str);

private:
    void lex(const std::string& str);
    Expression parse();
    void expression();
    void term();
    void factor();
    void power();
    void primary();
    void error();

    int position = 0;
    bool hasError = false;
    std::vector<Token> tokens;
    std::vector<double> constants;
    std::vector<Operation> operations;
};

#endif // EXPRESSIONPARSER_H
