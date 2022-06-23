define NEWLINE

endef

all:football.o
	@echo Building...
	g++ -std=gnu++17 -Wall src/football.cpp -o football.exe
	@echo Executing...
	@football.exe
build:football.o
	@echo Building...
	g++ -std=gnu++17 -Wall src/football.cpp -o football.exe
execute:football.o
	@echo Executing...
	@football.exe
clean:
	@echo Cleaning...
	@rm football.exe