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

  // Write 1 byte
  Byte& operator[]( u32 Address){
    return Data[Address];
  }

  // Write 2 bytes
  void WriteWord( Word Value, u32 Address, u32& Cycles ){
    Data[Address] = Value & 0xFF;
    Data[Address + 1] = (Value >> 8);
    Cycles -= 2;
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

  Word FetchWord( u32 Cycles, Mem& memory ){
    Word Data = memory[PC];
    ++PC;
    Data |= (memory[PC] << 8);
    ++PC;
    Cycles -= 2;
    return Data;
  }

  Byte ReadByte( u32 Cycles, Byte Address, Mem& memory ){
    Byte Data = memory[Address];
    --Cycles;
    return Data;
  }

  // opcodes
  static constexpr Byte
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_JSR = 0x20;


  void LDASetStatus(){
    Z = ( A == 0 );
    N = (A & 0b10000000) > 0;
  }

  void Execute( u32 Cycles, Mem& memory ){
    while( Cycles > 0 ){
      Byte Ins = FetchByte( Cycles, memory);
      switch ( Ins ) {
        case INS_LDA_IM: {
          Byte Value = FetchByte( Cycles, memory);
          A = Value;
          LDASetStatus();
        } break;
        case INS_LDA_ZP:{
          Byte ZeroPageAddr = FetchByte( Cycles, memory );
          A = ReadByte(Cycles, ZeroPageAddr, memory);
          LDASetStatus();
        } break;
        case INS_LDA_ZPX:{
          Byte ZeroPageAddr = FetchByte( Cycles, memory );
          ZeroPageAddr += X;
          --Cycles;
          A = ReadByte(Cycles, ZeroPageAddr, memory);
          LDASetStatus();
        } break;
        case INS_JSR: {
          Word SubAddr = FetchWord( Cycles, memory );
          memory.WriteWord( PC - 1, SP, Cycles);
          PC = SubAddr;
          Cycles--;
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

  // Start - inline program
  mem[0xFFFC] = CPU::INS_LDA_IM;
  mem[0xFFFD] = 0x42;
  // End - inline program

  cpu.Reset( mem );
  cpu.Execute( 2, mem );
  return 0;
}
