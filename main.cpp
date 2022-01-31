#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char; // Program Counter
using Word = unsigned short; // Stack Pointer

using u32 = unsigned int;

struct Mem{
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void Initialise(){
    for(u32 i = 0; i < MAX_MEM; ++i){
      Data[i] = 0;
    }
  }
};

struct CPU{
  Word PC;
  Word SP;

  Byte A, X, Y; // Registers

  Byte C : 1; // Status Flag
  Byte Z : 1; // Status Flag
  Byte I : 1; // Status Flag
  Byte D : 1; // Status Flag
  Byte B : 1; // Status Flag
  Byte V : 1; // Status Flag
  Byte N : 1; // Status Flag

  void Reset( Mem& memory){
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = B = V = N = D = 0;
    A = X = Y = 0;

    memory.Initialise();
  }

  void Execute( u32 Ticks, Mem& memory ){
      
  }

};

int main() {
  Mem mem;
  CPU cpu;
  cpu.Reset( mem );
  cpu.Execute( mem );
  return 0;
}
