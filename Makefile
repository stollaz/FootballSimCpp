define NEWLINE

endef

all:football.o
	g++ -std=gnu++17 src/football.cpp -o football.exe
	@echo Executing...
	@football.exe
clean:
	@rm football.exe