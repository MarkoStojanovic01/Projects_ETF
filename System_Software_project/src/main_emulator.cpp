#include "../inc/emulator.hpp"
#include <iostream>

int main(int argc, char* argv[]){

  if(argc != 2){
    cout << "Error: Emulator only needs one argument\n";
    return -1;
  }

  Emulator e;

  if(!e.readFromHexFile(argv[1])){
    std::cout << "Cant open hex file\n";
    return -1;
  }

  e.emulate();

  e.printState();

  // e.printMemory();


  return 0;

}