CC = gcc
CFLAGS = -g `pkg-config --cflags glib-2.0` \
	-Wall \
	-Wextra \
	-Wundef \
	-Wnested-externs \
	-Wwrite-strings \
	-Wpointer-arith \
	-Wmissing-declarations \
	-Wmissing-prototypes \
	-Wstrict-prototypes \
	-Wredundant-decls \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wdeclaration-after-statement \
	-Wformat=2 \
	-Wold-style-definition \
	-Wcast-align \
	-Wformat-nonliteral \
	-Wformat-security \
	-Wsign-compare \
	-Wstrict-aliasing \
	-Wshadow \
	-Winline \
	-Wpacked \
	-Wmissing-format-attribute \
	-Wmissing-noreturn \
	-Winit-self \
	-Wredundant-decls \
	-Wmissing-include-dirs \
	-Wunused-but-set-variable \
	-Warray-bounds \
	-Wimplicit-function-declaration \
	-Wreturn-type \
	-Wswitch-enum \
	-Wswitch-default
LDFLAGS = -lclang `pkg-config --libs glib-2.0`
SRCS = test.c
OBJS = $(SRCS:.c=.o)
EXECUTABLE = test

.PHONY: clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)
