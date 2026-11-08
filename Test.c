
#include <stdio.h>
#include <stdint.h>

#define NO_ERR 0

int main(void) {

    uint32_t pc = 0;
    uint32_t reg[32] = {0};

    // Here the first program hard coded as an array
    uint32_t progr[3] = {
        // A minimal RISC-V assembler example
        0x00200093, // addi x1, x0, 2
        0x00300113, // addi x2, x0, 3
        0x002081b3, // add x3, x1, x2
    };

    printf("Hello RISC-V World!\n");

    // x0 needs to be 0 all the time
    // Potential solution could be:
    
    // if (rd != 0) {
    // reg[rd] = result;
    // }



    // Simulation loop
    while (1) {

        uint32_t instr = progr[pc >> 2];
        uint32_t opcode = instr & 0x7f;
        uint32_t rd = (instr >> 7) & 0x01f;
        uint32_t rs1 = (instr >> 15) & 0x01f;
        uint32_t rs2 = (instr >> 20) & 0x01f;
        uint32_t func3 = (instr >> 12) & 0x07;
        uint32_t func7 = (instr >> 25) & 0x7F;
        uint32_t imm = (instr >> 20);

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
                                reg[rd] = reg[rs1] << reg[rs2];
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
                                reg[rd] = reg[rs1] >> reg[rs2];
                                break;
                            }
                            
                            case 0x20: {//Shift Right Arith* 
                                reg[rd] = (uint32_t)((int32_t)reg[rs1] >> (reg[rs2]));

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

                break;
            }
            
            case 0x23: {
                //"S-Type"

                break;
            }
            
            case 0x63: {
                //"B-Type"

                break;
            }
            
            case 0x6F: {//jal
                //"J-Type"

                break;
            }
        
            case 0x67: {//jalr
                //"I-Type"

                break;
            }
            
            case 0x37: {//lui
                //"U-Type"

                break;
            }

            case 0x17: {//auipc
                //"U-Type"

                break;
            }
            
            case 0x73: {//ecall
                //"I-Type"

                break;
            }

            break;
        }
        
        pc += 4; // One instruction is four bytes

        if ((pc >> 2) >=
            (sizeof(progr) / sizeof(progr[0]))) {
            break;
        }

        for (size_t i = 0;
             i < (sizeof(reg) / sizeof(reg[0]));
             ++i) {
            printf("%u ", reg[i]);
        }

        printf("\n");
    }

    printf("Program exit\n");

    return NO_ERR;
}
