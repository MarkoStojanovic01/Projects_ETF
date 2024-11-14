#include "../inc/linker.hpp"

int Linker::find_section(string sec_name, vector<section> sections){
  for(int i=0;i<sections.size();i++){
    if(sections.at(i).name == sec_name){
      return i;
    }
  }
  return -1;
}

int Linker::find_section(string sec_name, vector<section_map> sections_map){
  for(int i=0;i<sections_map.size();i++){
    if(sections_map.at(i).section_name == sec_name){
      return i;
    }
  }
  return -1;
}

bool Linker::read_input_files(vector<string> input_files){

  for(string input_file_name:input_files){
    ifstream input_file(input_file_name, ios::binary);
    if(!input_file){
      cout << input_file_name << " file cant open\n";
      return false;
    }
    file_data fd;
    fd.file_name = input_file_name;
    // read symbol_table
    int sym_num;
    input_file.read((char*)&sym_num, sizeof(sym_num));
    for(int i=0;i<sym_num;i++){
      symbol sym;

      input_file.read((char*)&sym.id, sizeof(sym.id));

      input_file.read((char*)&sym.offset, sizeof(sym.offset));

      input_file.read((char*)&sym.is_section, sizeof(sym.is_section));

      input_file.read((char*)&sym.global, sizeof(sym.global));

      input_file.read((char*)&sym.section, sizeof(sym.section));

      int sym_name_len;
      input_file.read((char*)&sym_name_len, sizeof(sym_name_len));
      sym.name.resize(sym_name_len);
      input_file.read((char*)sym.name.c_str(), sym_name_len);

      fd.symbol_table.push_back(sym);

      if(sym.is_section){
        section sec;
        sec.id = sym.id;
        sec.name = sym.name;
        fd.sections.push_back(sec);
      }
    }
    // read relocations
    // sections num
    int sections_num;
    input_file.read((char*)&sections_num, sizeof(sections_num));
    for(int i=0;i<sections_num;i++){
      
      int sec_name_len;
      string sec_name;
      input_file.read((char*)&sec_name_len, sizeof(sec_name_len));
      sec_name.resize(sec_name_len);
      input_file.read((char*)sec_name.c_str(), sec_name_len);
      int sec_relocations_size; 
      input_file.read((char*)&sec_relocations_size, sizeof(sec_relocations_size));

      int sec_position = find_section(sec_name, fd.sections);
      section sec = fd.sections.at(sec_position);

      for(int j=0;j<sec_relocations_size;j++){

        relocation rel;
        
        input_file.read((char*)&rel.offset, sizeof(rel.offset));

        bool pc_rel;
        input_file.read((char*)&pc_rel, sizeof(pc_rel));
        if(pc_rel){
          rel.rel_type = PC_REL;
        }
        else{
          rel.rel_type = ABS;
        }
        
        input_file.read((char*)&rel.id, sizeof(rel.id));

        input_file.read((char*)&rel.addend, sizeof(rel.addend));

        sec.relocations.push_back(rel);
      }
      fd.sections.at(sec_position) = sec;
    }
    //read code
    for(int i=0;i<fd.sections.size();i++){

      int sec_name_len;
      string sec_name;
      input_file.read((char*)&sec_name_len, sizeof(sec_name_len));
      sec_name.resize(sec_name_len);
      input_file.read((char*)sec_name.c_str(), sec_name_len);
      int sec_data_size;
      input_file.read((char*)&sec_data_size, sizeof(sec_data_size));

      int sec_position = find_section(sec_name, fd.sections);
      section sec = fd.sections.at(sec_position);


      for(int j=0;j<sec_data_size;j++){
        uint8_t byte;
        input_file.read((char*)&byte, sizeof(byte));
        sec.data.push_back(byte);
      }
      fd.sections.at(sec_position) = sec;

    }

    files_data.push_back(fd);

    input_file.close();
  }

  return true;
}

bool Linker::mapping_sections(map<string, uint64_t> places){

  vector<section_map> all_sections;
  // merge all sections
  for(file_data fd:files_data){
    for(section sec:fd.sections){
      int pos = find_section(sec.name, all_sections);
      if(pos != -1){
        section_map s = all_sections.at(pos);
        s.files_addr[fd.file_name] = s.code.size();
        for(uint8_t byte:sec.data){
          s.code.push_back(byte);
        }
        all_sections.at(pos) = s;
      }
      else{
        section_map s;
        s.section_name = sec.name;
        s.code = sec.data;
        s.files_addr[fd.file_name] = 0;
        all_sections.push_back(s);
      }
    }
  }
  uint64_t next_address = 0;
  // add first sections mentioned in place option
  for(map<string,uint64_t>::iterator it = places.begin();it!=places.end();it++){
    string sec_name = it->first;
    uint64_t address = it->second;
    int pos = find_section(sec_name,all_sections);
    if(pos==-1){
      cout << "Section: " << sec_name << " in place option doesn't exist\n";
      return false;
    }
    section_map sm;
    sm = all_sections.at(pos);
    sm.address = address;
    if(address + sm.code.size() > next_address){
      next_address = address + sm.code.size();
    }
    bool sm_inserted = false;
    for(int i=0;i<maped_sections.size();i++){
      if(maped_sections.at(i).address > address){
        maped_sections.insert(maped_sections.begin()+i,sm);
        sm_inserted = true;
        break;
      }
    }
    if(!sm_inserted){
      maped_sections.push_back(sm);
    }
    // remove added section from all_sections
    all_sections.erase(all_sections.begin()+pos);
  }
  // add left sections
  for(section_map sec:all_sections){
    section_map sm;
    sm = sec;
    sm.address = next_address;
    next_address += sm.code.size();
    maped_sections.push_back(sm);
  }
  // checking if there is intersection between sections
  for(int i=0;i<maped_sections.size()-1;i++){
    section_map sm_curr = maped_sections.at(i);
    section_map sm_next = maped_sections.at(i+1);
    if(sm_curr.address+sm_curr.code.size()>sm_next.address){
      cout << "Sections: " << sm_curr.section_name << " and " << sm_next.section_name << " are intersected\n";
      return false;
    }
  }

  return true;
}

int find_symbol(string sym_name, vector<symbol> symbols){
  for(int i=0;i<symbols.size();i++){
    if(symbols.at(i).name == sym_name){
      return i;
    }
  }
  return -1;
}

bool Linker::merge_symbols(){

  int curr_id = 0;
  // first add sections
  for(section_map sm:maped_sections){
    symbol s;
    s.id = curr_id++;
    s.name = sm.section_name;
    s.defined = true;
    s.global = false;
    s.is_section = true;
    s.offset = sm.address;
    s.section = s.id;
    symbol_table.push_back(s);
  }
  // extern symbols
  vector<string> extern_symbols;
  for(file_data fd:files_data){
    for(symbol sym:fd.symbol_table){
      if(sym.is_section){
        // already added
        continue;
      }
      int pos = find_symbol(sym.name, symbol_table);
      if(pos == -1){
          // add new symbol
          if(sym.section != 0xFF){
          symbol s = sym;
          s.id = curr_id++;
          string section_name = fd.symbol_table.at(sym.section).name;
          int sec_pos = find_section(section_name,maped_sections);
          s.section = sec_pos;
          section_map sm = maped_sections.at(sec_pos);
          s.offset = s.offset + sm.address + sm.files_addr[fd.file_name];
          symbol_table.push_back(s);
        }
        else{
          // extern symbol
          extern_symbols.push_back(sym.name);
        }
      }
      else{
        // symbol already added
        if(sym.section == 0xFF){
          // extern symbol
          extern_symbols.push_back(sym.name);
        }
        else{
          if(symbol_table.at(pos).defined && sym.defined){
            cout << sym.name << " symbol cant be twice defined\n";
            return false;
          }
        }
      }
    }
  }
  // check if there are undefined symbol
  for(string sym_name:extern_symbols){
    int pos = find_symbol(sym_name,symbol_table);
    if(pos == -1){
      cout << sym_name << " symbol is undefined\n";
      return false;
    }
    if(!symbol_table.at(pos).global){
      cout << sym_name << " symbol must be global for extern operation\n";
      return false;
    }
  }

  return true;
}

int find_symbol(int id_sym, vector<symbol> symbols){
  for(int i=0;i<symbols.size();i++){
    if(symbols.at(i).id == id_sym){
      return i;
    }
  }
  return -1;
}

void Linker::pass_through_relocations_table(){
  for(file_data fd:files_data){
    for(section sec:fd.sections){
      int section_pos = find_section(sec.name,maped_sections);
      section_map sm = maped_sections.at(section_pos);
      uint64_t sec_addr = sm.address;
      uint64_t sec_offset = sm.files_addr[fd.file_name];
      for(relocation r:sec.relocations){
        int sym_pos = find_symbol(r.id,fd.symbol_table);
        symbol sym = fd.symbol_table.at(sym_pos);
        // address in memory
        uint64_t offset = r.offset + sec_offset;
        int content;
        if(sym.is_section){
          int rel_sec_pos = find_section(sym.name, maped_sections);
          section_map rel_sec = maped_sections.at(rel_sec_pos);
          content = rel_sec.address + rel_sec.files_addr[fd.file_name];
        }
        else{
          int rel_sym_pos = find_symbol(sym.name, symbol_table);
          symbol rel_sym = symbol_table.at(rel_sym_pos);
          content = rel_sym.offset;
        }
        content += r.addend;
        if(r.rel_type == PC_REL){
          content = content - offset - sec_addr;
        }
        int iter = offset;
        sm.code.at(iter++) = (content & 0xFF);
        sm.code.at(iter++) = (content >> 8 & 0xFF);
        sm.code.at(iter++) = (content >> 16 & 0xFF);
        sm.code.at(iter) = (content >> 24 & 0xFF);
        maped_sections.at(section_pos) = sm;
      }
    }
  }
}

void Linker::createHexFile(string output_file){

  std::ofstream hex_file;
  hex_file.open(output_file);

  // print memory
  int i = 0;
  bool begin = true;
  for(section_map sm:maped_sections){
    int addr = sm.address;
    if(i%8!=0 && i!=addr){
      while(i%8!=0){
        hex_file << " " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(0) << std::dec;
        i++;
      }
      i = addr;
    }
    if(addr%8!=0 && i!=addr){
      if(!begin){
        hex_file << endl;
      }
      begin = false;
      i = addr;
      hex_file << std::setfill('0') << std::setw(8) << std::hex << (i) << std::dec << ":";
      while(i%8!=0){
        hex_file << " " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(0) << std::dec;
        i++;
      }
    }
    else{
      i = addr;
    }
    for(uint8_t byte:sm.code){
      if(i%8==0){
        if(!begin){
          hex_file << endl;
        }
        begin = false;
        hex_file << std::setfill('0') << std::setw(8) << std::hex << (i) << std::dec << ":";
      }
      hex_file << " " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << std::dec;
      i++;
    }
  }
  if(i%8!=0){
      while(i%8!=0){
        hex_file << " " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(0) << std::dec;
        i++;
      }
    }

  hex_file.close();
}

void Linker::createTextFilesForChecking(){
  std::ofstream txt_file;
  txt_file.open("linker_check.txt");

  

  for(file_data fd:files_data){

    // file name
    txt_file << "File: " << fd.file_name << endl;

    // print symbol table
    txt_file << "#.symtab\n";
    txt_file << "Id\tOffset\t\t\tType\tBind\tSection\tName\n";

    // simbol_table size
    int symtab_size = fd.symbol_table.size();

    for(symbol sym:fd.symbol_table){
      txt_file << static_cast<int>(sym.id) << ":\t" << std::setfill('0') << std::setw(8) << std::hex;


      txt_file << sym.offset << std::dec << "\t\t";


      if(sym.is_section){
        txt_file << "SCTN\t";
      }
      else{
        txt_file << "NOTYP\t";
      }


      if(sym.global){
        txt_file << "GLOB\t";
      }
      else{
        txt_file << "LOC  \t";
      }


      if(sym.section != 0xFF){
        txt_file << static_cast<int>(sym.section) << "\t\t\t";
      }
      else{
        txt_file << "UND\t\t";
      }


      txt_file << sym.name << endl;
    }

    //print relocations
    txt_file << "\nRelocations\n";

    // sections num
    for(section sec:fd.sections){
      txt_file << "#.rela." << sec.name << endl;
      txt_file << "Offset\t\t\tType\tSymbol\tAddend\n";

      for(relocation rel:sec.relocations){
        txt_file << std::setfill('0') << std::setw(8) << std::hex << rel.offset << std::dec << "\t\t";

        if(rel.rel_type == ABS){
          txt_file << "ABS\t\t\t";
        }
        else{
          txt_file << "PC_REL\t";
        }
        // binary_write(binary_file,rel.rel_type);

        txt_file << static_cast<int>(rel.id) << "\t\t\t\t";

        txt_file << std::setfill('0') << std::setw(4) << std::hex << rel.addend << std::dec << endl;
      }
    }

    //print code
    txt_file << "\nCode";

    for(section sec:fd.sections){
      txt_file << "\n#." << sec.name << " code.size:" << sec.data.size();


      int i = 0;
      for(uint8_t byte:sec.data){
        if(i%8==0){
          txt_file << endl;
        }
        txt_file << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << std::dec << "\t";
        i++;
      }

      txt_file << endl;
    }

    txt_file << endl;

  }

  

  // print memory
  txt_file << "Memory\n";
  for(section_map sm:maped_sections){
    txt_file << "section: " << sm.section_name << " ,code_size:" << sm.code.size();
    for(map<string,uint64_t>::iterator it = sm.files_addr.begin();it!=sm.files_addr.end();it++){
      txt_file << "\nfile-start offset: " << it->first << "-" << it->second;
    }
    int addr = sm.address;
    int i = 0;
    for(uint8_t byte:sm.code){
      if(i%8==0){
        txt_file << endl;
        txt_file << std::setfill('0') << std::setw(2) << std::hex << (addr+i) << std::dec << ":\t";
      }
      txt_file << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << std::dec << "\t";
      i++;
    }
    txt_file << endl;
  }

  // print symbol table
  txt_file << "\nSymbol table\n";
  txt_file << "Id\tOffset\t\t\tType\tBind\tSection\tName\n";

    // simbol_table size
    int symtab_size = symbol_table.size();

    for(symbol sym:symbol_table){
      txt_file << static_cast<int>(sym.id) << ":\t" << std::setfill('0') << std::setw(8) << std::hex;


      txt_file << sym.offset << std::dec << "\t\t";


      if(sym.is_section){
        txt_file << "SCTN\t";
      }
      else{
        txt_file << "NOTYP\t";
      }


      if(sym.global){
        txt_file << "GLOB\t";
      }
      else{
        txt_file << "LOC  \t";
      }


      if(sym.section != 0xFF){
        txt_file << static_cast<int>(sym.section) << "\t\t\t";
      }
      else{
        txt_file << "UND\t\t";
      }


      txt_file << sym.name << endl;
    }

  txt_file.close();
}