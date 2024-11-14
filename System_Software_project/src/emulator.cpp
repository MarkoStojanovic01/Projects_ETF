#include "../inc/emulator.hpp"
#include <iostream>
#include "../inc/assembler_structures.hpp"

Emulator::Emulator(){
  pc = 0x40000000;
}

bool Emulator::readFromHexFile(string hex_file_name){

  ifstream hex_file(hex_file_name);

  if(!hex_file.is_open()){
    return false;
  }

  while(!hex_file.eof()){
    int i=0;
    vector<uint8_t> bytes;
    uint64_t addr;
    while(i<8 && !hex_file.eof()){
      if(i==0){
        hex_file >> hex >> addr;
        // read ":"
        char c;
        hex_file >> c;
      }
      uint64_t data;
      hex_file >> hex >> data;
      uint8_t byte = (uint8_t)data;
      bytes.push_back(byte);
      i++;
    }
    memory[(uint32_t)addr] = bytes;
  }

  hex_file.close();

  return true;
}

uint8_t Emulator::readByte(uint32_t addr){
  int offset = addr % 8;
  uint32_t mem_addr = addr - offset;
  auto it = memory.find(mem_addr);
  if(it != memory.end()){
    return it->second.at(offset);
  }
  return 0;
}

uint32_t Emulator::readInt(uint32_t addr){
  uint32_t bytes[4] = {0};
  for(int i=0; i<4;i++){
    bytes[i] = readByte(addr+i);
  }
  uint32_t ret = (bytes[0]) | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
  return ret;
}

void Emulator::writeByte(uint32_t addr, uint8_t byte){
  // cout << "writeByte: addr=" << std::hex << static_cast<int>(addr) << " ,byte=" << std::hex << static_cast<int>(byte) << endl;
  int offset = addr % 8;
  uint32_t mem_addr = addr - offset;
  auto it = memory.find(mem_addr);
  if(it != memory.end()){
    memory[mem_addr].at(offset) = byte;
  }
  else{
    vector<uint8_t> bytes;
    for(int i=0; i<8; i++){
      if(i==offset){
        bytes.push_back(byte);
      }
      else{
        bytes.push_back(0);
      }
    }
    memory[mem_addr] = bytes;
  }
}

void Emulator::writeInt(uint32_t addr, uint32_t word){
  for(int i=0; i<4; i++){
    writeByte(addr+i, (word>>i*8)&0xFF);
  }
}

bool Emulator::emulate(){

  bool running = true;

  // std::ofstream txt_file;
  // txt_file.open("emulator_chec.txt");

  while(running){
    uint8_t instruction = readByte(pc++);

    switch(instruction){
      case OC_HALT:{
        running = false;
        break;
      }
      case OC_INT:{
        // push status register
        sp -= 4;
        writeInt(sp,status);
        // push pc
        sp -= 4;
        writeInt(sp,pc);

        cause = 4;
        status &= ~0x1;
        pc = handler;
        break;
      }
      case OC_IRET:{
        // pop pc
        pc = readInt(sp);
        sp += 4;
        // pop status register
        status = readInt(sp);
        sp += 4;
        break;
      }
      case OC_CALL:{
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;
        // push pc
        sp -= 4;
        writeInt(sp,pc);
        pc = operand;
        break;
      }
      case OC_RET:{
        // pop pc
        pc = readInt(sp);
        sp += 4;
        break;
      }
      case OC_JMP:{
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;
        pc += operand;
        break;
      }
      case OC_BEQ:{
        // read reg1 and reg2
        uint8_t reg12 = readByte(pc++);
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;

        if(gprs[(reg12>>4)&0xF] == gprs[reg12&0xF]){
          pc += operand;
        }
        break;
      }
      case OC_BNE:{
        // read reg1 and reg2
        uint8_t reg12 = readByte(pc++);
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;

        if(gprs[(reg12>>4)&0xF] != gprs[reg12&0xF]){
          pc += operand;
        }
        break;
      }
      case OC_BGT:{
        // read reg1 and reg2
        uint8_t reg12 = readByte(pc++);
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;

        if(gprs[(reg12>>4)&0xF] > gprs[reg12&0xF]){
          pc += operand;
        }
        break;
      }
      case OC_PUSH:{
        // read register
        uint8_t reg = readByte(pc++);
        // push reg
        sp -= 4;
        writeInt(sp, gprs[reg]);
        break;
      }
      case OC_POP:{
        // read register
        uint8_t reg = readByte(pc++);
        // pop reg
        gprs[reg] = readInt(sp);
        sp += 4;
        break;
      }
      case OC_XCHG:
      case OC_ADD:
      case OC_SUB:
      case OC_MUL:
      case OC_DIV:
      case OC_AND:
      case OC_OR:
      case OC_XOR:
      case OC_SHL:
      case OC_SHR:
      case OC_CSRRD:
      case OC_CSRWR:{
        // read regS and regD
        uint8_t regS_D = readByte(pc++);
        uint8_t regS = (regS_D>>4)&0xF;
        uint8_t regD = regS_D&0xF;
        switch(instruction){
          case OC_XCHG:{
            int temp = gprs[regD];
            gprs[regD] = gprs[regS];
            gprs[regS] = temp;
            break;
          }
          case OC_ADD:
            gprs[regD] += gprs[regS];
            break;
          case OC_SUB:
            gprs[regD] -= gprs[regS];
            break;
          case OC_MUL:
            gprs[regD] *= gprs[regS];
            break;
          case OC_DIV:
            gprs[regD] /= gprs[regS];
            break;
          case OC_AND:
            gprs[regD] &= gprs[regS];
            break;
          case OC_OR:
            gprs[regD] |= gprs[regS];
            break;
          case OC_XOR:
            gprs[regD] ^= gprs[regS];
            break;
          case OC_SHL:
            gprs[regD] <<= gprs[regS];
            break;
          case OC_SHR:
            gprs[regD] >>= gprs[regS];
            break;
          case OC_CSRRD:
            gprs[regD] = csrs[regS];
            break;
          case OC_CSRWR:
            csrs[regD] = gprs[regS];
            break;
        }
        break;
      }
      case OC_NOT:{
        // read register
        uint8_t reg = readByte(pc++);
        gprs[reg] = ~gprs[reg];
        break;
      }
      case OC_LD_IMM_LIT:
      case OC_LD_IMM_SYM:{
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;
        // read reg
        uint8_t reg = readByte(pc++);

        gprs[reg] = operand;
        break;
      }
      case OC_LD_ADDR_LIT:
      case OC_LD_ADDR_SYM:{
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;
        // read reg
        uint8_t reg = readByte(pc++);

        gprs[reg] = readInt(operand);
        break;
      }
      case OC_LD_REG:{
        // read reg_operand
        uint8_t reg_opr = readByte(pc++);
        // read reg
        uint8_t reg = readByte(pc++);

        gprs[reg] = gprs[reg_opr];
        break;
      }
      case OC_LD_ADDR_REG:{
        // read reg_operand
        uint8_t reg_opr = readByte(pc++);
        // read reg
        uint8_t reg = readByte(pc++);

        gprs[reg] = readInt(gprs[reg_opr]);
        break;
      }
      case OC_LD_ADDR_REG_LIT:{
        // read reg_operand and operand[0-3]
        int literal = 0;
        uint8_t first_byte = readByte(pc++);
        uint8_t reg_opr = (first_byte>>4)&0xF;
        literal = literal | (first_byte&0xF);
        // read operand[4-11]
        uint8_t second_byte = readByte(pc++);
        literal = literal | (second_byte << 4);
        // read reg
        uint8_t reg = readByte(pc++);

        gprs[reg] = readInt(gprs[reg_opr]+literal);
        break;
      }
      case OC_ST_ADDR_LIT:
      case OC_ST_ADDR_SYM:{
        // read reg
        uint8_t reg = readByte(pc++);
        // read operand
        uint32_t operand = readInt(pc);
        pc += 4;

        writeInt(operand, gprs[reg]);
        break;
      }
      case OC_ST_REG:{
        // read reg
        uint8_t reg = readByte(pc++);
        // read reg_operand
        uint8_t reg_opr = readByte(pc++);

        gprs[reg_opr] = gprs[reg];
        break;
      }
      case OC_ST_ADDR_REG:{
        // read reg
        uint8_t reg = readByte(pc++);
        // read reg_operand
        uint8_t reg_opr = readByte(pc++);

        writeInt(gprs[reg_opr],gprs[reg]);
        break;
      }
      case OC_ST_ADDR_REG_LIT:{
        // read reg
        uint8_t reg = readByte(pc++);
        // read reg_operand and operand[0-3]
        int literal = 0;
        uint8_t first_byte = readByte(pc++);
        uint8_t reg_opr = (first_byte>>4)&0xF;
        literal = literal | (first_byte&0xF);
        // read operand[4-11]
        uint8_t second_byte = readByte(pc++);
        literal = literal | (second_byte << 4);

        writeInt(gprs[reg_opr]+literal,gprs[reg]);
        break;
      }
    }
    /*
    txt_file << "Instruction: " << setfill('0') << setw(2) << std::hex << static_cast<int>(instruction) << endl;
    for(int i=0;i<16;i++){
      txt_file << "r" << i << "=0x" << setfill('0') << setw(8) << std::hex << gprs[i] << endl;
    }
    txt_file << endl;
    */
  }

  return true;
}

void Emulator::printState(){
  for(int i=0;i<16;i++){
    cout << "r" << i << "=0x" << setfill('0') << setw(8) << std::hex << gprs[i] << endl;
  }
}

void Emulator::printMemory(){

  std::ofstream txt_file;
  txt_file.open("emulator_memory.txt");

  for(map<uint32_t, vector<uint8_t>>::iterator it = memory.begin();it!=memory.end();it++){
      txt_file << setfill('0') << setw(8) << std::hex << it->first << ":";
      for(int i=0; i<8;i++){
        txt_file << " " << setfill('0') << setw(2) << std::hex << static_cast<int>(readByte(it->first+i));
      }
      txt_file << endl;
    }

  txt_file.close();
}