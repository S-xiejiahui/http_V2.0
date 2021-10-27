# 编译的文件
FILES += c-web-server
FILES += video-server
OTHER_FILES := html

FIND_BIN  := $(shell find . -name bin)
SUBDIRS   := $(shell dirname `find . -name "makefile" -o -name "Makefile"`)
DIR_INDEX  := $(patsubst %, %/, $(SUBDIRS))

# 编译整个工程
all:
ifeq ($(FIND_BIN), )
	mkdir bin
	@for i in $(FILES);do \
		make -C $$i $@ || exit $?; \
	done
else
	@for i in $(FILES);do \
		make -C $$i $@ || exit $?; \
	done
endif

# 工程初始化
init:
	make -C $(OTHER_FILES)

# 将编译好的文件进行打包
image:
ifeq ($(FIND_BIN), )
	@echo "\033[33m--The bin file could not be found---\033[0m"
	@echo "Please perform \033[31m'make' or 'make all'\033[0m"
else
	@tar -cvf image.tar bin html > /dev/null
	@cp image.tar /tftpboot
	@echo "\033[33m---The compressed file is successfully created---\033[0m"	
endif

# 编译单个目录
$(DIR_INDEX):
%_only:
	@case "$(@)" in \
	*/*) d=`expr $(@) : '\(.*\)_only'`; \
	     make -C $$d; \
	esac

# 清理单个目录
%_clean:
	@case "$(@)" in \
	*/*) d=`expr $(@) : '\(.*\)_clean'`; \
	     make -C $$d clean; \
	esac

# 清理所有目录
clean:
	@for i in $(FILES);do \
		make clean -C $$i; \
	done
	-rm -rf bin/*

