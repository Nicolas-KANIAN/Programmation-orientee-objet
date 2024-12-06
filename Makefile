CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2 -Iinclude
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

SRCS = src/Cellule.cpp \
       src/CelluleVivante.cpp \
       src/CelluleMorte.cpp \
       src/Grille.cpp \
       src/FichierTexte.cpp \
       src/Simulation.cpp \
       src/InterfaceGraphique.cpp \
       src/main.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main-debug: CXXFLAGS += -U_FORTIFY_SOURCE -O0
main-debug: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDLIBS)

clean:
	rm -f src/*.o $(TARGET) $(TARGET).exe main-debug main-debug.exe

.PHONY: all clean main-debug
