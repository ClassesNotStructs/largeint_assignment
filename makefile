CC = g++
SRC = src/main.cpp src/largeint.cpp
INC = -I inc
OBJ = main.o largeint.o
TARGET = bin/test
CXXFLAGS = -Wall 

$(TARGET) : $(OBJ)
	$(CC) -o $@ $^

$(OBJ) : $(SRC)
	$(CC) $(CXXFLAGS) $(INC) -c $(SRC)

.PHONY : clean cleanobj

clean : $(OBJ) $(TARGET)
	del $(OBJ) $(TARGET).exe

cleanobj : $(OBJ)
	del $(OBJ)