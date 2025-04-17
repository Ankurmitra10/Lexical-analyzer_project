
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define KEYWORD 1
#define IDENTIFIER 2
#define NUMBER 3
#define OPERATOR 4
#define SYMBOL 5
#define STRING 6

typedef struct {
    char token[50];
    int type;
    int line;
} Token;

char *keywords[] = {"int", "char", "if", "else", "return", "while", "for"};
int keywordCount = 7;

int isKeyword(char *word) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

int tokenize(char *code, Token tokens[], int *tokenCount) {
    int i = 0, j = 0, line = 1;
    char temp[50];
    *tokenCount = 0;

    while (code[i] != '\0') {
        if (code[i] == '\n') line++;
        if (isspace(code[i])) {
            i++;
            continue;
        }

        if (isalpha(code[i]) || code[i] == '_') {
            j = 0;
            while ((isalnum(code[i]) || code[i] == '_') && j < 49) {
                temp[j++] = code[i++];
            }
            temp[j] = '\0';

            Token t;
            strcpy(t.token, temp);
            t.type = isKeyword(temp) ? KEYWORD : IDENTIFIER;
            t.line = line;
            tokens[(*tokenCount)++] = t;
        } else if (isdigit(code[i])) {
            j = 0;
            while (isdigit(code[i]) && j < 49) temp[j++] = code[i++];
            temp[j] = '\0';

            Token t;
            strcpy(t.token, temp);
            t.type = NUMBER;
            t.line = line;
            tokens[(*tokenCount)++] = t;
        } else if (code[i] == '"') {
            j = 0;
            temp[j++] = code[i++];
            while (code[i] != '\0' && code[i] != '"' && j < 48) {
                temp[j++] = code[i++];
            }
            if (code[i] == '"') temp[j++] = code[i++];
            temp[j] = '\0';

            Token t;
            strcpy(t.token, temp);
            t.type = STRING;
            t.line = line;
            tokens[(*tokenCount)++] = t;
        } else {
            Token t;
            t.token[0] = code[i];
            t.token[1] = '\0';
            t.type = SYMBOL;
            t.line = line;
            tokens[(*tokenCount)++] = t;
            i++;
        }
    }
    return 1;
}

int main() {
    char code[2000] = "";
    Token tokens[1000];
    int tokenCount;
    char line[200];

    printf("Enter your multi-line C code (end with a single '$' on a new line):\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, "$\n") == 0) break;
        strcat(code, line);
    }

    if (tokenize(code, tokens, &tokenCount)) {
        printf("\nTokens:\n");
        for (int i = 0; i < tokenCount; i++) {
            printf("Token: %-15s Type: %-2d Line: %d\n", tokens[i].token, tokens[i].type, tokens[i].line);
        }
    } else {
        printf("Tokenization failed.\n");
    }

    return 0;
}
