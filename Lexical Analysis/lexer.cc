//Aashwin Ranjan,for CSE340 Summer 2019 
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM","BASE08NUM","BASE16NUM" //The new token types are added here
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()    //Modifying ScanNumber to incorporate
                                       //REALNUMBER, BASE08NUM. BASE16NUM
{
   char c;
    bool base08 = true;
    bool base16 = true;
    bool real = false;
    bool charZero= false;
    int countHex = 0;
    int countZero = 0;

    input.GetChar(c);

    if (isdigit(c)) {
        if (c == '0') {
            charZero = true;
        }
        tmp.lexeme = "";
        tmp.lexeme += c;
input.GetChar(c);

        while (!input.EndOfInput() && (isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F')) {
            if ((c - '0') >= 8){
                base08 = false;
            }
            countHex++;
            tmp.lexeme += c;
            input.GetChar(c);           
        }




        if (c == 'x'){
            if (charZero == true && countHex != 0){
                input.UngetChar('x');
                int i = 0;
                while(i < countHex){
                    input.UngetChar(tmp.lexeme[tmp.lexeme.size()-1]);
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                    i++;
                }
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
            tmp.lexeme += c;
            input.GetChar(c);
            if (c == '0' && base08 == true){
                tmp.lexeme += c;
                input.GetChar(c);
                if (c == '8'){
                    tmp.lexeme += c;
                    tmp.token_type = BASE08NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }
                else{
                    input.UngetChar(c);
                    input.UngetChar('0');
                    input.UngetChar('x');
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                }
            }
            else if (c == '1' && base16 == true){
                tmp.lexeme += c;
                input.GetChar(c);
                if (c == '6'){
                    tmp.lexeme += c;
                    tmp.token_type = BASE16NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }
                else{
                    input.UngetChar(c);
                    input.UngetChar('1');
                    input.UngetChar('x');
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                }
            }
            else{
                input.UngetChar(c);
                input.UngetChar('x');
                (tmp.lexeme).erase(tmp.lexeme.size() - 1);
            }
        } 
        else{
            input.UngetChar(c);
        }



        

        if (countHex > 0 && !input.EndOfInput()){
            int i = 0;
            while (i < countHex){
                if(!input.EndOfInput()){
                    input.UngetChar(tmp.lexeme[tmp.lexeme.size()-1]);
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                }               
                i++;
            }
        }








        input.GetChar(c);
        
        while (!input.EndOfInput() && charZero == false && isdigit(c)) {
            tmp.lexeme += c;
            if ((c - '0') >= 8){
                base08 = false;
            }
            input.GetChar(c);           
        }



        if (c == '.'){      
            tmp.lexeme += c;
            input.GetChar(c);
            while (!input.EndOfInput() && isdigit(c)) {
                countZero++;		
                if (c != '0'){		
                    real = true;		
                }
                tmp.lexeme += c;
                input.GetChar(c);           
            }


            
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }


            
            if((real == true || charZero == false) && isdigit(tmp.lexeme.at(tmp.lexeme.length()-1))){             
                tmp.token_type = REALNUM;
                tmp.line_no = line_no;
                return tmp;
            }
            else {
                int i = 0;
                while (i < countZero){
                    (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                    input.UngetChar('0');
                    i++;
                }
                (tmp.lexeme).erase(tmp.lexeme.size() - 1);
                input.UngetChar('.');
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
        }
        else{
            input.UngetChar(c);
        }
        
            
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
     }else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}




Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
