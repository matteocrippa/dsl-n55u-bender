
all:
	make -C UTIL/
	$(SHELL) cp_util.sh
	make -C MODULE/
	$(SHELL) cp_module.sh
	make -C NETIF/

clean:
	make -C UTIL/ clean
	make -C MODULE/ clean
	make -C NETIF/ clean

