#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

enum class TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    END_OF_INPUT
};

struct Token {
    TokenType type;
    int value;

    Token(TokenType t, int v) : type(t), value(v) {}
};

class Lexer {
public:
    Lexer(const std::string& input) : input_(input), position_(0) {}

    Token getNextToken() {
        while (position_ < input_.size() && std::isspace(input_[position_])) {
            ++position_;
        }

        if (position_ >= input_.size()) {
            return Token(TokenType::END_OF_INPUT, 0);
        }

        char currentChar = input_[position_];
        if (std::isdigit(currentChar)) {
            return Token(TokenType::INTEGER, parseInt());
        } else {
            ++position_;
            switch (currentChar) {
                case '+':
                    return Token(TokenType::PLUS, 0);
                case '-':
                    return Token(TokenType::MINUS, 0);
                case '*':
                    return Token(TokenType::MULTIPLY, 0);
                case '/':
                    return Token(TokenType::DIVIDE, 0);
                default:
                    throw std::runtime_error("Invalid token");
            }
        }
    }

private:
    int parseInt() {
        std::stringstream ss;
        while (position_ < input_.size() && std::isdigit(input_[position_])) {
            ss << input_[position_++];
        }
        int result;
        ss >> result;
        return result;
    }

    std::string input_;
    size_t position_;
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer_(lexer) {}

    int parseExpression() {
        int left = parseTerm();
        while (true) {
            Token token = lexer_.getNextToken();
            switch (token.type) {
                case TokenType::PLUS:
                    left += parseTerm();
                    break;
                case TokenType::MINUS:
                    left -= parseTerm();
                    break;
                default:
                    return left;
            }
        }
    }

    int parseTerm() {
        int left = parseFactor();
        while (true) {
            Token token = lexer_.getNextToken();
            switch (token.type) {
                case TokenType::MULTIPLY:
                    left *= parseFactor();
                    break;
                case TokenType::DIVIDE:
                    left /= parseFactor();
                    break;
                default:
                    return left;
            }
        }
    }

    int parseFactor() {
        Token token = lexer_.getNextToken();
        if (token.type == TokenType::INTEGER) {
            return token.value;
        } else {
            throw std::runtime_error("Expected integer");
        }
    }

private:
    Lexer& lexer_;
};

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression: ";
    std::cin >> input;

    Lexer lexer(input);
    Parser parser(lexer);

    try {
        int result = parser.parseExpression();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
