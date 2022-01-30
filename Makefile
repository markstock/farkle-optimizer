all : farkle-optimizer

farkle-optimizer : playfarkle.cpp
	g++ -o playfarkle -Ofast playfarkle.cpp

