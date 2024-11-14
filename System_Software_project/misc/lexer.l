
%{
  #include "parser.hpp"
  #include <stdio.h>

  int line_num = 1;
%}

%option outfile="lexer.cpp" header-file="lexer.hpp"
%option noyywrap

%x END
%x COMMENT

%%


[ \t]      { }

<INITIAL>\n            { line_num++; return ENDL; }

#             { BEGIN(COMMENT); }


<COMMENT>\n   { line_num++; BEGIN(INITIAL); return ENDL; }
<COMMENT>.    { }

\.end         { BEGIN(END); }

<END>.        { }
<END>\n       { }

\.global      { return DIR_GLOBAL; }
\.extern      { return DIR_EXTERN; }
\.section     { return DIR_SECTION; }
\.word        { return DIR_WORD; }
\.skip        { return DIR_SKIP; }

halt          { return INS_HALT; }
int           { return INS_INT; }
iret          { return INS_IRET; }
call          { return INS_CALL; }
ret           { return INS_RET; }
jmp           { return INS_JMP; }
beq           { return INS_BEQ; }
bne           { return INS_BNE; }
bgt           { return INS_BGT; }
push          { return INS_PUSH; }
pop           { return INS_POP; }
xchg          { return INS_XCHG; }
add           { return INS_ADD; }
sub           { return INS_SUB; }
mul           { return INS_MUL; }
div           { return INS_DIV; }
not           { return INS_NOT; }
and           { return INS_AND; }
or            { return INS_OR; }
xor           { return INS_XOR; }
shl           { return INS_SHL; }
shr           { return INS_SHR; }
ld            { return INS_LD; }
st            { return INS_ST; }
csrrd         { return INS_CSRRD; }
csrwr         { return INS_CSRWR; }


%r([0-9]|1[0-5])        { yylval.num = atoi(yytext+2); return REG; }
%sp                     { yylval.num = 14; return REG; }
%pc                     { yylval.num = 15; return REG; }

%status                 { yylval.num = 0; return CS_REG; }
%handler                { yylval.num = 1; return CS_REG; }
%cause                  { yylval.num = 2; return CS_REG; }

[,:\$\[\]\+\-]                  { return yytext[0]; }

-?[0-9]+                { yylval.num = atoi(yytext); return NUMBER; }
0x[0-9a-fA-F]+          { yylval.num = strtol(yytext, NULL, 16); return NUMBER; } 
\".*\"                  { yylval.str = strdup(yytext+1); yylval.str[strlen(yylval.str)-1] = '\0'; return STRING; }
[a-zA-Z][a-zA-Z0-9_]*   { yylval.str = strdup(yytext); return SYMBOL; }

.   {  }

%%

int yyerror(const char* msg){
  fprintf(stderr, "Lexer error: %s ,line_num=%d \n",msg, line_num);
  return 0;
}

