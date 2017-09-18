OUT=ChannelRouter.bin
FLAGS=-DTEXT_RENDER

ifeq ($(USER),levibyte)
	LIBS=./deps/SDL2/libSDL2.a ./deps/SDL2_ttf/libSDL2_ttf.a -lfreetype -ldl -lrt
	INCLS=-I ./deps/SDL2/include -I ./deps/SDL2_ttf
else
	LIBS=-L ~/levi/downloads/SDL2-2.0.5/build/.libs -lSDL2 -lfreetype -ldl -lrt
	INCLS=-I ~/levi/downloads/SDL2-2.0.5/include 
endif

ifeq ($(OS),Windows_NT)
	FLAGS=-static-libstdc++ -DOS_WINDOWS
	LIBS=-L C:/Users/levons/Downloads/sdl/SDL2-2.0.5/win -lSDL2
	INCLS=-I C:/Users/levons/Downloads/sdl/SDL2-2.0.5/include
	OUT=ChannelRouter.exe
endif


SRCS=src/main.cpp src/graph.cpp

default:
	#cd ./src
	clear
	rm -rf ./bin/$(OUT) 
	g++ -Wfatal-errors -g $(FLAGS) $(SRCS) $(INCLS) $(LIBS) -o ./bin/$(OUT)