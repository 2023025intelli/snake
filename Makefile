CC ?= gcc
LIBS = -lncurses -lmenu

SRC = main.c snake.c

.PHONY=all clean

OBJDIR := objdir
OBJS := $(addprefix $(OBJDIR)/,$(SRC:.c=.o))

all: snake

$(OBJDIR)/%.o: %.c
	$(CC) -c -o $@ $<

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

snake: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJDIR)
	rm -f snake
