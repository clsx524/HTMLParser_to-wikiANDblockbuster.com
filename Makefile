CC = g++
CCFLAGS = -pedantic -Wall -Werror -g

all: Parse_model
.PHONY: all

Parse_model: Parse_model.o
	$(CC) $(CCFLAGS) Parse_model.o -o Parse_model -lcurl

Parse_model.o: Parse_model.cpp Parse_model.h
	$(CC) $(CCFLAGS) -c Parse_model.cpp -lcurl

.PHONY clean:
	rm -rf *.o *~ Parse_model
