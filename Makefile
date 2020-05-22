#
################################################################################
#                                                                               
#     ####   ###   #####           #####   <T.M>                                
#    #        #    #      #     #  #                                            
#     ####    #    ####    #   #   ####    Powered by Course-Ware Framework     
#         #   #    #        # #    #          (C) Copyrights, In4MaySun Inc     
#    #####   ###   #####     #     #####                                        
#                                                         www.in4maysun.com     
#                                                          by Seo Kwang-Won
################################################################################
#

.EXPORT_ALL_VARIABLES:

TOPDIR 	= $(shell pwd)
VERSION = 4.4.1
TARGET	= arm-none-eabi
AIMAGE 	= image

AS			= $(TOOLPATH)/bin/$(TARGET)-as
CC			= $(TOOLPATH)/bin/$(TARGET)-gcc
LD 			= $(TOOLPATH)/bin/$(TARGET)-ld
OBJCOPY	 	= $(TOOLPATH)/bin/$(TARGET)-objcopy
OBJDUMP 	= $(TOOLPATH)/bin/$(TARGET)-objdump

LIBCDIR 	= $(TOOLPATH)/$(TARGET)/lib
LIBGCCDIR 	= $(TOOLPATH)/lib/gcc/$(TARGET)/$(VERSION)


INCLUDE	=  -I$(TOPDIR)/SOURCE -I$(LIBCDIR)/include -I$(LIBGCCDIR)/include

CFLAGS	+= $(INCLUDE) -g -Wall 
CFLAGS 	+= -march=armv5te -mtune=arm9e 
CFLAGS 	+= -mapcs		#### important option for calling isr routines   ###
#CFLAGS	+= 	-fno-builtin

CFLAGS  += -D$(EXAMPLE)

LDFLAGS	=  -Bstatic -nostartfiles -T ../S3C2450-RAM.ld
LDFLAGS	+= -e _start


OCFLAGS = -O binary -R .note -R .comment -S

all: $(AIMAGE).bin

$(AIMAGE).bin :
	make -C $(TOPDIR)/SOURCE || exit 1
	$(OBJCOPY) $(OCFLAGS) SOURCE/$(AIMAGE)   output/$@
	mv SOURCE/$(AIMAGE)   output/

	@echo ======== ======== ======== ======== ======== ======== ========
	@echo  The build output $(AIMAGE) and $(AIMAGE).bin  at dir output:
	@  ls  output/
	@echo ======== ======== ======== ======== ======== ======== ========
	@echo  
	@echo  download by Trace32  "B:: data.load.elf  image"
	@echo  download by u-boot   "loadb "
	@echo  
	@echo ======== ======== ======== ======== ======== ======== ========




	
clean:
	make -C $(TOPDIR)/SOURCE clean

help:


dep:
	make -C $(TOPDIR)/SOURCE dep
