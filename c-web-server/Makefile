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
all: update $(TARGET)
$(TARGET): $(OBJS) 
	$(CC) $+ $(INCS) $(LIBS) -o $@  $(CFLAG)
	cp $(TARGET) /tftpboot
	cp $(TARGET) ../bin

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -c $< $(INCS) -o $@ $(CFLAG)

update:
ifeq ($(FIND_NAME), )
	@mkdir $(OBJ)
	@chmod 777 ./ -R
	@git config --add core.filemode false
endif
	
info:
	@echo "----------   Url Download Path   ----------\n"
	@echo "\033[36m$(shell git remote -v | grep -v fetch | awk '{print $$2}')\033[0m"
	@echo "\n-------------------------------------------"

# Clean up the file
clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET)
