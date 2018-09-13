HEADER_DIR=.
OBJDIR=obj
LIBS=-std=c++11
SRCS=FP_Tree.cpp
CC=g++
DEBUG = -g
INCLUDE = -I $(HEADER_DIR)
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = $(wildcard $(HEADER_DIR)/*.h)
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: %.cpp $(DEPS) | $(OBJDIR)
	$(CC) $(LIBS) $(CFLAGS) $(INCLUDE) -o $@ $< -O3

DEBUG_FP_TREE: CFLAGS += -DDEBUG
DEBUG_FP_TREE: FP_Tree

SDEBUG_FP_TREE: CFLAGS += -DSIMPLE_DEBUG
SDEBUG_FP_TREE: FP_Tree


FP_Tree: $(OBJS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

.PHONY: clean

clean:
	rm -rf ./$(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)