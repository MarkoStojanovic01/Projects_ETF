#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "../inc/assembler_structures.hpp"
#include "../inc/line.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class Assembler{

  private:

    vector<symbol> symbol_table;
    vector<section> sections;
    int locationCounter;
    uint8_t currentSectionId;
    section currentSection;
    uint8_t currentId;
    vector<string> undefined_symbols;

    void addByteToSectionCode(uint8_t byte);
    void addOperationCode(OperationCode oc);
    void addWordToSectionCode(int num);
    void addOffsetToSectionCode(int num);
    void writeSymbol(char* symbol,Relocation_type rel_type);


  public:

    Assembler(){
      this->locationCounter = 0;
      this->currentSectionId = -1;
      this->currentId = 0;
    };

    int assemble(vector<line>* lines);
    void createOutputFiles(string output_file);

};

#endif