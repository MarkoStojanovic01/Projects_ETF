
#include <iostream>
#include <fstream>
#include "../inc/assembler.hpp"

using namespace std;

extern FILE* yyin;
extern int yyparse(void);

int main(int argc, char* argv[]){

  string input_file_name;
  string output_file_name;

  switch(argc){
    case 2:
      input_file_name = argv[1];
      output_file_name = input_file_name.substr(0,input_file_name.size()-2)+".o";
      break;
    case 4:
      if(string(argv[1]) != "-o"){
        cout << "Wrong option!!!\n";
        return -1;
      }
      input_file_name = argv[3];
      output_file_name = argv[2];
      break;
    default:
      cout << "Wrong option!!!\n";
      return -1;
  }

  FILE* input_file = fopen(input_file_name.c_str(), "r");

  if(!input_file){
    cout << "Cant open file\n";
    return -1;
  }

  yyin = input_file;

  lines = new vector<line>();

  if(yyparse()){
    return -1;
  }

  fclose(input_file);


//  for(line l:lines[0]){
//    print_line(l);
//  }

  Assembler a;

  a.assemble(lines);

  a.createOutputFiles(output_file_name);

  return 0;

}