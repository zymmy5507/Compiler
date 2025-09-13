CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Iinclude
SOURCEFILES = src/lexer.cpp src/parser.cpp src/ast.cpp src/main.cpp
HEADERS = include/lexer.h include/parser.h include/ast.h

slcompiler : ${SOURCEFILES} ${HEADERS}
	${CXX} ${SOURCEFILES} ${CXXFLAGS} -o slcompiler

clean :
	rm slcompiler
