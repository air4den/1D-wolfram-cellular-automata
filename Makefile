CC := g++
SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SRCDIR := src

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, %.o, $(SRCS))

CFLAGS := 
DEBUGFLAGS := -g

TARGET := 1D-wolfram-automata

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(OBJS) -o $@ $(SFMLFLAGS)

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

.PHONY: clean run debug 

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	lldb ./$(TARGET)

clean:
	rm -f *.o $(TARGET)