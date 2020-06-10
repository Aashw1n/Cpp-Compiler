//Aashwin Ranjan 
//CSE340 Summer 2019
//Modifying the lexer from previous asignment and adding parser. 
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stack>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "PUBLIC", "PRIVATE", "EQUAL", "COLON", "COMMA",
     "SEMICOLON", "LBRACE", "RBRACE", "ID", "ERROR"
};

#define KEYWORDS_COUNT 2
string keyword[] = { "public", "private" };



void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}


//Constructing the symbol table
struct stcontent{   
char* name;
char* scope;
int permission;
};
struct sTable{    stcontent* item;
sTable* prev;
sTable* next;
};


struct sTable* symbolTable; 
struct sTable* tempNode;
struct sTable* newNode;


char* sScope = "::";
int sPermission = 0;

struct store {
string lhs;
string rhs;
char* scope1;
char* scope2;
};
vector< struct store> storeList;





void getAssignment(Token tok1, Token tok2){

struct sTable* temp = newNode;
struct sTable* temp1 = newNode;
char* scope1 = (char*)malloc(sizeof(char)*20);      
char* scope2 = (char*)malloc(sizeof(char)*20);

while(temp != NULL){
   
   if(temp->item->name == tok1.lexeme){
   if(temp->item->permission == 2 && sScope != temp->item->scope){
       
        temp= temp->prev;
        continue;
    }else{
        scope1 = temp->item->scope;
        
        break;
    }
  
   }
   temp= temp->prev;
   
}
while(temp1 != NULL){
   if(temp1->item-> name == tok2.lexeme){
    if(temp1->item->permission == 2 && sScope != temp1->item->scope){
    temp1 = temp1->prev;
    continue;
        
    }else{
        scope2 = temp1->item->scope;
        break;
    }
   }
   
   temp1 = temp1->prev;
   
}

if(strlen(scope1) == 0) {

 scope1 = new char[2];
 scope1[0] = '?';
 scope1[1] = '\0';
}
if(strlen(scope2) == 0){

scope2 = new char[2];
scope2[0] = '?';
scope2[1] = '\0';

} 

struct store temp_node;
temp_node.lhs = tok1.lexeme;
temp_node.rhs = tok2.lexeme;
temp_node.scope1 = scope1;
temp_node.scope2 = scope2;
storeList.push_back(temp_node);


}





void printlist(){
for(int i=0; i<storeList.size();i++){
    cout << storeList[i].scope1;
    if(storeList[i].scope1 != "::") 
        cout << "."; 
    cout<< storeList[i].lhs << " = " << storeList[i].scope2;
    if(storeList[i].scope2 != "::") 
        cout<<".";
    cout<< storeList[i].rhs << endl;
    
    }
}
void deleteList(char* lexeme){
  
  if(symbolTable == NULL){
  return;
  }
   while(newNode->item->scope == lexeme){   
 
   tempNode->next = NULL;
   newNode ->prev = NULL;
   if(tempNode->prev != NULL){
  
   tempNode=tempNode->prev;
   newNode=tempNode->next;
   newNode->next=NULL;
   return;
   }
   else if(tempNode == newNode){
  
   tempNode = NULL;
   newNode = NULL;
   return;
   }
   else{
   
   tempNode->next = NULL;
   newNode ->prev = NULL;
   newNode=tempNode;
   tempNode->next =NULL;
   }

  
   }
   sScope = newNode->item->scope;
   
   
}


 void addList(char* lexeme){ 
 if(symbolTable == NULL){ 
 symbolTable = (sTable*) malloc (sizeof(sTable));
 struct stcontent* newItem = (stcontent*) malloc (sizeof(stcontent));
 symbolTable ->item = newItem;
 symbolTable-> next = NULL;   
 symbolTable-> prev = NULL;
  
 int len = strlen(lexeme) ; 
 symbolTable->item->name = new char[len+1];
 strcpy(symbolTable->item->name,lexeme);
 symbolTable->item->name[len] = '\0'; 
 symbolTable->item->scope = sScope;
 symbolTable->item->permission = sPermission; 
 
 newNode = symbolTable;
 tempNode = symbolTable; 
 }
 else { 
 tempNode = symbolTable;
 while (tempNode->next != NULL){ 
 tempNode = tempNode-> next;
 }
 
 newNode = (sTable*) malloc (sizeof(sTable));
 struct stcontent* newItem = (stcontent*) malloc(sizeof(stcontent));
 newNode -> item = newItem;
 newNode -> next = NULL;
 newNode -> prev = tempNode;
 tempNode ->next = newNode;
 int len = strlen(lexeme) ;

 newNode -> item-> name = new char[len+1];
 strcpy(newNode ->item->name, lexeme);
 newNode -> item -> name[len] = '\0';
 newNode -> item -> scope = sScope;
 newNode -> item -> permission = sPermission;

 
 }
 
}



LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}
 LexicalAnalyzer lexer;
 Token token1;
 Token token2;
 Token token3;

int LexicalAnalyzer::parse(void){

 token1 = lexer.GetToken();

 if(token1.token_type == ID) {
   token2 = lexer.GetToken();
    
   if(token2.token_type == COMMA || token2.token_type == SEMICOLON){
   lexer.UngetToken(token2);
   lexer.UngetToken(token1);
   lexer.parseglobal();
   lexer.parsescope();
   
   }
   else if(token2.token_type == LBRACE){
   lexer.UngetToken(token2);
   lexer.UngetToken(token1);
   lexer.parsescope();
   
   }
   else {
   cout << "Syntax Error"<<endl;
   exit(1);
   }
 } 
 else{
 cout <<"Syntax Error"<<endl; 
 exit(1);
  }
  return 0;
}

int LexicalAnalyzer::parseglobal(void){

token1 = lexer.GetToken();

if(token1.token_type == ID){
 lexer.UngetToken(token1);
 lexer.parsevariables();
 token1 = lexer.GetToken();

   if(token1.token_type ==SEMICOLON){
 
     }
   else {
   cout << "Syntax Error" <<endl;
   exit(1); 
   }   
 }
else {
cout << "Syntax Error"<< endl;
exit(1);
}
return 0;
}

int LexicalAnalyzer::parsescope(void){

token1 = lexer.GetToken();

 if(token1.token_type == ID){
 
 string slexeme = token1.lexeme; 
   sScope = const_cast<char *>(slexeme.c_str());  
 
   token1= lexer.GetToken();   

  if(token1.token_type == LBRACE){   
   lexer.parsepublic();
   lexer.parseprivate();
   lexer.parseslist();
   token1 = lexer.GetToken();
  
    if(token1.token_type == RBRACE){
    
    deleteList(sScope);
    token1=lexer.GetToken();
    
       if(token1.token_type ==END_OF_FILE){
       deleteList(sScope);
       }
       else{
       UngetToken(token1);
       }
    return 0; 
     
    }
    else {
    
    cout << "Syntax Error"<< endl;
    exit(1);
    }
   }
  else{
  cout<< "Syntax Error"<<endl;
  exit(1);
  }
 }
 else{
 cout << "Syntax Error"<<endl;
 exit(1);
 }
return 0; 
}

int LexicalAnalyzer::parsepublic(void){

token1 = lexer.GetToken();

 if(token1.token_type == PUBLIC){
  sPermission = 1;
  token1 = lexer.GetToken();

   if(token1.token_type == COLON){
    token1 = lexer.GetToken(); 
   
    if(token1.token_type == ID){
    lexer.UngetToken(token1);
    lexer.parsevariables();
    token1 = lexer.GetToken();
      if(token1.token_type == SEMICOLON){
     
      }
      else{
      cout<< "Syntax Error" << endl;
      exit(1);
      }
     }
    else{
     cout<< "Syntax Error" << endl;
     exit(1);
     }
    }
   else{
    cout<< "Syntax Error" << endl;
    exit(1);
    }
   }
  else if(token1.token_type == PRIVATE || token1.token_type == ID){
  lexer.UngetToken(token1);
   
   }
  else{
  cout << "Syntax Error"<< endl;
  exit(1);
  }
  return 0;
}
int LexicalAnalyzer::parseprivate(void){

 token1=lexer.GetToken();
 
 if(token1.token_type == PRIVATE){
 sPermission = 2;
  token1 = lexer.GetToken();
 
   if(token1.token_type == COLON){
    token1 =lexer.GetToken();
    
    if(token1.token_type == ID){
     lexer.UngetToken(token1);
     lexer.parsevariables();
     token1 = lexer.GetToken();
     
     if(token1.token_type == SEMICOLON){
  
     }
     else{
     cout<<"Syntax Error"<<endl;
     exit(1);
     }
     }
    else{
    cout<<"Syntax Error"<<endl;
    exit(1);
    }
   }
   else{
   cout<<"Syntax Error"<<endl;
   exit(1);
   }
  
 }
  
 else if(token1.token_type == ID) {
 lexer.UngetToken(token1);
 
 }
 else{
 cout<< "Syntax Error"<< endl;
 }
 return 0;
}

int LexicalAnalyzer::parsevariables(void){

  token1 = lexer.GetToken();
  
  char* lexeme = (char*) malloc (sizeof (token1.lexeme)); 
  memcpy(lexeme,(token1.lexeme).c_str(),sizeof(token1)); 

  addList(lexeme); 
  
  sTable* temp1 = symbolTable; 
 
  while(temp1 != NULL){
 
   temp1 = temp1 ->next; 
  }
  if(token1.token_type ==ID){
    token1 = lexer.GetToken();
    
     if(token1.token_type == COMMA){    
    
       lexer.parsevariables();

       return 0;        
       }
  
     else if(token1.token_type == SEMICOLON){
       UngetToken(token1);
 
       return 0;
       }
     else{
      cout << "Syntax Error" << endl;   
      exit(1);
     }
  }
  else {
   cout << "Syntax Error" << endl;
   exit(1);
  }
   
}

int LexicalAnalyzer::parsestatements(void){

  token1 = lexer.GetToken();
 
  if(token1.token_type == ID){
    token2 = lexer.GetToken();
    
     if(token2.token_type == EQUAL){     
      token3 = lexer.GetToken();
      
       if(token3.token_type == ID) {
       
        getAssignment(token1, token3);
        
        token1 = lexer.GetToken();
        
          if(token1.token_type == SEMICOLON){

          return 0;
          }
          else{
          cout << "Syntax Error" << endl;
           exit(1);
          }
       }
       else {
       cout << "Syntax Error" << endl;
       exit(1);
       }
      }
     else if(token2.token_type == LBRACE){
   
     sScope = const_cast<char*>((token1.lexeme).c_str()); 
  
     lexer.UngetToken(token2);
     lexer.UngetToken(token1);
     lexer.parsescope();     
     }
     
     else {
     cout << "Syntax Error" << endl;
     exit(1);
     }
  }
  
  else{
  cout << "Syntax Error" << endl;
  exit(1);
  }
  return 0;
}

int LexicalAnalyzer::parseslist(void){

 token1 = lexer.GetToken();
 
   if(token1.token_type == ID){
     lexer.UngetToken(token1);
      lexer.parsestatements();
       token2 = lexer.GetToken();
   
       if(token2.token_type == ID){
       lexer.UngetToken(token2);
       lexer.parseslist();
  
       return 0;
       }
      
       else if(token2.token_type == RBRACE){       
    
       lexer.UngetToken(token2);       
       return 0;
       }  
       
       else {
       cout<< "Syntax Error"<<endl;
       exit(1);
       }
    }
    
    else {
    cout<< "Syntax Error"<<endl;
    exit(1);
    }
 
}
bool LexicalAnalyzer::SkipComment(){
 
  char c;
  bool comment = false;
    input.GetChar(c);
  if(input.EndOfInput()){
  input.UngetChar(c);
  return comment;
  }
  
  
     if(c=='/'){
    input.GetChar(c);
         if(c=='/'){
         comment = true;
         
           while (c != '\n'){
           comment = true;
           input.GetChar(c);
           }
         line_no +=1;
         SkipComment();      
         }
         else{
         comment = false;
         
         exit(1);
         }
      }
     else {
     input.UngetChar(c);    
     return comment;
     }
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
        if (IsKeyword(tmp.lexeme)){          
            tmp.token_type = FindKeywordIndex(tmp.lexeme);           
            }
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

TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }
 
    SkipSpace();
    SkipComment();
    SkipSpace();
    
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
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
        case '{':
            tmp.token_type = LBRACE;
            return tmp;
        case '}':
            tmp.token_type = RBRACE;
            return tmp;
        
        default:
            if (isdigit(c)){
              input.UngetChar(c);
              tmp.token_type = ERROR;
              return tmp;
             }            
            else if (isalpha(c)) {
            input.UngetChar(c);
            return ScanIdOrKeyword();
            }
            else if (input.EndOfInput())
            tmp.token_type = END_OF_FILE;
               
            else{
            tmp.token_type = ERROR;
            }

            return tmp;
    }
}

int main()
{
    int i;
    i = lexer.parse(); 
    printlist();
   
}