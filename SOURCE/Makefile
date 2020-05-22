CSRCS = $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

ASRCS = $(wildcard *.S)
AOBJS = $(ASRCS:.S=.o)

SRCS = $(CSRCS) $(ASRCS)
OBJS = $(COBJS) $(AOBJS)

%.o:%.S
	$(CC) -c $(CFLAGS) -o $@ $<

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(OBJS)
	$(LD) $(LDFLAGS) -o $(AIMAGE) $(OBJS) -L$(LIBCDIR) -L$(LIBGCCDIR) -lc -lgcc

clean:
	@rm -f *.o 
	@rm -f aImage
	@rm -f *.map
	@rm -f $(TOPDIR)/output/*
	
dep:
	$(CC) -M $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif					
