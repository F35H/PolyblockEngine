FILE = genCBFvar

LINKER =   -l freeglut -l opengl32 -lstdc++

OBJ =  main.o
OUT = engine
ERROR = err

COMOBJ = $(OUT) $(OBJ)
OUTEXE = $(OUT).exe
LOGOUT = $(ERROR).txt

start: $(OUT) $(OUTEXE)
	$$(start $(OUTEXE))

compile: $(OUT) $(OUTEXE)

create:	
	echo "" > $(FILE).h
	echo "" > $(FILE).cpp
	
upload:
	git add *.cpp *.h 
	git add Makefile
	git commit -m "MakeUpdate" 
	git branch -M development 
	git push -u origin development

$(OUT): $(OBJ) 
	gcc main.cpp -o engine.exe  $(LINKER) -std=c++14


clean:
	rm *.o	*.exe 
