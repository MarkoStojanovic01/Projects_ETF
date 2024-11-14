#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <fstream>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

class Emulator{

  map<uint32_t, vector<uint8_t>> memory;

  uint32_t gprs[16] = {0};
  uint32_t csrs[3] = {0};
  uint32_t& sp = gprs[14];
  uint32_t& pc = gprs[15];
  uint32_t& status = csrs[0];
  uint32_t& handler = csrs[1];
  uint32_t& cause = csrs[2];

  public:

    Emulator();

    bool readFromHexFile(string hex_file_name);

    uint8_t readByte(uint32_t addr);

    void writeByte(uint32_t addr, uint8_t byte);

    uint32_t readInt(uint32_t addr);

    void writeInt(uint32_t addr, uint32_t byte);

    bool emulate();

    void printState();

    void printMemory();

};


#endif