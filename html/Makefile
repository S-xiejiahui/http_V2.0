# get local ip addr
LOACL_IP_ADDR = $(shell ifconfig | grep inet | grep -v inet6 | grep -v 127 | sed 's/^[ \t]*//g' | cut -d ' ' -f2)
# Use regular expressions to match files's ip addr
JS_FILE_IP_ADDR = $(shell grep -E -o '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)' js/app.js)
OLD_IP = $(shell echo $(JS_FILE_IP_ADDR)| cut -d ' ' -f 1)

# Update IP to match local
files := js/app.js

all:
	@sed -i 's/$(OLD_IP)/$(LOACL_IP_ADDR)/' $(files)
	@echo "\033[33mUpdate success:\033[0m"
	@echo "replace \033[36mold_IP($(OLD_IP)\033[0m) as \033[31mlocal_IP($(LOACL_IP_ADDR)\033[0m)"