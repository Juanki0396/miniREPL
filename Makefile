CC      = gcc
SFLAGS  = -std=c11
GFLAGS  = -g
OFLAGS  = -O3
WFLAG1  = -Wall
WFLAG2  = -Wextra
WFLAG3  = -Werror
WFLAG4  = -Wstrict-prototypes
WFLAG5  = -Wmissing-prototypes
WFLAGS  = $(WFLAG1) $(WFLAG2) $(WFLAG3) $(WFLAG4) $(WFLAG5)
UFLAGS  = # Set on command line only

CFLAGS = -c $(SFLAGS) $(GFLAGS) $(OFLAGS) $(WFLAGS) $(UFLAGS)
LDFLAGS =
LDLIBS =

BUILDDIR = ./build
SOURCEDIR = ./src

PROGRAM = miniREPL
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
HEADERS = $(wildcard $(SOURCEDIR)/*.h)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

all: build_dir $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(BUILDDIR)/$@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) $^ -o $@

build_dir: 
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
