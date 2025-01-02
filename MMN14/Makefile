FILES_SOURCE = src/main.c src/assembler.c src/utils.c src/macros.c src/symbols.c

all: $(FILES_SOURCE)
	gcc -g -Wall -ansi -pedantic -I include -o assembler $(FILES_SOURCE)

clean:
	rm -f assembler *.ent *.ext *.ob *.am
