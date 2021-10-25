CC = g++
IPATH = -I./include
OS:=$(shell uname)
ifeq ($(OS),Darwin)
    LIBS= -framework OpenGL -framework Cocoa -lSDL2 -lSDL2_image
endif
ifeq ($(OS),Linux)
    LIBS= -lGL -lGLU -lSDL2 -lSDL2_image
endif
BUILD = build/$(OS)/
SRC = src/
CPP_FILES := $(wildcard src/*.cpp)
O_FILES := $(addprefix $(BUILD),$(notdir $(CPP_FILES:.cpp=.o)))

all: $(BUILD) $(BUILD)PAE3D

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)PAE3D: $(O_FILES)
	$(CC) -o $@ $^ $(LIBS)

$(BUILD)%.o: $(SRC)%.cpp
	$(CC) -c -o $@ $^ $(IPATH)

clean:
	$(RM) -f $(BUILD)*
