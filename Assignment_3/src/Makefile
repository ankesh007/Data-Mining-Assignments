HEADER_DIR=.
OBJDIR=obj
LIBS=-std=c++11
SRCS=candidate_list.cpp edge_evolution.cpp main.cpp declaration.cpp feature.cpp pattern.cpp
CC=g++
DEBUG = -g
INCLUDE = -I $(HEADER_DIR)
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)
DEPS = $(wildcard $(HEADER_DIR)/*.h)
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: %.cpp $(DEPS) | $(OBJDIR)
	$(CC) $(LIBS) $(CFLAGS) $(INCLUDE) -o $@ $< -O3

GAIA: $(OBJS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

.PHONY: clean

clean:
	rm -rf ./$(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
