CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Wextra

SRC = main.cpp InputHandler.cpp LeitnerBox.cpp Card.cpp 
OBJ = $(SRC:.cpp=.o)

TARGET = LeitnerBox

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
