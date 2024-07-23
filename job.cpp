//TASK
//1. Defining syntax and semantics for logical operations
#include <iostream>
using namespace std;

int main() {
  int x = 100;
  int y = 28;
  if (x > y) {
    cout << "x is greater than y";
  }  
  return 0;
}
//THIS BELOW CODE IS IN LEXER
#include <iostream>
#include <vector>
#include <regex>
#include <string>

enum TokenType { INT, IDENTIFIER, NUMBER, ASSIGN, PLUS, MINUS, IF, EQUALS, LBRACE, RBRACE, SEMICOLON };

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string &source) : source(source), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::regex patterns("(int)|([a-zA-Z_][a-zA-Z0-9_]*)|(\\d+)|(=)|([+])|([-])|(if)|(==)|(\\{)|(\\})|(;)");
        auto words_begin = std::sregex_iterator(source.begin(), source.end(), patterns);
        auto words_end = std::sregex_iterator();

        for (auto i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            if (match[1].matched) tokens.push_back({INT, match.str()});
            else if (match[2].matched) tokens.push_back({IDENTIFIER, match.str()});
            else if (match[3].matched) tokens.push_back({NUMBER, match.str()});
            else if (match[4].matched) tokens.push_back({ASSIGN, match.str()});
            else if (match[5].matched) tokens.push_back({PLUS, match.str()});
            else if (match[6].matched) tokens.push_back({MINUS, match.str()});
            else if (match[7].matched) tokens.push_back({IF, match.str()});
            else if (match[8].matched) tokens.push_back({EQUALS, match.str()});
            else if (match[9].matched) tokens.push_back({LBRACE, match.str()});
            else if (match[10].matched) tokens.push_back({RBRACE, match.str()});
            else if (match[11].matched) tokens.push_back({SEMICOLON, match.str()});
        }
        return tokens;
    }

private:
    std::string source;
    size_t pos;
};

int main() {
    std::string code = "int a; a = 5 + 3; if (a == 8) { a = a - 1; }";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();
    for (const Token &token : tokens) {
        std::cout << "Token: " << token.value << std::endl;
    }
    return 0;
}
//THIS BELOW CODE IS PARSER
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

enum TokenType { INT, IDENTIFIER, NUMBER, ASSIGN, PLUS, MINUS, IF, EQUALS, LBRACE, RBRACE, SEMICOLON };

struct Token {
    TokenType type;
    std::string value;
};

class Parser {
public:
    Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parse() {
        while (pos < tokens.size()) {
            if (tokens[pos].type == INT) {
                parseDeclaration();
            } else if (tokens[pos].type == IDENTIFIER) {
                parseAssignment();
            } else if (tokens[pos].type == IF) {
                parseConditional();
            } else {
                throw std::runtime_error("Unexpected token: " + tokens[pos].value);
            }
        }
    }

private:
    std::vector<Token> tokens;
    size_t pos;

    void parseDeclaration() {
        pos++;  // Skip 'int'
        if (tokens[pos].type == IDENTIFIER) {
            std::cout << "Declare variable: " << tokens[pos].value << std::endl;
            pos++;  // Skip variable name
            if (tokens[pos].type == SEMICOLON) {
                pos++;  // Skip semicolon
            } else {
                throw std::runtime_error("Expected ';' after declaration");
            }
        } else {
            throw std::runtime_error("Expected variable name after 'int'");
        }
    }

    void parseAssignment() {
        std::string var = tokens[pos].value;
        pos++;  // Skip variable name
        if (tokens[pos].type == ASSIGN) {
            pos++;  // Skip '='
            std::cout << "Assign to variable: " << var << std::endl;
            parseExpression();
            if (tokens[pos].type == SEMICOLON) {
                pos++;  // Skip semicolon
            } else {
                throw std::runtime_error("Expected ';' after assignment");
            }
        } else {
            throw std::runtime_error("Expected '=' after variable name");
        }
    }

    void parseExpression() {
        // Parse a simple expression with numbers and +/-
        if (tokens[pos].type == NUMBER) {
            std::cout << "Number: " << tokens[pos].value << std::endl;
            pos++;
        } else if (tokens[pos].type == IDENTIFIER) {
            std::cout << "Variable: " << tokens[pos].value << std::endl;
            pos++;
        } else {
            throw std::runtime_error("Expected a number or variable in expression");
        }

        while (pos < tokens.size() && (tokens[pos].type == PLUS || tokens[pos].type == MINUS)) {
            std::cout << "Operator: " << tokens[pos].value << std::endl;
            pos++;
            if (tokens[pos].type == NUMBER) {
                std::cout << "Number: " << tokens[pos].value << std::endl;
                pos++;
            } else if (tokens[pos].type == IDENTIFIER) {
                std::cout << "Variable: " << tokens[pos].value << std::endl;
                pos++;
            } else {
                throw std::runtime_error("Expected a number or variable after operator");
            }
        }
    }

    void parseConditional() {
        pos++;  // Skip 'if'
        if (tokens[pos].type == LBRACE) {
            pos++;  // Skip '{'
            while (pos < tokens.size() && tokens[pos].type != RBRACE) {
                if (tokens[pos].type == IDENTIFIER) {
                    parseAssignment();
                } else {
                    throw std::runtime_error("Unexpected token in if block: " + tokens[pos].value);
                }
            }
            if (tokens[pos].type == RBRACE) {
                pos++;  // Skip '}'
            } else {
                throw std::runtime_error("Expected '}' at the end of if block");
            }
        } else {
            throw std::runtime_error("Expected '{' after 'if'");
        }
    }
};

int main() {
    std::string code = "int a; a = 5 + 3; if (a == 8) { a = a - 1; }";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    parser.parse();
    return 0;
}
class Parser {
public:
    // Previous code...

    void parseAssignment() {
        std::string var = tokens[pos].value;
        pos++;  // Skip variable name
        if (tokens[pos].type == ASSIGN) {
            pos++;  // Skip '='
            std::string assemblyCode = generateExpressionCode();
            assemblyCode += "STORE " + var + "\n";
            std::cout << assemblyCode;
            if (tokens[pos].type == SEMICOLON) {
                pos++;  // Skip semicolon
            } else {
                throw std::runtime_error("Expected ';' after assignment");
            }
        } else {
            throw std::runtime_error("Expected '=' after variable name");
        }
    }

    std::string generateExpressionCode() {
        std::string code;
        // Similar to parseExpression, but generate assembly code
        if (tokens[pos].type == NUMBER) {
            code += "LOAD A, " + tokens[pos].value + "\n";
            pos++;
        } else if (tokens[pos].type == IDENTIFIER) {
            code += "LOAD A, " + tokens[pos].value + "\n";
            pos++;
        }

        while (pos < tokens.size() && (tokens[pos].type == PLUS || tokens[pos].type == MINUS)) {
            std::string op = tokens[pos].value;
            pos++;
            if (tokens[pos].type == NUMBER) {
                code += "LOAD B, " + tokens[pos].value + "\n";
                pos++;
            } else if (tokens[pos].type == IDENTIFIER) {
                code += "LOAD B, " + tokens[pos].value + "\n";
                pos++;
            }
            if (op == "+") {
                code += "ADD A, B\n";
            } else if (op == "-") {
                code += "SUB A, B\n";
            }
        }

        return code;
    }
};

