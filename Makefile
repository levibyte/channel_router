LIBS=./deps/SDL2/libSDL2.a
INCLS=-I ./deps/SDL2/include

default:
	rm -rf ztest
	g++ -Wfatal-errors  test.cpp  $(LIBS) $(INCLS) -g  -lpthread -ldl -o ztest