FILE = CBF
PRECOMP = game_values

LINKER = -LG:/Dev/Custom_Engine/src/lib   -lopengl32  -l freeglut -lfreetype.dll -lstdc++
INCLUDE = -IG:/Dev/Custom_Engine/src/include 

CPP =	main.cpp engine_hub.cpp errorhandling.cpp game_values.cpp  
OBJ =  main.o engine_hub.o  errorhandling.o game_values.o
OUT = engine

COMOBJ = $(OUT) $(OBJ)
OUTEXE = $(OUT).exe
LOGOUT = $(ERROR).txt

clean:
#	rm *.txt
#	g++ -c $(PRECOMP).h -o $(PRECOMP).h.gch 
#	g++  $(CPP) -o $(OUT)  $(LINKER) -std=c++14

start: $(OUT) $(OUTEXE)
	$$(start $(OUTEXE))

compile: $(OUT) $(OUTEXE)

create:	
	echo "" > $(FILE).cpp
	
upload:
	git add *
	git add Makefile
	git commit -m "MakeUpdate" 
	git branch -M development 
	git push -u origin development
$(OUT): 
	gcc  -o $(OUT) $(CPP)  $(INCLUDE) $(LINKER) -std=c++14



