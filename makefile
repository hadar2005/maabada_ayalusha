CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

# List of all object files required to build the program
OBJS = assembler.o pre_assembler.o first_pass.o second_pass.o symbol_table.o data_parser.o instruction_parser.o utils.o tables.o output_writer.o memory_manager.o

# Build the executable assembler
assembler: $(OBJS)
	$(CC) $(CFLAGS) -o assembler $(OBJS)

# Dependencies and compilation rules for each source file

assembler.o: assembler.c data_structures.h pre_assembler.h first_pass.h second_pass.h output_writer.h memory_manager.h
	$(CC) $(CFLAGS) -c assembler.c

pre_assembler.o: pre_assembler.c pre_assembler.h data_structures.h
	$(CC) $(CFLAGS) -c pre_assembler.c


first_pass.o: first_pass.c first_pass.h data_structures.h symbol_table.h utils.h data_parser.h instruction_parser.h
	$(CC) $(CFLAGS) -c first_pass.c

second_pass.o: second_pass.c second_pass.h data_structures.h symbol_table.h utils.h
	$(CC) $(CFLAGS) -c second_pass.c

symbol_table.o: symbol_table.c symbol_table.h data_structures.h
	$(CC) $(CFLAGS) -c symbol_table.c

data_parser.o: data_parser.c data_parser.h data_structures.h symbol_table.h utils.h
	$(CC) $(CFLAGS) -c data_parser.c

instruction_parser.o: instruction_parser.c instruction_parser.h data_structures.h tables.h utils.h
	$(CC) $(CFLAGS) -c instruction_parser.c

utils.o: utils.c utils.h data_structures.h
	$(CC) $(CFLAGS) -c utils.c

tables.o: tables.c tables.h data_structures.h
	$(CC) $(CFLAGS) -c tables.c

output_writer.o: output_writer.c output_writer.h data_structures.h
	$(CC) $(CFLAGS) -c output_writer.c

memory_manager.o: memory_manager.c memory_manager.h data_structures.h symbol_table.h
	$(CC) $(CFLAGS) -c memory_manager.c

# Remove intermediate files before rebuilding
clean:
	rm -f *.o assembler