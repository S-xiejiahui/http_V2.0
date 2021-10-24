# 编译整个工程
FILES += c-web-server
FILES += video-server

OTHER_FILES += html

all:
	@for i in $(FILES);do \
		make -C $$i; \
	done

init:
	make -C $(THER_FILES)

clean:
	@for i in $(FILES);do \
		make clean -C $$i; \
	done