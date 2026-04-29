# Compiler
CXX = g++

# Compiler flags (-I. for third_party/cpp-httplib and local includes)
CXXFLAGS = -Wno-c++11-extensions --std=c++17 -Wall -I. -Ithird_party

# Shared objects (no main.cpp / http_server.cpp)
COMMON_CPP = armor.cpp \
	attacking.cpp \
	commands.cpp \
	dungeon.cpp \
	helper_fxns.cpp \
	main_commands.cpp \
	object.cpp \
	room.cpp \
	weapon.cpp \
	static_object_factory.cpp \
	db/rooms_db.cpp \
	db/static_objects_db.cpp \
	game_io.cpp \
	game_input.cpp \
	game_session.cpp

COMMON_OBJ = $(COMMON_CPP:.cpp=.o)

# CLI game
TARGET = game.exe

# Default rule
all: $(TARGET)

$(TARGET): main.o $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

http_server: http_server.o $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(COMMON_OBJ) main.o http_server.o $(TARGET) http_server
