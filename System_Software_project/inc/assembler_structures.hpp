#include <string>
#include <vector>

using namespace std;

enum Relocation_type {
  ABS = 0,
  PC_REL = 1
};

struct forwardref {
  string section;
  int patch;
  Relocation_type rel_type;
};


struct relocation {
  int offset;
  Relocation_type rel_type;
  uint8_t id;
  int addend;
};

struct symbol {
  uint8_t id;
  int offset;
  bool is_section;
  bool global;
  uint8_t section;
  string name;
  bool defined;
  vector<forwardref> forward_table;
};

struct section {
  uint8_t id;
  string name;
  vector<uint8_t> data;
  vector<relocation> relocations;
};

enum OperationCode{
  OC_HALT = 0x00,
  OC_INT = 0x10,
  OC_IRET = 0x11,
  OC_CALL = 0x20,
  OC_RET = 0x12,
  OC_JMP = 0x30,
  OC_BEQ = 0x31,
  OC_BNE = 0x32,
  OC_BGT = 0x33,
  OC_PUSH = 0x13,
  OC_POP = 0x14,
  OC_XCHG = 0x40,
  OC_ADD = 0x50,
  OC_SUB = 0x51,
  OC_MUL = 0x52,
  OC_DIV = 0x53,
  OC_NOT = 0x60,
  OC_AND = 0x61,
  OC_OR = 0x62,
  OC_XOR = 0x63,
  OC_SHL = 0x70,
  OC_SHR = 0x71,
  OC_LD_IMM_LIT = 0x91,
  OC_LD_IMM_SYM = 0x92,
  OC_LD_ADDR_LIT = 0x93,
  OC_LD_ADDR_SYM = 0x94,
  OC_LD_REG = 0x95,
  OC_LD_ADDR_REG = 0x96,
  OC_LD_ADDR_REG_LIT = 0x97,
  OC_ST_ADDR_LIT = 0x81,
  OC_ST_ADDR_SYM = 0x82,
  OC_ST_REG = 0x83,
  OC_ST_ADDR_REG = 0x84,
  OC_ST_ADDR_REG_LIT = 0x85,
  OC_CSRRD = 0x90,
  OC_CSRWR = 0x80
};