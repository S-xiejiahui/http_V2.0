# 编译整个工程
FILES += c-web-server
FILES += video-server
OTHER_FILES := html

FIND_OBJ = $(shell find -name obj)
SUBDIRS  = $(shell dirname `find . -name "makefile" -o -name "Makefile"`)
DIR_INDEX  := $(patsubst %, %/, $(SUBDIRS))


all:
ifeq ($(FIND_OBJ), )
	mkdir bin
endif
	@for i in $(FILES);do \
		make -C $$i $@ || exit $?; \
	done

init:
	make -C $(OTHER_FILES)

$(DIR_INDEX):
%_only:
	@case "$(@)" in \
	*/*) d=`expr $(@) : '\(.*\)_only'`; \
	     make -C $$d; \
	esac

%_clean:
	@case "$(@)" in \
	*/*) d=`expr $(@) : '\(.*\)_clean'`; \
	     make -C $$d clean; \
	esac

clean:
	@for i in $(FILES);do \
		make clean -C $$i; \
	done