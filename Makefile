FILE = precomptest

LINKER =   -l freeglut -l opengl32 -lstdc++

OBJ =  main.o engine_hub.o genCBFvar.o
OUT = engine
ERROR = err

COMOBJ = $(OUT) $(OBJ)
OUTEXE = $(OUT).exe
LOGOUT = $(ERROR).txt

start: $(OUT) $(OUTEXE)
	$$(start $(OUTEXE))

compile: $(OUT) $(OUTEXE)

create:	
	echo "" > $(FILE).h.gch
	
upload:
	git add *.cpp *.h 
	git add Makefile
	git commit -m "MakeUpdate" 
	git branch -M development 
	git push -u origin development

$(OUT): $(OBJ) 
	g++ main.cpp engine_hub.cpp genCBFvar.cpp -o engine  $(LINKER) -std=c++14


clean:
	rm *.o	*.exe 
