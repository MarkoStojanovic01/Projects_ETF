#include <vector>
#include <iostream>

using namespace std;

enum Directive_type {
    GLOBAL,
    EXTERN,
    SECTION,
    WORD,
    SKIP
};

enum Symbol_or_literal_type{
    SYM,
    NUM,
    STR
  };

struct symbol_or_literal {
  union {
    char* symbol;
    int num;
    char* str;
  };

  Symbol_or_literal_type type;
};

struct symbols {
  symbol_or_literal* data;
  int size;
};

struct directive {
  Directive_type type;
  struct symbols symbols;
};

enum Instruction_type {
    HALT,
    INT,
    IRET,
    CALL,
    RET,
    JMP,
    BEQ,
    BNE,
    BGT,
    PUSH,
    POP,
    XCHG,
    ADD,
    SUB,
    MUL,
    DIV,
    NOT,
    AND,
    OR,
    XOR,
    SHL,
    SHR,
    LD,
    ST,
    CSRRD,
    CSRWR
};

enum Operand_type {
    IMM_LIT,
    IMM_SYM,
    ADDR_LIT,
    ADDR_SYM,
    REGISTER,
    ADDR_REG,
    ADDR_REG_LIT,
    ADDR_REG_SYM
  };

struct operand {
  Operand_type type;

  union {
    int num;
    char* symbol;
  };
  int reg;
};

struct instruction {
  Instruction_type type;
  struct operand operand;
  int regS;
  int regD;
};

enum Line_type{
    EMPTY,
    DIRECTIVE,
    INSTRUCTION
  };

struct line {
  char* label;
  Line_type type;
  union {
    struct directive directive;
    struct instruction instruction;
  };
};

void print_line(struct line line);

extern vector<line>* lines;
