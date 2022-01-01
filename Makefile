FILE = game_def

LINKER =   -l freeglut -l opengl32 -lstdc++

OBJ =  main.o bake.o gameio.o timers.o engine_collect.o
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
	gcc *.cpp  $(LINKER) -std=c++14


clean:
	rm *.o	*.exe 
