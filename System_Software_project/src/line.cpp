#include "../inc/line.hpp"
#include <stdlib.h>
#include <stdio.h>

vector<line>* lines = nullptr;

void print_symbol_or_literal(struct symbol_or_literal elem){
    switch(elem.type){
      case NUM: cout << elem.num; break;
      case SYM: cout << elem.symbol; break;
      case STR: cout << "\"" << elem.str << "\""; break;
    }
}

void print_directive(struct directive dir){
    switch(dir.type){
      case GLOBAL: cout << "global "; break;
      case EXTERN: cout << "extern "; break;
      case SECTION: cout << "section "; break;
      case WORD: cout << "word "; break;
      case SKIP: cout << "skip "; break;
    }
    for(int i=0;i<dir.symbols.size;i++){
      if(i!=0) cout << ", ";
      print_symbol_or_literal(dir.symbols.data[i]);
    }
    
} 

void print_instruction_type(enum Instruction_type type){
  switch(type){
    case HALT: cout << "halt"; break;
    case INT: cout << "int"; break;
    case IRET: cout << "iret"; break;
    case CALL: cout << "call"; break;
    case RET: cout << "ret"; break;
    case JMP: cout << "jmp"; break;
    case BEQ: cout << "beq"; break;
    case BNE: cout << "bne"; break;
    case BGT: cout << "bgt"; break;
    case PUSH: cout << "push"; break;
    case POP: cout << "pop"; break;
    case XCHG: cout << "xchg"; break;
    case ADD: cout << "add"; break;
    case SUB: cout << "sub"; break;
    case MUL: cout << "mul"; break;
    case DIV: cout << "div"; break;
    case NOT: cout << "not"; break;
    case AND: cout << "and"; break;
    case OR: cout << "or"; break;
    case XOR: cout << "xor"; break;
    case SHL: cout << "shl"; break;
    case SHR: cout << "shr"; break;
    case LD: cout << "ld"; break;
    case ST: cout << "st"; break;
    case CSRRD: cout << "csrrd"; break;
    case CSRWR: cout << "csrwr"; break;
  }
}

void print_operand(struct operand opr){
    switch(opr.type){
      case IMM_LIT: cout << opr.num; break;
      case IMM_SYM: cout << opr.symbol; break;
      case ADDR_LIT: cout << "[" << opr.num << "]"; break;
      case ADDR_SYM: cout << "[" << opr.symbol << "]"; break;
      case REGISTER: cout << opr.reg; break;
      case ADDR_REG: cout << "[" << opr.reg << "]"; break;
      case ADDR_REG_LIT: cout << "[" << opr.reg << " + " << opr.num << "]"; break;
      case ADDR_REG_SYM: cout << "[" << opr.reg << " + " << opr.symbol << "]"; break;
    }
}

void print_instruction(struct instruction ins){
    switch(ins.type){
      case HALT:
      case INT:
      case IRET:
      case RET:
        print_instruction_type(ins.type);
        break;
      case CALL:
      case JMP:
        print_instruction_type(ins.type);
        cout << " ";
        print_operand(ins.operand);
        break;
      case BEQ:
      case BNE:
      case BGT:
        print_instruction_type(ins.type);
        cout << " " << ins.regS << ", " << ins.regD << ", ";
        print_operand(ins.operand);
        break;
      case PUSH:
      case POP:
      case NOT:
        print_instruction_type(ins.type);
        cout << " " << ins.regS;
        break;
      case XCHG:
      case ADD:
      case SUB:
      case MUL:
      case DIV:
      case AND:
      case OR:
      case XOR:
      case SHL:
      case SHR:
      case CSRRD:
      case CSRWR:
        print_instruction_type(ins.type);
        cout << " " << ins.regS << ", " << ins.regD;
        break;
      case LD:
        print_instruction_type(ins.type);
        print_operand(ins.operand);
        cout << ", " << ins.regD;
        break;
      case ST:
        print_instruction_type(ins.type);
        cout << " " << ins.regS << ", ";
        print_operand(ins.operand);
    }
}

void print_line(struct line line){
    if(line.label!=NULL){
      cout << line.label << ": ";
    }
    switch(line.type){
      case EMPTY: break;
      case DIRECTIVE: print_directive(line.directive); break;
      case INSTRUCTION: print_instruction(line.instruction); break;
    }
    cout << "\n";
}