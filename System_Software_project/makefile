
FILES_AS = src/main_assembler.cpp src/line.cpp misc/parser.cpp misc/lexer.cpp src/assembler.cpp
FILES_LN = src/linker.cpp src/main_linker.cpp
FILES_EM = src/emulator.cpp src/main_emulator.cpp

run_tests:
	./assembler -o results/main.o tests/nivo-a/main.s
	./assembler -o results/math.o tests/nivo-a/math.s
	./assembler -o results/handler.o tests/nivo-a/handler.s
	./assembler -o results/isr_software.o tests/nivo-a/isr_software.s
	./assembler -o results/isr_terminal.o tests/nivo-a/isr_terminal.s
	./assembler -o results/isr_timer.o tests/nivo-a/isr_timer.s
	./linker -hex \
		-place=my_code@0x40000000 -place=math@0xF0000000 \
		-o results/program.hex results/handler.o results/main.o results/math.o \
		results/isr_software.o results/isr_terminal.o results/isr_timer.o
	./emulator results/program.hex

run_mini_test:
	./assembler -o test.o tests/test.s
	./linker -hex \
		-place=my_code@0x40000000 -place=data@0xF0000000 \
		-o mini_program.hex test.o

lexer: misc/lexer.l
			flex misc/lexer.l
			mv lexer.cpp ./misc/
			mv lexer.hpp ./misc/

parser: misc/parser.y misc/lexer.l
			bison misc/parser.y
			mv parser.cpp ./misc/
			mv parser.hpp ./misc/

assembler: $(FILES_AS)
			g++ -g $(FILES_AS) -o assembler

linker: $(FILES_LN)
			g++ -g $(FILES_LN) -o linker

emulator: $(FILES_EM)
			g++ -g $(FILES_EM) -o emulator			

clean_tests_results:
			rm -rf ./results/*.o ./results/*.txt ./results/*.hex

clean:
	rm -rf ./misc/*.cpp ./misc/*.hpp ./assembler ./linker ./emulator

clean_all: clean clean_tests_results

lex_par: lexer parser

all: assembler linker emulator