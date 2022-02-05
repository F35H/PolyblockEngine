FILE = CBF
PRECOMP = game_values

LINKER = -LG:/Dev/Custom_Engine/src/lib   -lopengl32  -l freeglut -lfreetype.dll -lstdc++
INCLUDE = -IG:/Dev/Custom_Engine/src/include -IG:/Dev/Custom_Engine/src/ 

CPP =	main.cpp engine_hub.cpp InitCBF.cpp GenCBF.cpp GameCBF.cpp mainmenCBF.cpp errorhandling.cpp game_values.cpp  
OBJ =  main.o engine_hub.o InitCBF.o GenCBF.o GameCBF.o mainmenCBF.o errorhandling.o game_values.o 
OUT = engine

COMOBJ = $(OUT) $(OBJ)
OUTEXE = $(OUT).exe
LOGOUT = $(ERROR).txt

clean:
	rm *.o *.exe
	g++ -c $(PRECOMP).h -o $(PRECOMP).h.gch 
#	g++  $(CPP) -o $(OUT)  $(LINKER) -std=c++14

start: $(OUT) $(OUTEXE)
	$(OUTEXE)

compile: $(OUT) $(OUTEXE)


create:	
	echo "" > MainMenuCBF.cpp
	
upload:
	git add *
	git add Makefile
	git commit -m "MakeUpdate" 
	git branch -M development 
	git push -u origin development
$(OUT): 
	gcc  -o $(OUT) $(CPP)  $(INCLUDE) $(LINKER) -std=c++17



