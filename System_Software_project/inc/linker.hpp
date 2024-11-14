#ifndef LINKER_HPP
#define LINKER_HPP

#include "../inc/assembler_structures.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

using namespace std;

class Linker{

  struct file_data{
    string file_name;
    vector<symbol> symbol_table;
    vector<section> sections;
  };

  struct section_map{
    string section_name;
    vector<uint8_t> code;
    uint64_t address;
    map<string, uint64_t> files_addr;
  };

  vector<file_data> files_data;
  vector<section_map> maped_sections;
  vector<symbol> symbol_table;

  int find_section(string sec_name, vector<section> sections);
  int find_section(string sec_name, vector<section_map> sections_map);

  public:

    Linker() = default;

    bool read_input_files(vector<string> input_files);

    bool mapping_sections(map<string, uint64_t> places);

    bool merge_symbols();

    void pass_through_relocations_table();

    void createHexFile(string output_file);

    void createTextFilesForChecking();
};


#endif
