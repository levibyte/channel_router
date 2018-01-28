OUT=ChannelRouter.bin
FLAGS=-DTEXT_RENDER
SRCS=src/main.cpp src/graph.cpp

ifeq ($(USER),levons)
	LIBS=-L ~/levi/downloads/SDL2-2.0.5/build/.libs -lSDL2 -lfreetype -ldl -lrt
	INCLS=-I ~/levi/downloads/SDL2-2.0.5/include 
else
	LIBS=./deps/SDL2/libSDL2.a ./deps/SDL2_ttf/libSDL2_ttf.a -lfreetype -ldl -lrt
	INCLS=-I ./deps/SDL2/include -I ./deps/SDL2_ttf
endif

ifeq ($(OS),Windows_NT)
	FLAGS=-static-libstdc++ -DOS_WINDOWS -lmingw32
	#LIBS=-L ./deps/win/SDL2/lib/win32 -lSDL2
	LIBS=./deps/win/SDL2/lib/win32/SDL2.lib
	INCLS=-I ./deps/win/SDL2/include
	OUT=ChannelRouter.exe
endif



default:
	#cd ./src
	clear
	rm -f ./bin/$(OUT) 
	g++ -Wfatal-errors -g $(FLAGS) $(SRCS) $(INCLS) $(LIBS) -o ./bin/$(OUT)