# 编译整个工程
FILES += c-web-server
FILES += video-server
OTHER_FILES += html

FIND_NAME = $(shell find -name obj)

all:
ifeq ($(FIND_NAME), )
	mkdir bin
endif
	@for i in $(FILES);do \
		make -C $$i; \
	done

init:
	make -C $(THER_FILES)

clean:
	@for i in $(FILES);do \
		make clean -C $$i; \
	done