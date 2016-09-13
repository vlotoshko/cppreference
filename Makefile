# compiler
CC = g++

# compiler flags
CFLAGS = -c -Wall -std=c++1y

# libraries flags
LDFLAGS =

# main target
TARGET = cppreference

# objects directory
OBJDIR = ./obj

# objects list - *.o files
_OBJECTS_WITH_HEADERS = initialization.o strings.o nvi_and_crtp.o type_deduction.o smart_pointers.o \
other.o
# adds './obj/' to every item of OBJECTS_WITH_HEADERS making new list for OBJS
OBJECTS_WITH_HEADERS = $(patsubst %, $(OBJDIR)/%, $(_OBJECTS_WITH_HEADERS))

# main is separated becasue it depends of all headers
_MAINOBJ = main.o
MAINOBJ = $(patsubst %, $(OBJDIR)/%, $(_MAINOBJ))

# first target
all: $(TARGET)

# $@ - macros that means left side of the :
# $^ - macros that means right side of the :
# $< - the first item in the dependencies list

# linking objects
$(TARGET): $(OBJECTS_WITH_HEADERS) $(MAINOBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp %.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(MAINOBJ): main.cpp *.h
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -rf *.o $(OBJDIR)/*.o $(TARGET)
