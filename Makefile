CC = g++
CFLAGS = -c -Wall -std=c++1y

TARGET = cppreference
ODIR = ./obj
_OBJS = initialization.o strings.o main.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $^


$(ODIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<


.PHONY: clean
clean:
	rm -rf *.o $(ODIR)/*.o $(TARGET)
