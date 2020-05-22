CC=gcc
CFLAGS=-Iinclude -w -fcompare-debug-second

MAKEFLAGS=-s

IDIR=include
ODIR=obj

OUT=main

LIBS=-lncurses -lm

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = hud.o life.o main.o alien.o person.o player.o screen.o vector.o terrain.o projectile.o ## Probably need to find a better way of doing this
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS) | $(ODIR)
	@$(CC) -c -o $@ $< $(CFLAGS)
	@printf %-15.15s "   $<"
	@printf ' compiled to $@\n'

$(OUT): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@printf '\n   Object files linked to: $@\n'
	@rm $(ODIR) -r
	@printf '    - $(ODIR) directory removed\n'
	@printf '\n   Use `make run` or `./$@` to run\n'

$(ODIR):
	@mkdir -p $(ODIR)

clean:
	@mkdir -p $(ODIR)
	@echo 'fake main' > $(OUT)
	@rm $(ODIR) -r
	@rm $(OUT)
	@printf '   Directory cleaned\n'

run: $(OUT)
	@printf '\n   Game Exited\n'
	-@./$^ ||:

open:
	code *.c $(IDIR)/* Makefile ../README.md