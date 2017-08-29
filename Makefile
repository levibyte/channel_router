ifeq ($(USER),levibyte)
	LIBS=./deps/SDL2/libSDL2.a
	INCLS=-I ./deps/SDL2/include
else
	LIBS=-L ~levons/levi/downloads/SDL2-2.0.5/build/.libs -lSDL2
	INCLS=-I ~levons/levi/downloads/SDL2-2.0.5/include 
endif

F=main.cpp graph.cpp
default:
	clear
	rm -rf zRouter.bin
	g++ -Wfatal-errors -g  $(F) $(INCLS) $(LIBS) -ldl -lrt -o zRouter.bin