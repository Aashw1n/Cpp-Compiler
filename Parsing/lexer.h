
#ifndef __LEXER__H__
#define __LEXER__H__
#include <vector>
#include <string>
#include "inputbuf.h"


typedef enum { END_OF_FILE = 0,
    PUBLIC, PRIVATE, 
    EQUAL, COLON, COMMA, SEMICOLON,
    LBRACE, RBRACE, ID, ERROR
} TokenType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();
    int parse(void); 
    int parseglobal(void); 
    int parsevariables(void);
    int parsepublic(void);
    int parseprivate(void);
    int parseslist(void);
    int parsestatements(void);
    int parsescope(void);
   

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;
    bool SkipSpace();
    bool IsKeyword(std::string);
    bool SkipComment();
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    
};

#endif
