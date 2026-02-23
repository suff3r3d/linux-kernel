obj-m += challenge.o
LINUX = ../../../linux

ccflags-y += -Wno-error -DDEBUG -g

all:
	make -C $(LINUX) M=$(PWD) modules

clean:
	make -C $(LINUX) M=$(PWD) clean