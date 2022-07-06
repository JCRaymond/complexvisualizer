CC=g++
SDIR=./src
IDIR=./include
BDIR=./bin
LIBPNG_CFLAGS := $(libpng-config --cflags)
CFLAGS=$(LIBPNG_CFLAGS) -std=c++20 -I$(SDIR) -I$(IDIR) -Wno-narrowing -fopenmp -Ofast
IDEPS=$(wildcard $(IDIR)/*)
SDEPS=$(wildcard $(SDIR)/*)
FDEPS=
DEPS=$(IDEPS) $(SDEPS) $(FDEPS)

%: ./src/%.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -lpng -o $(BDIR)/$@

