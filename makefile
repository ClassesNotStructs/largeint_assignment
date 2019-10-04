CC = g++
SRC = src/main.cpp src/largeint.cpp
INC = -I inc
OBJ = main.o largeint.o
TARGET = test
CXXFLAGS = -Wall 

$(TARGET) : $(OBJ)
	$(CC) -o $@ $^

$(OBJ) : $(SRC)
	$(CC) $(CXXFLAGS) $(INC) -c $(SRC)

.PHONY : clean

clean : $(OBJ) $(TARGET)
	del $(OBJ) $(TARGET).exe