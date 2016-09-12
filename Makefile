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
OBJECTS = initialization.o strings.o nvi_and_crtp.o type_deduction.o smart_pointers.o \
other.o main.o

## main is separated becasue it does not depend of header
#MAINOBJ = main.o

OBJS = $(patsubst %, $(OBJDIR)/%, $(OBJECTS))

# SRCDIR = ./src
# SRCS = $(patsubst %, $(SRCDIR)/%, $(OBJECTS))


# first target
all: $(TARGET)

# $@ - macros that means left side of the :
# $^ - macros that means right side of the :
# $< - the first item in the dependencies list
$(TARGET): $(OBJS)
	echo "OBJS ::: " $^
	$(CC) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	echo "CPPS ::: " $^
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean
clean:
	rm -rf *.o $(OBJDIR)/*.o $(TARGET)
