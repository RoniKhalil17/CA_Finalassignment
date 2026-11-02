
#include <stdio.h>
#include <stdint.h>

#define NO_ERR 0

int main(void) {

    uint32_t pc = 0;
    uint32_t reg[4] = {0};

    // Here the first program hard coded as an array
    uint32_t progr[3] = {
        // A minimal RISC-V assembler example
        0x00200093, // addi x1, x0, 2
        0x00300113, // addi x2, x0, 3
        0x002081b3, // add x3, x1, x2
    };

    printf("Hello RISC-V World!\n");

    // Simulation loop
    while (1) {

        uint32_t instr = progr[pc >> 2];
        uint32_t opcode = instr & 0x7f;
        uint32_t rd = (instr >> 7) & 0x01f;
        uint32_t rs1 = (instr >> 15) & 0x01f;
        uint32_t imm = (instr >> 20);

        switch (opcode) {

            case 0x13:
                reg[rd] = reg[rs1] + imm;
                break;

            default:
                printf("Opcode %u not yet implemented\n", opcode);
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
