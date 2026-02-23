# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := challenge.o
    
    # --- ADD THIS LINE HERE ---
    # Disable the unused-result warning for challenge.c
	CFLAGS_challenge.o := -Wno-unused-result
    # --------------------------

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= ../../linux
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif