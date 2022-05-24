define NEWLINE

endef

all:football.o
	g++ src/football.cpp -o football.exe
	@echo Executing...
	@football.exe
clean:
	@rm football.exe