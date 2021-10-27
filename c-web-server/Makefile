# The target file name
TARGET := http
# The compiler
CC    := gcc
# The source file
SRC   := ./src
CSRCS := $(wildcard $(SRC)/*.c)
# The header file
INCS  := -I./inc
INCS  += -I./lib-h
# Link libraries
CFLAG := -lm -lpthread
LIBS  := -L./lib
# The file object
OBJ   := ./obj
OBJS  := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(CSRCS))

# Check whether the OBj folder exists
FIND_NAME = $(shell find -name obj)

# Compiling method
all: init $(TARGET)
$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $@  $(CFLAG)
	cp $(TARGET) /tftpboot
	cp $(TARGET) ../bin

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -c $< $(INCS) -o $@ $(CFLAG)

init:
ifeq ($(FIND_NAME), )
	@mkdir $(OBJ)
	@echo "---init successfully---";
endif

# Clean up the file
clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET)
