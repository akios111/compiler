#ifndef TOKEN_H
#define TOKEN_H

// Τύποι tokens
typedef enum {
    INTEGER = 258,
    REAL,
    COMPLEX,
    LOGICAL,
    CHARACTER,
    PROGRAM,
    END,
    IF,
    THEN,
    ELSE,
    ENDIF,
    DO,
    ENDDO,
    CALL,
    GOTO,
    CONTINUE,
    RETURN,
    STOP,
    READ,
    WRITE,
    ID,
    ICONST,
    RCONST,
    ASSIGN,
    ADDOP,
    MULOP,
    DIVOP,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    COMMA,
    COLON
} token_type_t;

#endif 