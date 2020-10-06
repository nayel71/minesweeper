# compiler variables
CC	= gcc
LDLIBS	= `pkg-config --libs gtk+-3.0`
CFLAGS	= -Wall -MMD -MP `pkg-config --cflags gtk+-3.0`

# directory variables
SRCDIR	= src
OBJDIR	= obj
DEPDIR	= dep

# file variables
SRCS  	= $(wildcard $(SRCDIR)/*.c)
OBJS  	= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS  	= $(OBJS:.o=.d)

# targets and dependencies
.PHONY: all mkdirs clean

all: mkdirs $(OBJS) main

mkdirs:
	mkdir -p $(OBJDIR) $(DEPDIR)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

main: $(OBJS)
	$(CC) $(LDLIBS) -o $@ $^
	mv $(OBJDIR)/*.d $(DEPDIR)/

-include $(DEPS)

# clean-up
clean:
	rm -rf $(OBJDIR) $(DEPDIR) main
