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
FIND_OBJ = $(shell find .  -maxdepth 1 -name "obj")
FIND_BIN = $(shell find .. -maxdepth 1 -name "bin")
FIND_TFTP= $(shell find  / -maxdepth 1 -name "tftpboot")

# Compiling method
all: init $(TARGET)
$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $@  $(CFLAG)
ifneq ($(FIND_TFTP), )
	cp $(TARGET) /tftpboot
endif

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -c $< $(INCS) -o $@ $(CFLAG)

init:
ifeq ($(FIND_OBJ), )
	@mkdir $(OBJ)
	@echo "---init successfully---";
endif
ifeq ($(FIND_BIN), )
	@mkdir ../bin
endif

# Clean up the file
clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET)
