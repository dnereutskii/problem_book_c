CC = gcc
RM = rm
CFLAGS = -Wall -g -O0
SRCMODULES =
OBJMODULES = $(SRCMODULES:.c=.o)
PROJNAME = encrypt

# mod1.o: mod1.c mod1.h
# 	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PROJNAME): main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	$(RM) -f *.o *.mk $(PROJNAME)

run: $(PROJNAME)
	./$(PROJNAME)
