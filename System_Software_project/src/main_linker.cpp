
#include <iostream>
#include <fstream>
#include "../inc/linker.hpp"
#include <regex>

using namespace std;

int main(int argc, char* argv[]){

  vector<string> input_files_name;
  string output_file_name;
  bool outputfile_option = false;
  map<string, uint64_t> places;
  bool hex_option = false;
  regex place_option_regex("-place=([a-zA-Z_][a-zA-Z0-9_]*)@(0[xX][0-9a-fA-F]+)$");
  smatch place_arguments;

  for(int i=1;i<argc;i++){
    string arg = argv[i];
    if(arg == "-o"){
      if(i == argc-1){
        cout << "Option -o must have file name after!\n";
        return -1;
      }
      outputfile_option = true;
      output_file_name = argv[++i];
    }
    else if(arg == "-hex"){
      hex_option = true;
    }
    else if(regex_search(arg, place_arguments, place_option_regex)){
      string section_name = place_arguments.str(1);
      uint64_t start_addres = stol(place_arguments.str(2),nullptr,16);
      places[section_name] = start_addres;
    }
    else{
      input_files_name.push_back(arg);
    }
  }
  if(!hex_option){
    cout << "Hex option must be included\n";
    return -1;
  }
  if(!outputfile_option){
    output_file_name = "linker_output.hex";
  }

  Linker l;

  if(l.read_input_files(input_files_name)){
    cout << "Successfully read files\n";
  }
  else{
    cout << "Error while reading files\n";
    return 0;
  }

  if(l.mapping_sections(places)){
    cout << "Successfully merged sections\n";
  }
  else{
    cout << "Error while merging sections\n";
    return -1;
  }

  if(l.merge_symbols()){
    cout << "Successfully merged symbols\n";
  }
  else{
    cout << "Error while merging symbols\n";
    return -1;
  }
  l.pass_through_relocations_table();

  // l.createTextFilesForChecking();

  l.createHexFile(output_file_name);

  return 0;

}