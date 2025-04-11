#ifndef interpres_compiler_h
#define interpres_compiler_h

/*
 * @brief Compile a set of instructions into bytecode.
 * This function takes a set of instructions in the form of a string and
 * compiles it into bytecode which can later be handled by our virtual machine.
 *
 * @param input The input set of instructions to compile.
 * @return void
 */
void compile(const char *input);

#endif
