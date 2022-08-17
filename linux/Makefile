CHAIN_ROOT=/home/lxb/buildroot-2020.02.6/output/host/bin
CROSS_COMPILE=$(CHAIN_ROOT)/mipsel-buildroot-linux-musl-

#CHAIN_ROOT= /home/yang/b503/ctools/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin
#CROSS_COMPILE=$(CHAIN_ROOT)/arm-linux-gnueabihf-
#CROSS_COMPILE = 

CC     := $(CROSS_COMPILE)gcc
#CC = arm-poky-linux-gnueabi-gcc
TARBALL = InfoNES08J

# InfoNES
.CFILES =	./../K6502.cpp \
		./../InfoNES.cpp \
		./../InfoNES_Mapper.cpp \
		./../InfoNES_pAPU.cpp \
		./InfoNES_System_Linux.cpp
#joypad_input.cpp

.OFILES	=	$(.CFILES:.cpp=.o)

CCFLAGS =    -o2 -fsigned-char  -I../ --sysroot /home/lxb/buildroot-2020.02.6/output/staging
LDFILGS = -lstdc++	-L../libs	--sysroot /home/lxb/buildroot-2020.02.6/output/staging# gcc3.x.x

all: InfoNES

InfoNES: $(.OFILES)
	$(CC) $(INCLUDES) -o $@ $(.OFILES) $(LDFILGS) -lm  -lpthread -lasound

.cpp.o:
	$(CC) $(INCLUDES) -c $(CCFLAGS) $*.cpp  -o $@

clean:
	rm -f $(.OFILES) ../*~ ../*/*~ core

cleanall:
	rm -f $(.OFILES) ../*~ ../*/*~ core InfoNES

release: clean all

tar:
	( cd ..; \
	tar cvf $(TARBALL).tar ./*; \
	gzip $(TARBALL).tar \
	)

install:
	install ./InfoNES /usr/local/bin
