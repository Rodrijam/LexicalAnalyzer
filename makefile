P1.out : Project1.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P1.out Project1.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	
Project1.o : Project1.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project1.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P1.out *.gch *.dbg *.p1 *.lst

submit : Project1.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt 
	rm -rf PenguinP1
	mkdir PenguinP1
	cp Project1.cpp PenguinP1
	cp LexicalAnalyzer.h PenguinP1
	cp LexicalAnalyzer.cpp PenguinP1
	cp SyntacticalAnalyzer.h PenguinP1
	cp SyntacticalAnalyzer.cpp PenguinP1
	cp makefile PenguinP1
	cp README.txt PenguinP1
	tar cfvz PenguinP1.tgz PenguinP1

