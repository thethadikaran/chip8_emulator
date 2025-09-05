#include "chip8.h"

/**
 * instructions 16 bits -> 4 nibbles
 * 
 * CHP8 has 34 instructions/opcode
 * 
 * 4 hex digits are used to represent a instruction
 * 0000 0000 0000 0000 - 16 bits (2 bytes)
 * 
 * stored in big-endian (MSB stored at lowest memory address)
 * 
 *  
 *   byte-1      byte-2
 * (high byte) (low byte)
 * 0000 0000   0000 0000
 * 
 *       x       y    n
 *               n    n
 *       n       n    n
 * INS
 * 
 * 
 * x - value (4 bits)    -> lower 4 bits of high byte in the instruction
 * y - value (4 bits)    -> upper 4 bits of low byte  in the instruction
 * 
 * n   - value (4 bits)  -> lowest 4 bits of instruction
 * nn  - value (8 bits)  -> lowest 8 bits of instruction
 * nnn - value (12 bits) -> lowest 12 bits of instruction (for holding address)
 * 
 * INS - instruction (4 bits) -> upper 4 bits in high byte of the instruction
 * 
 */


 void parse_opcode(emulator_t *emulator) {
  // get the 16bit opcode, pointed by the program counter
  emulator->instruction.opcode = (emulator->ram[emulator->PC] << 8) | emulator->ram[emulator->PC + 1];

  emulator->PC += 2;     // increment program counter for next opcode
                         // +2 as each instruction takes 8 bit

  // & operator copy the needed bit fields, ignoring the other fields
  
  emulator->instruction.instruction = (emulator->instruction.opcode >> 12) & 0x0F;
  emulator->instruction.x = (emulator->instruction.opcode >> 8) & 0x0F;
  emulator->instruction.y = (emulator->instruction.opcode >> 4) & 0x0F;
  emulator->instruction.n = emulator->instruction.opcode & 0x0F;
  emulator->instruction.nn = emulator->instruction.opcode & 0x0FF;
  emulator->instruction.nnn = emulator->instruction.opcode & 0x0FFF;
}