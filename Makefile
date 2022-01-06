FILE = errorhandling
precomp = game_values 

LINKER =   -l freeglut -l opengl32 -lstdc++

CPP =	main.cpp engine_hub.cpp genCBFvar.cpp errorhandling.cpp  initCBFvar.cpp
OBJ =  main.o engine_hub.o genCBFvar.o errorhandling.o initCBFvar.o
OUT = engine

COMOBJ = $(OUT) $(OBJ)
OUTEXE = $(OUT).exe
LOGOUT = $(ERROR).txt

clean:
#	rm *.o	*.exe  *.h.gch
	g++ -c game_values.h -o game_values.h.gch 
	g++  $(CPP) -o $(OUT)  $(LINKER) -std=c++14

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
	g++  $(CPP) -o $(OUT)  $(LINKER) -std=c++14



