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
# Check whether the OBj folder exists
FIND_NAME = $(shell find -name obj)

# Compiling method
all: init $(TARGET)
$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $@  $(CFLAG)
	cp $(TARGET) ../bin
	cp $(TARGET) /tftpboot

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
