PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj

OBJ = $(OBJDIR)/main.o $(OBJDIR)/Application.o $(OBJDIR)/RenderSystem.o $(OBJDIR)/Window.o $(OBJDIR)/Actor.o

FLAGS = -std=c++0x -Wall -c

# Some of the linked libraries are obviously only for compiling
# on windows. Remove 'em and things should be fine on other systems.
all: $(OBJ)
	c++ -o $(PROJECT_NAME) $(OBJ) -lmingw32 -lSDL2main -lSDL2 -mwindows -lglu32 -lopengl32 -lglew32

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	c++ $(FLAGS) -o $(OBJDIR)/main.o $(SRCDIR)/main.cpp

$(OBJDIR)/Application.o: $(SRCDIR)/Application.cpp $(SRCDIR)/Application.h
	c++ $(FLAGS) -o $(OBJDIR)/Application.o $(SRCDIR)/Application.cpp

$(OBJDIR)/RenderSystem.o: $(SRCDIR)/RenderSystem.cpp $(SRCDIR)/RenderSystem.h
	c++ $(FLAGS) -o $(OBJDIR)/RenderSystem.o $(SRCDIR)/RenderSystem.cpp
	
$(OBJDIR)/Window.o: $(SRCDIR)/Window.cpp $(SRCDIR)/Window.h
	c++ $(FLAGS) -o $(OBJDIR)/Window.o $(SRCDIR)/Window.cpp
	
$(OBJDIR)/Actor.o: $(SRCDIR)/Actor.cpp $(SRCDIR)/Actor.h
	c++ $(FLAGS) -o $(OBJDIR)/Actor.o $(SRCDIR)/Actor.cpp

clean:
	rm -f $(OBJ) $(PROJECT_NAME)


