.PHONY : all clean

CFLAGS := -Wall -std=gnu99 -D_GNU_SOURCE -L/usr/lib -lbluetooth
DIRS := $(shell find . -type d)
FILES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS := $(addsuffix .o, $(basename $(FILES)))
APP := BROFist

all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(APP)

clean:
	rm -f $(OBJS) $(APP) $(OBJS:.o=.d)

%.d: %.c
	$(CC) $(CFLAGS) -MM -MF $@ $<

-include $(OBJS:.o=.d)
