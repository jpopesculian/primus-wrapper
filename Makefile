# ifeq ($(origin CC),default)
# 	CC = gcc
# endif
AR := ar
SH ?= bash

LOG_LEVEL ?= 0
TEST_LOG_LEVEL ?= 3

ROOT := .
PRIMUS := /usr/local/primus
OBJ := $(ROOT)
SRC := $(ROOT)
CUTEST := $(ROOT)/cutest
CONFIG := $(ROOT)/config

GLOBAL_LIB = /usr/local/lib
GLOBAL_INCLUDE = /usr/local/include
INSTALL_LIB ?= $(GLOBAL_LIB)
INSTALL_INCLUDE ?= $(GLOBAL_INCLUDE)

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

CFLAGS=-Wall -I$(GLOBAL_INCLUDE) -I$(ROOT) -I$(CONFIG) -I$(CUTEST) -I$(PRIMUS)/include -fPIC
LDFLAGS=-L$(PRIMUS)/lib -leprimus -lprimusP11

.PHONY:all clean watch

all: build-shared build-archive

build: $(OBJECTS)

build-shared: $(SHARED)

build-archive: $(ARCHIVE)

check: $(TEST_OBJECT)
	@LD_LIBRARY_PATH=$(PRIMUS)/lib:$(LD_LIBRARY_PATH) $(TEST_OBJECT)

watch:
	$(ROOT)/.watch

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS) -DLOG_LEVEL=$(LOG_LEVEL)

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

$(TEST_SOURCE): $(SOURCES) $(HEADERS)
	$(SH) cutest/make-tests.sh > $(TEST_SOURCE)

$(TEST_OBJECT): $(TEST_SOURCE) Makefile
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES) $(CUTEST_SOURCE) $(LDFLAGS) -DUNIT_TESTS=1 -DLOG_LEVEL=$(TEST_LOG_LEVEL)
