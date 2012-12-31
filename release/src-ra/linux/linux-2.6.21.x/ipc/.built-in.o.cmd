cmd_ipc/built-in.o :=  /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-ld  -m elf32ltsmip  -r -o ipc/built-in.o ipc/util.o ipc/msgutil.o ipc/msg.o ipc/sem.o ipc/shm.o ipc/ipc_sysctl.o
