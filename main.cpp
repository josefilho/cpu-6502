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

  // Read 1 byte
  Byte operator[]( u32 Address ) const{
    return Data[Address]; // Assert here Address is < MAX_MEM
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

  Byte FetchByte( u32 Cycles, Mem& memory ){
    Byte Data = memory[PC];
    ++PC,
    --Cycles;
    return Data;
  }

  // opcodes
  static constexpr Byte
    INS_LDA_IM = 0xA9;

  void Execute( u32 Cycles, Mem& memory ){
    while( Cycles > 0 ){
      Byte Ins = FetchByte( Cycles, memory);
      switch ( Ins ) {
        case INS_LDA_IM: {
          Byte Value = FetchByte( Cycles, memory);
          A = Value;
          Z = ( A == 0 );
          N = (A & 0b10000000) > 0;
        } break;
        default: {
          printf("Instruction not handled %d", Ins);
        } break;
      }
    }
  }

};

int main() {
  Mem mem;
  CPU cpu;
  cpu.Reset( mem );
  cpu.Execute( 2, mem );
  return 0;
}
