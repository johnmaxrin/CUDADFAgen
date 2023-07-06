all:	program

program:	y.tab.c lex.yy.cc 
	gcc lex.yy.c y.tab.c generator/dfaGen.c generator/regex/regextodfa.h generator/helpers/boilerplate.c generator/keyword/aho.c generator/regex/regextodfa.c generator/regex/transitionTable.c -o dfagen -lfl

lex.yy.cc:	parser/dfagen.l
	lex parser/dfagen.l

y.tab.c:	parser/dfagen.y
	yacc -d parser/dfagen.y

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h dfagen plex plex.cu  