# 要编译的文件
FILES += c-web-server
#FILES += video-server
OTHER_FILES := html

# 要临时生成的文件
FIND_BIN  := $(shell find . -name bin)
SUBDIRS   := $(shell dirname `find . -name "makefile" -o -name "Makefile"`)
DIR_INDEX  := $(patsubst %, %/, $(SUBDIRS))

# get local ip addr
LOACL_IP_ADDR = $(shell ifconfig | grep inet | grep -v inet6 | grep -v 127 | sed 's/^[ \t]*//g' | cut -d ' ' -f2)
# Use regular expressions to match files's ip addr
JS_FILE_IP_ADDR = $(shell grep -E -o '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)' js/app.js)
OLD_IP = $(shell echo $(JS_FILE_IP_ADDR)| cut -d ' ' -f 1)

# 编译整个工程
all:
ifeq ($(FIND_BIN), )
	@mkdir bin
	@for i in $(FILES);do \
		make -C $$i $@ || exit $?; \
	done
else
	@for i in $(FILES);do \
		make -C $$i $@ || exit $?; \
	done
endif
	@echo "\n---------------------     Usage    -----------------------"
	@echo "\tYou can use \033[33m'make usage'\033[0m to view The manual"
	@echo "-----------------------------------------------------------"

# 工程初始化
init:
	@for i in $(FILES);do \
		make init -C $$i; \
	done
	@chmod 777 ./ -R
	@git config --add core.filemode false

info:
#	@sed -i 's/$(OLD_IP)/$(LOACL_IP_ADDR)/' $(files)
	@echo "-----------------local_IP-------------------\n"
	@for ip in $(LOACL_IP_ADDR);do echo "\t      \033[31m$$ip\033[0m"; done
	@echo "\n--------------Url Download Path-------------\n"
	@echo "  \033[36m$(shell git remote -v | grep -v fetch | awk '{print $$2}')\033[0m"
	@echo "\n--------------------------------------------"

# 将编译好的文件进行打包
image:
ifeq ($(FIND_BIN), )
	@echo "\033[33m--The bin file could not be found---\033[0m"
	@echo "Please perform \033[31m'make'\033[0m or \033[31m'make all'\033[0m"
else
	@tar -cvf image.tar bin html > /dev/null
	@cp image.tar /tftpboot
	@echo "\033[33m---The compressed file(image.tar) is successfully created---\033[0m"	
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

# 将工程复原成编译前的状态
recovery: 
	@-rm -rf bin
	@for i in $(FILES);do \
		rm -rf $$i/obj; \
	done
	@echo "\n\t---recovery successfully---\n"

usage:
	@echo "---------------------     make    -----------------------"
	@echo "You can use \033[33m'make info'\033[0m to view local IP"
	@echo "You can use \033[33m'make image'\033[0m package files"
	@echo "You can use \033[33m'make clean'\033[0m clean files"
	@echo "You can use \033[33m'make .../_only(_clean)'\033[0m to make(clean) a directory you want"
	@echo "---------------------     Usage    -----------------------"
	@echo "Usage 1:     \033[36m./bin/http\033[0m (default:  port = 8080; ip = localhost)"
	@echo "Usage 2:     \033[36m./bin/http <ip>\033[0m"
	@echo "Usage 3:     \033[36m./bin/http <port>\033[0m"
	@echo "Usage 4:     \033[36m./bin/http <port> <ip>\033[0m"
	@echo "-----------------------------------------------------------"