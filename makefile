assembler: assembler.o pre_assembler.o utils.o
	gcc -Wall -ansi -pedantic -o assembler assembler.o pre_assembler.o utils.o


assembler.o: assembler.c pre_assembler.h data_structures.h
	gcc -Wall -ansi -pedantic -c assembler.c


pre_assembler.o: pre_assembler.c pre_assembler.h utils.h data_structures.h
	gcc -Wall -ansi -pedantic -c pre_assembler.c


utils.o: utils.c utils.h data_structures.h
	gcc -Wall -ansi -pedantic -c utils.c

clean:
	rm -f assembler.o pre_assembler.o utils.o assembler
