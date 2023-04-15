CC=g++
CFLAGS=
LDFLAGS=-lm  -w -std=c++20
SOURCES=../logger/logger_builder.cpp ../logger/logger_builder_concrete.cpp ../logger/logger.cpp ../logger/logger_concrete.cpp memory.cpp alloc_types.cpp main.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)

.PRECIOUS: %.o