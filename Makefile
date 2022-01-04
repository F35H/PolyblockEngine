FILE = errorhandling
precomp = game_values 

LINKER =   -l freeglut -l opengl32 -lstdc++

CPP =	main.cpp engine_hub.cpp genCBFvar.cpp errorhandling.cpp 
OBJ =  main.o engine_hub.o genCBFvar.o errorhandling.o
OUT = engine

COMOBJ = $(OUT) $(OBJ)
OUTEXE = $(OUT).exe
LOGOUT = $(ERROR).txt

start: $(OUT) $(OUTEXE)
	$$(start $(OUTEXE))

compile: $(OUT) $(OUTEXE)

create:	
	echo "" > $(FILE).cpp
	
upload:
	git add *.cpp *.h 
	git add Makefile
	git commit -m "MakeUpdate" 
	git branch -M development 
	git push -u origin development

$(OUT): $(OBJ) 
	g++ -c $(precomp).h -o $(precomp).h.gch
	g++   -o engine  $(LINKER) -std=c++14


clean:
	rm *.o	*.exe  *.h.gch *.txt *.bin
