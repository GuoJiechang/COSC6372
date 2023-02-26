SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=%.o)
EXEC = hw2
CPP_FLAGS='-std=c++11'
COMPILER = g++
DELETE = rm

$(EXEC) : $(OBJECTS)
	$(COMPILER) $(CPP_FLAGS) $(OBJECTS) -o $(EXEC)

%.o : $(SRC_DIR)/%.cpp
	$(COMPILER) $(CPP_FLAGS) -c $<

.PHONY : all
all : $(EXEC)

.PHONY : clean
clean :
	-$(DELETE) $(OBJECTS) core
