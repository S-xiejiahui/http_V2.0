# 编译整个工程
FILES += c-web-server
FILES += video-server

all:
	@for i in $(FILES);do \
		make -C $$i; \
	done

clean:
	@for i in $(FILES);do \
		make clean -C $$i; \
	done