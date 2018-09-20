HEADER_DIR=.
OBJDIR=obj
LIBS=-std=c++11
SRCS=dbscan.cpp ds.cpp
CC=g++
DEBUG = -g
INCLUDE = -I $(HEADER_DIR)
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = $(wildcard $(HEADER_DIR)/*.h)
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: %.cpp $(DEPS) | $(OBJDIR)
	$(CC) $(LIBS) $(CFLAGS) $(INCLUDE) -o $@ $< -O3

DBSCAN: $(OBJS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

DEBUG_DBSCAN: CFLAGS += -DDEBUG
DEBUG_DBSCAN: DBSCAN

SDEBUG_DBSCAN: CFLAGS += -DSIMPLE_DEBUG
SDEBUG_DBSCAN: DBSCAN

.PHONY: clean

clean:
	rm -rf ./$(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)