# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wno-c++11-extensions --std=c++17 -Wall -Ithird_party

# Source files
SRC = armor.cpp \
	attacking.cpp \
	commands.cpp \
	dungeon.cpp \
	helper_fxns.cpp \
	main_commands.cpp \
	main.cpp \
	object.cpp \
	room.cpp \
	weapon.cpp \
	static_object_factory.cpp \
	db/rooms_db.cpp \
	db/static_objects_db.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Target executable
TARGET = game.exe

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)
