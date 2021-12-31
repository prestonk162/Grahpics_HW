CXX = g++

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

CXXFLAGS = -g
LDLIBS = -lm -lGL -lGLU -lglut

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC = -I$(INC_DIR)

TARGET = program

$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) $(INC) -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INC)

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)