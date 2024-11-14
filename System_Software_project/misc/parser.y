%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <vector>
  

  extern int yylex(void);
  extern FILE *yyin;
  extern void yyerror(const char*);
%}

%output "parser.cpp"
%defines "parser.hpp"

%code requires {
  #include "../inc/line.hpp"
}


%union {
  int num;
  char* str;
  struct line line;
  struct directive dir;
  struct instruction ins;
  struct symbol_or_literal symbol_or_literal;
  struct symbols symbols;
  struct operand operand;
}

%token DIR_GLOBAL
%token DIR_EXTERN
%token DIR_SECTION
%token DIR_WORD
%token DIR_SKIP

%token INS_HALT
%token INS_INT
%token INS_IRET
%token INS_CALL
%token INS_RET
%token INS_JMP
%token INS_BEQ
%token INS_BNE
%token INS_BGT
%token INS_PUSH
%token INS_POP
%token INS_XCHG
%token INS_ADD
%token INS_SUB
%token INS_MUL
%token INS_DIV
%token INS_NOT
%token INS_AND
%token INS_OR
%token INS_XOR
%token INS_SHL
%token INS_SHR
%token INS_LD
%token INS_ST
%token INS_CSRRD
%token INS_CSRWR

%token <num> REG
%token <num> CS_REG

%token ENDL

%token <num> NUMBER
%token <str> STRING
%token <str> SYMBOL

%type <line> line
%type <str> label
%type <dir> directive
%type <ins> instruction
%type <symbols> symbols
%type <symbols> symbols_and_literals
%type <symbol_or_literal> symbol_or_literal
%type <operand> operand

%%

lines: lines line 
      |
      line
      ;

line: directive ENDL { $$.label = nullptr; $$.type = DIRECTIVE; $$.directive = $1; lines->push_back($$); }
      |
      label directive ENDL { $$.label = $1; $$.type = DIRECTIVE; $$.directive = $2; lines->push_back($$); }
      |
      instruction ENDL { $$.label = nullptr; $$.type = INSTRUCTION; $$.instruction = $1; lines->push_back($$); }
      |
      label instruction ENDL { $$.label = $1; $$.type = INSTRUCTION; $$.instruction = $2; lines->push_back($$); }
      |
      label ENDL { $$.label = $1; $$.type = EMPTY; lines->push_back($$); }
      |
      ENDL { $$.label = nullptr; $$.type = EMPTY; }
      ;

label: SYMBOL ':' { $$ = $1; };

directive: DIR_GLOBAL symbols { $$.type = GLOBAL; $$.symbols = $2; }
          |
          DIR_EXTERN symbols { $$.type = EXTERN; $$.symbols = $2; }
          |
          DIR_SECTION SYMBOL { 
            $$.type = SECTION; 
            symbols syms;
            syms.size = 1;
            syms.data = (symbol_or_literal*)malloc(sizeof(symbol_or_literal));
            syms.data[0].symbol = $2;
            syms.data[0].type = SYM;
            $$.symbols = syms;
          }
          |
          DIR_WORD symbols_and_literals { $$.type = WORD; $$.symbols = $2; }
          |
          DIR_SKIP NUMBER {
            $$.type = SKIP; 
            symbols syms;
            syms.size = 1;
            syms.data = (symbol_or_literal*)malloc(sizeof(symbol_or_literal));
            syms.data[0].num = $2;
            syms.data[0].type = NUM;
            $$.symbols = syms;
          }
          ;

instruction: INS_HALT { $$.type = HALT; }
            |
            INS_INT { $$.type = INT; }
            |
            INS_IRET { $$.type = IRET; }
            |
            INS_CALL operand { $$.type = CALL; $$.operand = $2; }
            |
            INS_RET { $$.type = RET; }
            |
            INS_JMP operand { $$.type = JMP; $$.operand = $2; }
            |
            INS_BEQ REG ',' REG ',' operand { $$.type = BEQ; $$.regS = $2; $$.regD = $4; $$.operand = $6; }
            |
            INS_BNE REG ',' REG ',' operand { $$.type = BNE; $$.regS = $2; $$.regD = $4; $$.operand = $6; }
            |
            INS_BGT REG ',' REG ',' operand { $$.type = BGT; $$.regS = $2; $$.regD = $4; $$.operand = $6; }
            |
            INS_PUSH REG { $$.type = PUSH; $$.regS = $2; }
            |
            INS_POP REG { $$.type = POP; $$.regS = $2; }
            |
            INS_XCHG REG ',' REG { $$.type = XCHG; $$.regS = $2; $$.regD = $4; }
            |
            INS_ADD REG ',' REG { $$.type = ADD; $$.regS = $2; $$.regD = $4; }
            |
            INS_SUB REG ',' REG { $$.type = SUB; $$.regS = $2; $$.regD = $4; }
            |
            INS_MUL REG ',' REG { $$.type = MUL; $$.regS = $2; $$.regD = $4; }
            |
            INS_DIV REG ',' REG { $$.type = DIV; $$.regS = $2; $$.regD = $4; }
            |
            INS_NOT REG { $$.type = NOT; $$.regS = $2; }
            |
            INS_AND REG ',' REG { $$.type = AND; $$.regS = $2; $$.regD = $4; }
            |
            INS_OR REG ',' REG { $$.type = OR; $$.regS = $2; $$.regD = $4; }
            |
            INS_XOR REG ',' REG { $$.type = XOR; $$.regS = $2; $$.regD = $4; }
            |
            INS_SHL REG ',' REG { $$.type = SHL; $$.regS = $2; $$.regD = $4; }
            |
            INS_SHR REG ',' REG { $$.type = SHR; $$.regS = $2; $$.regD = $4; }
            |
            INS_LD operand ',' REG { $$.type = LD; $$.operand = $2; $$.regD = $4; }
            |
            INS_ST REG ',' operand { $$.type = ST; $$.regS = $2; $$.operand = $4; }
            |
            INS_CSRRD CS_REG ',' REG { $$.type = CSRRD; $$.regS = $2; $$.regD = $4; }
            |
            INS_CSRWR REG ',' CS_REG { $$.type = CSRWR; $$.regS = $2; $$.regD = $4; }
            ;

symbols: symbols ',' SYMBOL
          {
            $$.size = $$.size+1;
            $$.data = (symbol_or_literal*)realloc($1.data, $$.size * sizeof(symbol_or_literal));
            $$.data[$$.size-1].symbol = $3;
            $$.data[$$.size-1].type = SYM;
          }
          |
          SYMBOL
          {
            $$.size = 1;
            $$.data = (symbol_or_literal*)malloc(sizeof(symbol_or_literal));
            $$.data[0].symbol = $1;
            $$.data[0].type = SYM;
          }
          ;

symbols_and_literals: symbols_and_literals ',' symbol_or_literal
          {
            $$.size = $$.size+1;
            $$.data = (symbol_or_literal*)realloc($1.data, $$.size * sizeof(symbol_or_literal));
            $$.data[$$.size-1] = $3;
          }
          |
          symbol_or_literal
          {
            $$.size = 1;
            $$.data = (symbol_or_literal*)malloc(sizeof(symbol_or_literal));
            $$.data[0] = $1;
          }
          ;

symbol_or_literal: NUMBER { $$.num = $1; $$.type = NUM; }
          |
          STRING { $$.str = $1; $$.type = STR; }
          |
          SYMBOL { $$.symbol = $1; $$.type = SYM; }
          ;

operand: '$' NUMBER { $$.type = IMM_LIT; $$.num = $2; }
          |
          '$' SYMBOL { $$.type = IMM_SYM; $$.symbol = $2; }
          |
          NUMBER { $$.type = ADDR_LIT; $$.num = $1; }
          |
          SYMBOL { $$.type = ADDR_SYM; $$.symbol = $1; }
          |
          REG { $$.type = REGISTER; $$.reg = $1; }
          |
          '[' REG ']' { $$.type = ADDR_REG; $$.reg = $2; }
          |
          '[' REG '+' NUMBER ']' { $$.type = ADDR_REG_LIT; $$.reg = $2; $$.num = $4; }
          |
          '[' REG '+' SYMBOL ']' { $$.type = ADDR_REG_SYM; $$.reg = $2; $$.symbol = $4; }
          ;


%%


