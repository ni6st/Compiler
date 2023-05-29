#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

typedef struct {
    const char* input;
    Token currentToken;
    int currentIndex;
} Lexer;

void initLexer(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->currentIndex = 0;
    lexer->currentToken.type = TOKEN_END;
    lexer->currentToken.value = 0;
}

void getNextToken(Lexer* lexer) {
    const char* input = lexer->input;
    int currentIndex = lexer->currentIndex;
    char currentChar = input[currentIndex];

    if (currentChar == '\0') {
        lexer->currentToken.type = TOKEN_END;
        lexer->currentToken.value = 0;
        return;
    }

    if (isdigit(currentChar)) {
        int value = 0;
        while (isdigit(currentChar)) {
            value = value * 10 + (currentChar - '0');
            currentChar = input[++currentIndex];
        }
        lexer->currentToken.type = TOKEN_INTEGER;
        lexer->currentToken.value = value;
    } else {
        switch (currentChar) {
            case '+':
                lexer->currentToken.type = TOKEN_PLUS;
                break;
            case '-':
                lexer->currentToken.type = TOKEN_MINUS;
                break;
            case '*':
                lexer->currentToken.type = TOKEN_MULTIPLY;
                break;
            case '/':
                lexer->currentToken.type = TOKEN_DIVIDE;
                break;
            case '(':
                lexer->currentToken.type = TOKEN_LPAREN;
                break;
            case ')':
                lexer->currentToken.type = TOKEN_RPAREN;
                break;
            default:
                printf("Invalid character: %c\n", currentChar);
                exit(1);
        }
        lexer->currentToken.value = 0;
        currentIndex++;
    }

    lexer->currentIndex = currentIndex;
}

void parseExpression(Lexer* lexer);

void parseFactor(Lexer* lexer) {
    Token token = lexer->currentToken;
    if (token.type == TOKEN_INTEGER) {
        printf("    push %d\n", token.value);
        getNextToken(lexer);
    } else if (token.type == TOKEN_LPAREN) {
        getNextToken(lexer);
        parseExpression(lexer);
        if (lexer->currentToken.type != TOKEN_RPAREN) {
            printf("Expected ')'\n");
            exit(1);
        }
        getNextToken(lexer);
    } else {
        printf("Unexpected token: %d\n", token.type);
        exit(1);
    }
}

void parseTerm(Lexer* lexer) {
    parseFactor(lexer);
    Token token = lexer->currentToken;

    while (token.type == TOKEN_MULTIPLY || token.type == TOKEN_DIVIDE) {
        getNextToken(lexer);
        parseFactor(lexer);

        if (token.type == TOKEN_MULTIPLY) {
            printf("    multiply\n");
        } else {
            printf("    divide\n");
        }

        token = lexer->currentToken;
    }
}

void parseExpression(Lexer* lexer) {
    parseTerm(lexer);
    Token token = lexer->currentToken;

    while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
        getNextToken(lexer);
        parseTerm(lexer);

        if (token.type == TOKEN_PLUS) {
            printf("    add\n");
        } else {
            printf("    subtract\n");
        }

        token = lexer->currentToken;
    }
}

int main() {
    const char* input = "2 * (3 + 4)";

    Lexer lexer;
    initLexer(&lexer, input);

    getNextToken(&lexer);
    parseExpression(&lexer);

    return 0;
}
