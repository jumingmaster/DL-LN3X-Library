vpath %.hpp ./include
vpath %.cpp ./src

INCLUDE = ./include
CPPFLAGS = $(DEBUG) -I $(INCLUDE) -Wall -std=c++11 -lstdc++ -pthread

objects = test.o Serial.o

test: $(objects)
	cc $(CPPFLAGS) -o test $(objects)


Serial.o: Serial.hpp
test.o : Serial.hpp

.PHONY: clean

clean:
	rm test $(objects)
