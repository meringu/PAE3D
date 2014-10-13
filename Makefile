CC = g++
IPATH = -I/usr/X11/include -I/usr/pkg/include -I./include
LPATH = -L/usr/X11/lib -L/usr/pkg/lib
LDPATH = -Wl,-R/usr/pkg/lib
LIBS= -lGL -lglut -lGLU -ljpeg -lpng16 -lm
BUILD = build/
SRC = src/

all: $(BUILD) $(BUILD)PAE3D

$(BUILD):
	mkdir $(BUILD)

$(BUILD)PAE3D: $(BUILD)main.o $(BUILD)Model.o $(BUILD)Color.o $(BUILD)Handle.o $(BUILD)ImageLoader.o $(BUILD)Toolbar.o $(BUILD)RenderHelper.o
	$(CC) -o $@ $^ $(LPATH) $(LDPATH) $(LIBS)

$(BUILD)%.o: $(SRC)%.cpp
	$(CC) -c -o $@ $^ $(IPATH)

clean:
	$(RM) -f $(BUILD)*.o $(BUILD)PAE3D
