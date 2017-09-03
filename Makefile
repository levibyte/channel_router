ifeq ($(USER),levibyte)
	LIBS=./deps/SDL2/libSDL2.a ./deps/SDL2_ttf/libSDL2_ttf.a
	INCLS=-I ./deps/SDL2/include -I ./deps/SDL2_ttf
else
	LIBS=-L ~levons/levi/downloads/SDL2-2.0.5/build/.libs -lSDL2
	INCLS=-I ~levons/levi/downloads/SDL2-2.0.5/include 
endif

F=main.cpp graph.cpp
default:
	clear
	rm -rf zRouter.bin
	g++ -Wfatal-errors -g  $(F) $(INCLS) $(LIBS) -lfreetype -ldl -lrt -o zRouter.bin