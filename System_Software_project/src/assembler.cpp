#include "../inc/assembler.hpp"


void Assembler::addByteToSectionCode(uint8_t byte){
  currentSection.data.push_back(byte);
}

void Assembler::addOperationCode(OperationCode oc){
  currentSection.data.push_back(oc & 0xFF);
  locationCounter += 1;
}

void Assembler::addWordToSectionCode(int num){
  currentSection.data.push_back(num & 0xFF);
  currentSection.data.push_back((num >> 8) & 0xFF);
  currentSection.data.push_back((num >> 16) & 0xFF);
  currentSection.data.push_back((num >> 24) & 0xFF);
}

void Assembler::addOffsetToSectionCode(int num){
  currentSection.data.push_back(num & 0xFF);
  currentSection.data.push_back((num >> 8) & 0xFF);
}

void Assembler::writeSymbol(char* sym_name,Relocation_type rel_type){
  bool symbol_found = false;
  symbol s;
  int j;
  for(j=0;j<symbol_table.size();j++){
    if(symbol_table.at(j).name == sym_name){
      symbol_found = true;
      s = symbol_table.at(j);
      break;
    }
  }
  if(!symbol_found){
    s.id = currentId++;
    s.offset = 0;
    s.is_section = false;
    s.global = false;
    s.section = -1;
    s.name = sym_name;
    s.defined = false;
    forwardref f;
    f.patch = locationCounter;
    f.rel_type = rel_type;
    f.section = currentSection.name;
    s.forward_table.push_back(f);
    symbol_table.push_back(s);
    // symbol must be defined by the end of assembler
    undefined_symbols.push_back(s.name);

    addWordToSectionCode(0);

    return;
  }
  if(!s.defined){
    // add another patch to forward_table
    forwardref f;
    f.patch = locationCounter;
    f.rel_type = rel_type;
    f.section = currentSection.name;
    s.forward_table.push_back(f);
    symbol_table.at(j) = s;
  }
  else{
    if(s.section == currentSectionId && rel_type == PC_REL){
      int offset = s.offset - locationCounter - 4;
      addWordToSectionCode(offset);
      return;
    }
    // add relocation
    relocation r;
    r.offset = locationCounter;
    r.rel_type = rel_type;
    if(s.global){
      r.id = s.id;
      r.addend = 0;
    }
    else{
      r.id = s.section;
      r.addend = s.offset;
    }
    if(rel_type == PC_REL){
      r.addend -= 4;
    }
    currentSection.relocations.push_back(r);
  }
  addWordToSectionCode(0);
}

int Assembler::assemble(vector<line>* lines){

  for(line line: lines[0]){
    
    // line l = lines->at(i);

    // LABEL

    if(line.label){
      // cout << "Adding Label ";
      if(currentSectionId==0xFF){
            cout << "Label must be in section\n";
            return -1;
          }
      // label already exists
      bool label_found = false;
      for(int i=0; i<symbol_table.size();i++){
        symbol s = symbol_table.at(i);
        if(s.name == line.label){
          if(s.defined){
            cout << "Label already defined\n";
            return -1;
          }
          
          label_found = true;

          s.defined = true;
          s.offset = locationCounter;
          s.section = currentSectionId;
  
          symbol_table.at(i) = s;
          
          // erase symbol from undefined symbols
          for(int in=0; in<undefined_symbols.size();in++){
            if(undefined_symbols.at(in)==s.name){
              undefined_symbols.erase(undefined_symbols.begin()+in);
              break;
            }
          }
          // backpathcing
          for(forwardref f:s.forward_table){
            // fixing in section code
            if(f.section == currentSection.name && f.rel_type == PC_REL){
              uint64_t offset = locationCounter - f.patch - 4;
              int iter = f.patch;
              currentSection.data.at(iter++) = (offset & 0xFF);
              currentSection.data.at(iter++) = (offset >> 8 & 0xFF);
              currentSection.data.at(iter++) = (offset >> 16 & 0xFF);
              currentSection.data.at(iter) = (offset >> 24 & 0xFF);
              continue;
            }
            // add reloaction
            relocation r;
            r.offset = f.patch;
            r.rel_type = f.rel_type;
            if(s.global){
              r.id = s.id;
              r.addend = 0;
            }
            else{
              r.id = s.section;
              r.addend = s.offset;
            }
            if(r.rel_type == PC_REL){
              r.addend -= 4;
            }
            if(f.section == currentSection.name){
              currentSection.relocations.push_back(r);
            }
            else{
              for(int j=0;j<sections.size();j++){
                section sec = sections.at(j);
                if(sec.name == f.section){
                  sec.relocations.push_back(r);
                  sections.at(j) = sec;
                  break;
                }
              }
            }
          }
          break;
        }

      }
      // add new label
      if(!label_found){
        symbol s;
        s.id = currentId++;
        s.offset = locationCounter;
        s.is_section = false;
        s.global = false;
        s.section = currentSectionId;
        s.name = line.label;
        s.defined = true;
        

        symbol_table.push_back(s);
      }
    }

    // DIRECTIVE

    if(line.type == DIRECTIVE){
      switch(line.directive.type){
        
        case GLOBAL:
        // cout << "Global Directive\n";
          for(int i=0; i<line.directive.symbols.size; i++){
            string symbol_name = line.directive.symbols.data[i].symbol;

            // symbol already exists
            bool symbol_found = false;
            for(int j=0;j<symbol_table.size();j++){
              symbol s = symbol_table.at(j);
              if(symbol_name == s.name){
                symbol_found = true;
                s.global = true;
                symbol_table.at(j) = s;
              }
            }
            // add new symbol
            if(!symbol_found){
              symbol s;
              s.id = currentId++;
              s.offset = 0;
              s.is_section = false;
              s.global = true;
              s.section = -1;
              s.name = symbol_name;
              s.defined = false;
              
              symbol_table.push_back(s);

              // symbol must be defined by the end of assembler
              undefined_symbols.push_back(s.name);
            }
          }
          break;
        case EXTERN:
        // cout << "Extern Directive\n";
          for(int i=0; i<line.directive.symbols.size; i++){
            string symbol_name = line.directive.symbols.data[i].symbol;

            // symbol already exists
            bool symbol_found = false;
            for(int j=0;j<symbol_table.size();j++){
              symbol s = symbol_table.at(j);
              if(symbol_name == s.name){
                if(s.defined){
                  cout << "Extern symbol cant be defined\n";
                  return -1;
                }
                symbol_found = true;
                s.defined = true;
                s.global = true;
                symbol_table.at(j) = s;

                // undefined extern symbol shouldn't throw error at the end of assembler
                for(int in=0; in<undefined_symbols.size();in++){
                  if(undefined_symbols.at(in)==s.name){
                    undefined_symbols.erase(undefined_symbols.begin()+in);
                    break;
                  }
          }
              }
            }
            // add new symbol
            if(!symbol_found){
              symbol s;
              s.id = currentId++;
              s.offset = 0;
              s.is_section = false;
              s.global = true;
              s.section = -1;
              s.name = symbol_name;
              s.defined = true;
              
              symbol_table.push_back(s);
            }
          }
          break;
        case SECTION:{
          // cout << "Section Directive\n";
          if(currentSectionId!=0xFF){
            sections.push_back(currentSection);
          }
          for(section s:sections){
            if(s.name == line.directive.symbols.data[0].symbol){
              cout << " Section: " << s.name << " already exists\n";
              return -1;
            }
          }
          section sec;
          sec.id = currentId++;
          sec.name = line.directive.symbols.data[0].symbol;
          currentSection = sec;
          currentSectionId = sec.id;
          locationCounter = 0;


          symbol s;
          s.id = sec.id;
          s.offset = 0;
          s.is_section = true;
          s.global = false;
          s.section = sec.id;
          s.name = sec.name;
          s.defined = true;
          
          symbol_table.push_back(s);
          
          break;
        }
        case WORD:
        // cout << "Word Directive\n";
          if(currentSectionId==0xFF){
            cout << "Word must be allocated in section\n";
            return -1;
          }
          for(int i=0;i<line.directive.symbols.size;i++){
            
            switch(line.directive.symbols.data[i].type){
              case SYM:
              {
                writeSymbol(line.directive.symbols.data[i].symbol, ABS);
                break;
              }
              case NUM:
                addWordToSectionCode(line.directive.symbols.data[i].num);
                break;
            }

            locationCounter += 4;
          }
          break;
          
          case SKIP:
          // cout << "Skip Directive\n";
            if(currentSectionId==0xFF){
              cout << "Skip must be in section\n";
              return -1;
            }
            int num = line.directive.symbols.data[0].num;
            for(int i=0;i<num;i++){
              addByteToSectionCode(0);
            }
            locationCounter += num;
      }
    }
    else if(line.type == INSTRUCTION){
      // cout << "Instruction: " << line.instruction.type <<  "\n";
      if(currentSectionId==0xFF){
            cout << "Instruction must be in section\n";
            return -1;
          }
      switch(line.instruction.type){
        case HALT: addOperationCode(OC_HALT); break;
        case INT: addOperationCode(OC_INT); break;
        case IRET: addOperationCode(OC_IRET); break;
        case RET: addOperationCode(OC_RET); break;
        case CALL:
          switch(line.instruction.operand.type){
            case ADDR_LIT:
              addOperationCode(OC_CALL);
              addWordToSectionCode(line.instruction.operand.num);
              break;
            case ADDR_SYM:{
              addOperationCode(OC_CALL);

              writeSymbol(line.instruction.operand.symbol,ABS);
              break;
            }
            default:
              cout << "Wrong operand type for Call function\n";
              return -1;
          }
          locationCounter += 4;
          break;

        case JMP:
        case BEQ:
        case BNE:
        case BGT:
          switch(line.instruction.operand.type){
            case ADDR_LIT:
              switch(line.instruction.type){
                case JMP: addOperationCode(OC_JMP); break;
                case BEQ: addOperationCode(OC_BEQ); break;
                case BNE: addOperationCode(OC_BNE); break;
                case BGT: addOperationCode(OC_BGT); break;
              }
              if(line.instruction.type != JMP){
                addByteToSectionCode((line.instruction.regS << 4) | line.instruction.regD);
                locationCounter += 1;
              }
              addWordToSectionCode(line.instruction.operand.num);
              locationCounter += 4;
              break;
            case ADDR_SYM:{
              switch(line.instruction.type){
                case JMP: addOperationCode(OC_JMP); break;
                case BEQ: addOperationCode(OC_BEQ); break;
                case BNE: addOperationCode(OC_BNE); break;
                case BGT: addOperationCode(OC_BGT); break;
              }
              if(line.instruction.type != JMP){
                addByteToSectionCode((line.instruction.regS << 4) | line.instruction.regD);
                locationCounter += 1;
              }
              writeSymbol(line.instruction.operand.symbol,PC_REL);
              locationCounter += 4;
              break;
            }
            default:
              cout << "Wrong operand type for Jump function\n";
              return -1;
            }
          break;
        case PUSH:
        case POP:
        case NOT:
          switch(line.instruction.type){
            case PUSH: addOperationCode(OC_PUSH); break;
            case POP: addOperationCode(OC_POP); break;
            case NOT: addOperationCode(OC_NOT); break;
          }
          addByteToSectionCode(line.instruction.regS);
          locationCounter += 1;
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
          switch(line.instruction.type){
            case XCHG: addOperationCode(OC_XCHG); break;
            case ADD: addOperationCode(OC_ADD); break;
            case SUB: addOperationCode(OC_SUB); break;
            case MUL: addOperationCode(OC_MUL); break;
            case DIV: addOperationCode(OC_DIV); break;
            case AND: addOperationCode(OC_AND); break;
            case OR: addOperationCode(OC_OR); break;
            case XOR: addOperationCode(OC_XOR); break;
            case SHL: addOperationCode(OC_SHL); break;
            case SHR: addOperationCode(OC_SHR); break;
            case CSRRD: addOperationCode(OC_CSRRD); break;
            case CSRWR: addOperationCode(OC_CSRWR); break;
          }
          addByteToSectionCode((line.instruction.regS << 4) | line.instruction.regD);
          locationCounter += 1;
          break;
        case LD:
          switch(line.instruction.operand.type){
            case IMM_LIT:
              addOperationCode(OC_LD_IMM_LIT);
              addWordToSectionCode(line.instruction.operand.num);
              locationCounter += 4;
              break;
            case ADDR_LIT:
              addOperationCode(OC_LD_ADDR_LIT);
              addWordToSectionCode(line.instruction.operand.num);
              locationCounter += 4;
              break;
            case ADDR_SYM:
            case IMM_SYM:{
              switch(line.instruction.operand.type){
                case ADDR_SYM: addOperationCode(OC_LD_ADDR_SYM); break;
                case IMM_SYM: addOperationCode(OC_LD_IMM_SYM); break;
              }
            
              writeSymbol(line.instruction.operand.symbol,ABS);
              locationCounter += 4;
              break;
            }
            case REGISTER:
              addOperationCode(OC_LD_REG);
              addByteToSectionCode(line.instruction.operand.reg);
              locationCounter += 1;
              break;
            case ADDR_REG:
              addOperationCode(OC_LD_ADDR_REG);
              addByteToSectionCode(line.instruction.operand.reg);
              locationCounter += 1;
              break;
            case ADDR_REG_LIT:
              
              addOperationCode(OC_LD_ADDR_REG_LIT);
              if(line.instruction.operand.num > 1<<11 || line.instruction.operand.num < -(1<<11)){
                cout << "Size of literal must be in range od 12 bits in Register indirect + literal";
                return -1;
              }
              addByteToSectionCode((line.instruction.operand.reg << 4) | (line.instruction.operand.num & 0xFF) );
              addByteToSectionCode(((line.instruction.operand.num >>4) & 0xFF) | ((line.instruction.operand.num >> 4) & 0xFF00));
              locationCounter += 2;
              break;
            default:
              cout << "Wrond operand type for Load instruction\n";
              return -1;
          }
          addByteToSectionCode(line.instruction.regD);
          locationCounter += 1;
          break;

        case ST:
          switch(line.instruction.operand.type){
            case ADDR_LIT:
              addOperationCode(OC_ST_ADDR_LIT);
              addByteToSectionCode(line.instruction.regS);
              locationCounter += 1;
              addWordToSectionCode(line.instruction.operand.num);
              locationCounter += 4;
              break;
            case ADDR_SYM:{
              addOperationCode(OC_ST_ADDR_SYM);
              addByteToSectionCode(line.instruction.regS);
              locationCounter += 1;
              writeSymbol(line.instruction.operand.symbol,ABS);
              locationCounter += 4;
              break;
            }
            case REGISTER:
              addOperationCode(OC_ST_REG);
              addByteToSectionCode(line.instruction.regS);
              locationCounter += 1;
              addByteToSectionCode(line.instruction.operand.reg);
              locationCounter += 1;
              break;
            case ADDR_REG:
              addOperationCode(OC_ST_ADDR_REG);
              addByteToSectionCode(line.instruction.regS);
              locationCounter += 1;
              addByteToSectionCode(line.instruction.operand.reg);
              locationCounter += 1;
              break;
            case ADDR_REG_LIT:
              addOperationCode(OC_ST_ADDR_REG_LIT);
              addByteToSectionCode(line.instruction.regS);
              locationCounter += 1;
              addByteToSectionCode((line.instruction.operand.reg << 4) | (line.instruction.operand.num & 0xFF) );
              addByteToSectionCode(((line.instruction.operand.num >>4) & 0xFF) | ((line.instruction.operand.num >> 4) & 0xFF00));
              locationCounter += 2;
              break;
            default:
              cout << "Wrond operand type for Load instruction\n";
              return -1;
          }
          break;
      }
    }
  }
  // add last section
  if(currentSectionId!=0xFF){
    sections.push_back(currentSection);
  }
  if(undefined_symbols.size() > 0){
    cout << "Error: Undefined symbols: ";
    for(string sym:undefined_symbols){
      cout << sym << ", ";
    }
    cout << endl;
    return -1;
  }
  return 0;
}

template<typename T>
void binary_write(ostream& binary_file, T code){
  binary_file.write((char*)&code, sizeof(code));
}

void Assembler::createOutputFiles(string output_file){
  std::ofstream txt_file;
  txt_file.open(output_file.substr(0,output_file.size()-2)+".txt");

  std::ofstream binary_file(output_file, ios::out | ios::binary);

  // print symbol table
  txt_file << "#.symtab\n";
  txt_file << "Id\tOffset\t\t\tType\tBind\tSection\tName\n";

  // simbol_table size
  int symtab_size = symbol_table.size();
  binary_write(binary_file, symtab_size);
  for(symbol sym:symbol_table){
    txt_file << static_cast<int>(sym.id) << ":\t" << std::setfill('0') << std::setw(8) << std::hex;
    binary_write(binary_file,sym.id);

    txt_file << sym.offset << std::dec << "\t\t";
    binary_write(binary_file,sym.offset);

    if(sym.is_section){
      txt_file << "SCTN\t";
    }
    else{
      txt_file << "NOTYP\t";
    }
    binary_write(binary_file,sym.is_section);

    if(sym.global){
      txt_file << "GLOB\t";
    }
    else{
      txt_file << "LOC  \t";
    }
    binary_write(binary_file,sym.global);

    if(sym.section != 0xFF){
      txt_file << static_cast<int>(sym.section) << "\t\t\t";
    }
    else{
      txt_file << "UND\t\t";
    }
    binary_write(binary_file,sym.section);

    txt_file << sym.name << endl;
    int sym_name_len = sym.name.length();
    binary_write(binary_file, sym_name_len);
    binary_file.write(sym.name.c_str(), sym_name_len);
  }

  //print relocations
  txt_file << "\nRelocations\n";

  // sections num
  int sections_num = sections.size();
  binary_write(binary_file,sections_num);
  for(section sec:sections){
    txt_file << "#.rela." << sec.name << endl;
    txt_file << "Offset\t\t\tType\tSymbol\tAddend\n";

    // section and relocations size
    int sec_name_len = sec.name.length();
    binary_write(binary_file,sec_name_len);
    binary_file.write(sec.name.c_str(), sec_name_len);
    int sec_relocations_size = sec.relocations.size(); 
    binary_write(binary_file, sec_relocations_size);

    for(relocation rel:sec.relocations){
      txt_file << std::setfill('0') << std::setw(8) << std::hex << rel.offset << std::dec << "\t\t";
      binary_write(binary_file,rel.offset);

      if(rel.rel_type == ABS){
        txt_file << "ABS\t\t\t";
        bool pc_rel = false;
        binary_write(binary_file, pc_rel);
      }
      else{
        txt_file << "PC_REL\t";
        bool pc_rel = true;
        binary_write(binary_file, pc_rel);
      }
      // binary_write(binary_file,rel.rel_type);

      txt_file << static_cast<int>(rel.id) << "\t\t\t\t";
      binary_write(binary_file,rel.id);

      txt_file << std::setfill('0') << std::setw(4) << std::hex << rel.addend << std::dec << endl;
      binary_write(binary_file,rel.addend);
    }
  }

  //print code
  txt_file << "\nCode";

  for(section sec:sections){
    txt_file << "\n#." << sec.name;

    // section and code size
    int sec_name_len = sec.name.length();
    binary_write(binary_file,sec_name_len);
    binary_file.write(sec.name.c_str(), sec_name_len);
    int sec_data_size = sec.data.size();
    binary_write(binary_file, sec_data_size);

    int i = 0;
    for(uint8_t byte:sec.data){
      if(i%8==0){
        txt_file << endl;
      }
      txt_file << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << std::dec << "\t";
      binary_write(binary_file,byte);
      i++;
    }
  }

  txt_file.close();
  binary_file.close();

}


