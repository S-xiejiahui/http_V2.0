# The target file name
TARGET := video
# The compiler
CC    := arm-linux-gcc
# The source file
SRC   := ./src
CSRCS := $(wildcard $(SRC)/*.c)
# The header file
INCS  := -I./inc
INCS  += -I./lib-include
# Link libraries
CFLAG := -lm -lpthread -ljpeg
LIBS  := -L./lib
# The file object
OBJ   := ./obj
OBJS  := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(CSRCS))
# bin
BIN   := ../bin
# Check whether the OBj folder exists
FIND_OBJ = $(shell find .  -maxdepth 1 -name "obj")
FIND_BIN = $(shell find .. -maxdepth 1 -name "bin")
FIND_TFTP= $(shell find  / -maxdepth 1 -name "tftpboot")

# Compiling method
all: init $(TARGET)
$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $(BIN)/$@  $(CFLAG)
ifneq ($(FIND_TFTP),)
	cp $(BIN)/$(TARGET) /tftpboot
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
