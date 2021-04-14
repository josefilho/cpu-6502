#include <cstdio>
#include <cstdlib>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Memory {
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void Initialise(){
    for(unsigned char & i : Data) i = 0;
  }

  // Read 1 byte
  Byte operator[](u32 Address) const {
    //assert here Address is < MAX_MEM
    return Data[Address];
  }

  //Write 1 byte
  Byte& operator=(u32 Address) {
    //assert here Address is < MAX_MEM
    return Data[Address];
  }
};

struct CPU {
  Word PC; //Program Counter
  Word SP; //Stack Pointer

  Byte A; //Accumulator
  Byte X; //Index Register X
  Byte Y; //Index Register Y

  Byte C : 1,
       Z : 1,
       I : 1,
       D : 1,
       B : 1,
       V : 1,
       N : 1; //Status Flag

  void Reset( Memory& memory){
    PC = 0xFFFC;
    SP = 0x0100;
    C= Z = I = D = B = V = N = 0;
    A = X = Y = 0;

    memory.Initialise();
  }

  Byte FetchByte(u32& Cycles, Memory& memory){
    Byte Data = memory[PC];
    ++PC, --Cycles;
    return Data;
  }

  static constexpr Byte INS_LDA_IM = 0xA9;

  void Execute(u32 Cycles, Memory& memory){
    while (Cycles > 0){
      Byte Ins = FetchByte( Cycles, memory);
      switch (Ins) {
        case INS_LDA_IM: {
          Byte Value = FetchByte( Cycles, memory);
          A = Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
        }break;
        default:{
          printf("Instruction not handled %d", Ins);
        }break;
      }
    }
  }
};

int main() {
  Memory mem;
  CPU cpu;
  cpu.Reset( mem );
  mem[0xFFFC] = CPU::INS_LDA_IM;
  mem[0xFFFd] = 0x42;
  cpu.Execute( 2, mem );
  return 0;
}
