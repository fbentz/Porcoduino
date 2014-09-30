
all: command.o command

command.o: command.cc 
	g++ -c command.cc

command: command.o 
	g++ command.o -o command


clean:
	rm command 
	rm command.o



