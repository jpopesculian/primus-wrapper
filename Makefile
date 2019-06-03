ifeq ($(origin CC),default)
	CC = gcc
endif
AR := ar

LOG_LEVEL ?= 3

ROOT := .
PRIMUS := /usr/local/primus
OBJ := $(ROOT)
SRC := $(ROOT)
GLOBAL_LIB = /usr/local/lib
GLOBAL_INCLUDE = /usr/local/include
INSTALL_LIB ?= $(GLOBAL_LIB)
INSTALL_INCLUDE ?= $(GLOBAL_INCLUDE)
CUTEST := $(ROOT)/cutest

NAME := primus_wrapper
SOURCES := $(wildcard $(SRC)/*.c)
HEADERS := $(wildcard $(SRC)/*.h)

OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

TEST_NAME := primus_wrapper_test
TEST_SOURCE := $(SRC)/$(TEST_NAME).c
TEST_OBJECT := $(OBJ)/$(TEST_NAME).o
SOURCES := $(filter-out $(TEST_SOURCE), $(SOURCES))
OBJECTS := $(filter-out $(TEST_OBJECT), $(OBJECTS))
CUTEST_SOURCE := $(CUTEST)/CuTest.c

SHARED := $(OBJ)/lib$(NAME).so
ARCHIVE := $(OBJ)/lib$(NAME).a

CFLAGS=-Wall -I$(GLOBAL_INCLUDE) -I$(ROOT) -I$(PRIMUS)/include -fPIC
LDFLAGS=-L$(PRIMUS)/lib -leprimus -lprimusP11

.PHONY:all clean watch

all: build-shared build-archive

build: $(OBJECTS)

build-shared: $(SHARED)

build-archive: $(ARCHIVE)

check: $(TEST_OBJECT)
	@$(TEST_OBJECT)

$(OBJ)/%.o: $(SRC)/%.c Makefile
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

$(SHARED): $(OBJECTS)
	$(CC) -shared -o $@ $<

$(ARCHIVE): $(OBJECTS)
	$(AR) rcs $@ $<

install: uninstall build-shared build-archive
	cp $(HEADERS) $(INSTALL_INCLUDE)
	cp $(SHARED) $(INSTALL_LIB)
	cp $(ARCHIVE) $(INSTALL_LIB)

uninstall:
	rm -f $(patsubst $(SRC)/%.h, $(INSTALL_INCLUDE)/%.h, $(HEADERS))
	rm -f $(patsubst $(OBJ)/%.so, $(INSTALL_LIB)/%.so, $(SHARED))
	rm -f $(patsubst $(OBJ)/%.a, $(INSTALL_LIB)/%.a, $(ARCHIVE))

clean:
	rm -f $(OBJECTS)
	rm -f $(SHARED)
	rm -f $(ARCHIVE)
	rm -f $(TEST_SOURCE)
	rm -f $(TEST_OBJECT)

$(TEST_SOURCE): $(SOURCES)
	bash cutest/make-tests.sh > $(TEST_SOURCE)

$(TEST_OBJECT): $(TEST_SOURCE) $(SOURCES) $(CUTEST_SOURCE) Makefile
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES) $(CUTEST_SOURCE) $(LDFLAGS) -I$(CUTEST) -DUNIT_TESTS=1 -DLOG_LEVEL=$(LOG_LEVEL)
