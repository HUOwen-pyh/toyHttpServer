server: main.o AmasumiServer.o
	@if [ ! -e "./bin" ]; then mkdir bin; fi
	@g++ $^ -o ./bin/server
	@rm -rf $^ *.h.gch

main.o: main.cpp
	@g++ main.cpp -c main.o

a.o: AmasumiServer.h AmasumiServer.cpp
	@g++ $^ -c $@

.PHONY : clean
clean:
	@rm -rf bin
