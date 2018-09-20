HEADER_DIR=.
OBJDIR=obj
LIBS=-std=c++11
SRCS=dbscan.cpp ds.cpp
SRCS_OPTICS=optics.cpp ds.cpp
CC=g++
DEBUG = -g
INCLUDE = -I $(HEADER_DIR)
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = $(wildcard $(HEADER_DIR)/*.h)
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
OBJS_OPTICS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS_OPTICS))

$(OBJDIR)/%.o: %.cpp $(DEPS) | $(OBJDIR)
	$(CC) $(LIBS) $(CFLAGS) $(INCLUDE) -o $@ $< -O3

DBSCAN: $(OBJS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

OPTICS: $(OBJS_OPTICS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

.PHONY: clean

clean:
	rm -rf ./$(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)