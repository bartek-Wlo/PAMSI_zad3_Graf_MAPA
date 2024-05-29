# make -> komenda do kompilacji
# make clean -> komenda do usunie wszystkie pliki obiektowe i plik wykonywalny

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -Werror=return-type -Werror=parentheses -Werror=vla -Werror=array-bounds -Wcast-qual -Wmaybe-uninitialized

TARGET = wykonywalny.out
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(wildcard *.h) # Dodajemy zależności plików nagłówkowych

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp $(DEPS) # Dodajemy zależności plików nagłówkowych
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS) $(TARGET)



	