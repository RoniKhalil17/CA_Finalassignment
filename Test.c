
#include <stdio.h>
#include <stdint.h>

#define NO_ERR 0
#define MEM_SIZE (1 << 20)
uint8_t mem[MEM_SIZE] = {0};

//Helper to check for bad address
static int mem_ok(uint32_t addr, uint32_t size) {
    return addr <= MEM_SIZE && size <= MEM_SIZE - addr;
}


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s program.bin [output.bin]\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "rb");
    if (in == NULL) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }
    size_t prog_size = fread(mem, 1, MEM_SIZE, in); // size in bytes
    fclose(in);

    uint32_t pc = 0;
    uint32_t reg[32] = {0};

    /* Here the first program hard coded as an array
    uint32_t progr[3] = {
        // A minimal RISC-V assembler example
        0x00200093, // addi x1, x0, 2
        0x00300113, // addi x2, x0, 3
        0x002081b3, // add x3, x1, x2
    }; */

    printf("Hello RISC-V World!\n");

    // x0 needs to be 0 all the time
    // Potential solution could be:
    
    // if (rd != 0) {
    // reg[rd] = result;
    // }



    int running = 1;

    // Simulation loop
    while (running) {

        uint32_t instr = mem[pc]
               | (mem[pc + 1] << 8)
               | (mem[pc + 2] << 16)
               | ((uint32_t)mem[pc + 3] << 24);
        uint32_t opcode = instr & 0x7f;
        uint32_t rd = (instr >> 7) & 0x01f;
        uint32_t rs1 = (instr >> 15) & 0x01f;
        uint32_t rs2 = (instr >> 20) & 0x01f;
        uint32_t func3 = (instr >> 12) & 0x07;
        uint32_t func7 = (instr >> 25) & 0x7F;
        int32_t imm = (int32_t)instr >> 20;

        uint32_t next_pc = pc + 4; // Default go to the next instruction

        switch (opcode) {

            case 0x33: {
                //"R-Type"

                switch (func3){
                    case 0x0: {
                        switch(func7){
                            case 0x00: {//ADD
                                reg[rd] = reg[rs1] + reg[rs2];
                                break;
                            }
                            case 0x20: {//SUB
                                reg[rd] = reg[rs1] - reg[rs2];
                                break;
                            }
                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }
                        break;
                    }

                    case 0x4: {//XOR
                        switch(func7){
                            case 0x00: {
                                reg[rd] = reg[rs1] ^ reg[rs2];
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }

                        break;
                    }

                    case 0x6: {//OR
                        switch(func7){
                            case 0x00: {
                                reg[rd] = reg[rs1] | reg[rs2];
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }        

                        break;
                    }
                    
                    case 0x7: {//AND
                        switch(func7){
                            case 0x00: {
                                reg[rd] = reg[rs1] & reg[rs2];
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }        

                        break;
                    }
                    
                    case 0x1: {//Shift Left Logical
                        switch(func7){
                            case 0x00: {
                                reg[rd] = reg[rs1] << (reg[rs2] & 0x1F);
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }        

                        break;
                    }
                    
                    case 0x5: {
                        switch(func7){
                            case 0x00: {//Shift Right Logical 
                                reg[rd] = reg[rs1] >> (reg[rs2] & 0x1F);
                                break;
                            }
                            
                            case 0x20: {//Shift Right Arith* 
                                reg[rd] = (uint32_t)((int32_t)reg[rs1] >> (reg[rs2] & 0x1F));

                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }        

                        break;
                    }
                    
                    case 0x2: {//Set Less Than 
                        switch(func7){
                            case 0x00: {
                                reg[rd] = ((int32_t)reg[rs1] < (int32_t)reg[rs2]) ? 1 : 0;
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }        

                        break;
                    }

                    case 0x3: {//Set Less Than (U)
                        switch(func7){
                            case 0x00: {
                                reg[rd] = (reg[rs1] < reg[rs2])?1:0;
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }        

                        break;
                    }

                    default: {
                        printf("Opcode %u not yet implemented\n", opcode);
                        break;
                    }
                }

                break;
            }

            case 0x13: {
                //"I-Type"

                switch (func3){
                    case 0x0: {//ADDI
                        reg[rd] = reg[rs1] + imm;
                        break;
                    }

                    case 0x4:{//XOR immediate
                        reg[rd] = reg[rs1] ^ imm;
                        break;
                    }

                    case 0x6:{//OR immediate
                        reg[rd] = reg[rs1] | imm;
                        break;
                    }

                    case 0x7:{//AND immediate
                        reg[rd] = reg[rs1] & imm;
                        break;
                    }

                    case 0x1:{//Shift Left Logical Immediate
                        if (func7 == 0x00) {
                            reg[rd] = reg[rs1] << (imm & 0x1F);
                        }
                        else {
                            printf("SLLI not valid\n");
                        }
                        break;
                    }       

                    case 0x5:{//Shift Right Immediate
                        switch(func7){
                            case 0x00: {//SRLI
                                reg[rd] = reg[rs1] >> (imm & 0x1F);
                                break;
                            }

                            case 0x20: {//SRAI
                                reg[rd] = (uint32_t)((int32_t)reg[rs1] >> (imm & 0x1F));
                                break;
                            }

                            default: {
                                printf("Opcode %u and func3 %u and func7 %u not yet implemented\n", opcode, func3, func7);
                                break;
                            }
                        }
                        break;
                    }

                    case 0x2:{//Set Less Than Immediate
                        reg[rd] = ((int32_t)reg[rs1] < imm) ? 1 : 0;
                        break;
                    }

                    case 0x3:{//Set Less Than Immediate Unsigned
                        reg[rd] = (reg[rs1] < (uint32_t)imm) ? 1 : 0;
                        break;
                    }

                    default: {
                        printf("Opcode %u not yet implemented\n", opcode);
                        break;
                    }
                }

                break;
            }

            case 0x03: {
                //"I-Type-Load"

                uint32_t addr = reg[rs1] + imm;
                uint32_t size = 1u << (func3 & 0x3); // 1, 2 or 4 bytes
                if (!mem_ok(addr, size)) {
                    printf("Memory access out of bounds at address %u\n", addr);
                    running = 0;
                    break;
                }

                switch (func3){
                    case 0x0: {//LB
                        reg[rd] = (uint32_t)(int32_t)(int8_t)mem[addr];
                        break;
                    }

                    case 0x1: {//LH
                        uint16_t half = mem[addr] | (mem[addr + 1] << 8);
                        reg[rd] = (uint32_t)(int32_t)(int16_t)half;
                        break;
                    }

                    case 0x2: {//LW
                        reg[rd] = mem[addr]
                        | (mem[addr + 1] << 8)
                        | (mem[addr + 2] << 16)
                        | ((uint32_t)mem[addr + 3] << 24);
                    break;
                    }

                    case 0x4: {//LBU
                        reg[rd] = mem[addr];
                        break;
                    }

                    case 0x5: {//LHU
                        reg[rd] = mem[addr] | (mem[addr + 1] << 8);
                        break;
                    }

                    default: {
                        printf("Opcode %u and func3 %u not yet implemented\n", opcode, func3);
                        break;
                    }
                }

                break;
            }
            
            case 0x23: {
                //"S-Type"

                int32_t s_imm = (((int32_t)instr >> 25) << 5) | (int32_t)rd; // imm[11:5] | imm[4:0]
                uint32_t addr = reg[rs1] + s_imm;
                uint32_t size = 1u << (func3 & 0x3); // 1, 2 or 4 bytes
                if (!mem_ok(addr, size)) {
                    printf("Memory access out of bounds at address %u\n", addr);
                    running = 0;
                    break;
                }

                switch (func3){
                    case 0x0: {//SB
                        mem[addr] = reg[rs2] & 0xFF;
                        break;
                    }

                    case 0x1: {//SH
                        mem[addr]     = reg[rs2] & 0xFF;
                        mem[addr + 1] = (reg[rs2] >> 8) & 0xFF;
                        break;
                    }

                    case 0x2: {//SW
                        mem[addr]     = reg[rs2] & 0xFF;
                        mem[addr + 1] = (reg[rs2] >> 8) & 0xFF;
                        mem[addr + 2] = (reg[rs2] >> 16) & 0xFF;
                        mem[addr + 3] = (reg[rs2] >> 24) & 0xFF;
                        break;
                    }

                    default: {
                        printf("Opcode %u and func3 %u not yet implemented\n", opcode, func3);
                        break;
                    }
                }

                break;
            }
            
            case 0x63: {
                //"B-Type"

                // imm[12] | imm[10:5] | imm[4:1] | imm[11], then sign-extend from bit 12
                uint32_t b_raw = (((instr >> 31) & 0x1)  << 12)
                   | (((instr >> 7)  & 0x1)  << 11)
                   | (((instr >> 25) & 0x3F) << 5)
                   | (((instr >> 8)  & 0xF)  << 1);
                int32_t b_imm = (int32_t)(b_raw << 19) >> 19;

                int take = 0;

                switch (func3){
                    case 0x0: {//BEQ
                        take = (reg[rs1] == reg[rs2]);
                        break;
                    }

                    case 0x1: {//BNE
                        take = (reg[rs1] != reg[rs2]);
                        break;
                    }

                    case 0x4: {//BLT
                        take = ((int32_t)reg[rs1] < (int32_t)reg[rs2]);
                        break;
                    }

                    case 0x5: {//BGE
                        take = ((int32_t)reg[rs1] >= (int32_t)reg[rs2]);
                        break;
                    }

                    case 0x6: {//BLTU
                        take = (reg[rs1] < reg[rs2]);
                        break;
                    }

                    case 0x7: {//BGEU
                        take = (reg[rs1] >= reg[rs2]);
                        break;
                    }

                    default: {
                        printf("Opcode %u and func3 %u not yet implemented\n", opcode, func3);
                        break;
                    }
                }

                if (take) {
                    next_pc = pc + b_imm;
                }

                break;
            }
            
            case 0x6F: {//jal
                //"J-Type"

                // imm[20] | imm[10:1] | imm[11] | imm[19:12], then sign-extend from bit 20
                uint32_t j_raw = (((instr >> 31) & 0x1)   << 20)
                   | (((instr >> 12) & 0xFF)  << 12)
                   | (((instr >> 20) & 0x1)   << 11)
                   | (((instr >> 21) & 0x3FF) << 1);
                int32_t j_imm = (int32_t)(j_raw << 11) >> 11;

                reg[rd] = pc + 4;       // Return address
                next_pc = pc + j_imm;

                break;
            }
        
            case 0x67: {//jalr
                //"I-Type"

                uint32_t target = (reg[rs1] + imm) & ~1u; // Compute BEFORE writing rd
                reg[rd] = pc + 4;                         // Return address
                next_pc = target;

                break;
            }
            
            case 0x37: {//lui
                //"U-Type"

                reg[rd] = instr & 0xFFFFF000; // imm[31:12] already sits in the upper 20 bits

                break;
            }

            case 0x17: {//auipc
                //"U-Type"

                reg[rd] = pc + (instr & 0xFFFFF000);

                break;
            }
            
            case 0x73: {//ecall
                //"I-Type"

                if (func3 == 0x0 && imm == 0x0) {
                    switch (reg[17]) { // a7 holds the environment call number
                        case 10: {//Exit
                            running = 0;
                            break;
                        }

                        default: {
                            printf("Ecall %u not yet implemented\n", reg[17]);
                            break;
                        }
                    }
                }
                else {
                    printf("Opcode %u and func3 %u not yet implemented\n", opcode, func3);
                }

                break;
            }

            default: {
                printf("Opcode %u not yet implemented\n", opcode);
                break;
            }
        }
        
        reg[0] = 0;    // x0 is always 0
        pc = next_pc;

        if (pc >= prog_size) {
            running = 0;
        }

        for (size_t i = 0;
             i < (sizeof(reg) / sizeof(reg[0]));
             ++i) {
            printf("%u ", reg[i]);
        }

        printf("\n");
    }

    for (size_t i = 0; i < 32; ++i) {
        printf("%u ", reg[i]);
    }
    printf("\n");

    const char *out_name = (argc >= 3) ? argv[2] : "output.bin";
    FILE *out = fopen(out_name, "wb");
    fwrite(reg, sizeof(uint32_t), 32, out);
    fclose(out);

    printf("Program exit\n");

    return NO_ERR;
}
