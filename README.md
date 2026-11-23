# RISC-V RV32I Instruction Set Simulator

A simple instruction set simulator for the RV32I base integer instruction set, written in C.
It reads a RISC-V program from a binary file, executes it, and writes the final register
values to an output file.

## Requirements

- Ubuntu

## Files

| File           | Description                                    |
|----------------|------------------------------------------------|
| `simulator.c`  | The simulator source code                      |
| `Makefile`     | Build and test commands                        |
| `test_all.sh`  | Script that runs all tests in `tests/`         |
| `tests/`       | Test programs (`.bin`) and expected results (`.res`) |

## Compile

```bash
make
```

This builds the executable `simulator`.

## Run

Run a test through the Makefile (in this example the test "add negative" from task 1 is ran):

```bash
make test TEST=tests/task1/addneg.bin
```

The simulator prints the register values after each instruction, followed by the final
register values in hexadecimal. It also writes the binary dump of registers x0 to x31, in
that order, to the output file.

## Test

Run a single test and compare the result with the expected `.res` file:

```bash
make check TEST=tests/task1/addneg.bin
```

This prints `PASS` or `FAIL`. The reference file is found automatically by replacing `.bin`
with `.res`. A different reference file can be given with `REF=path/to/file.res`.

Run all tests:

```bash
make tests
```

If the script is not allowed to run, make it executable first:

```bash
chmod +x test_all.sh
```

## Clean

Remove the executable and the output file:

```bash
make clean
```

## Notes

- The simulator has 1 MB of memory, which holds both instructions and data.
- The program counter starts at address 0.
- The program ends when it executes `ecall` with `a7 = 10`, or when the program counter moves past the last instruction.
- `fence`, `fence.i`, `ebreak` and the CSR instructions are not implemented.
