HEADER_DIR=.
OBJDIR=obj
LIBS=-std=c++11
SRCS_DBSCAN=dbscan.cpp ds.cpp
SRCS_OPTICS=optics.cpp ds.cpp
SRCS_KMEANS=kmeans.cpp
CC=g++
DEBUG = -g
INCLUDE = -I $(HEADER_DIR)
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = $(wildcard $(HEADER_DIR)/*.h)
OBJS_DBSCAN = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS_DBSCAN))
OBJS_OPTICS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS_OPTICS))
OBJS_KMEANS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS_KMEANS))

$(OBJDIR)/%.o: %.cpp $(DEPS) | $(OBJDIR)
	$(CC) $(LIBS) $(CFLAGS) $(INCLUDE) -o $@ $< -O3

DBSCAN: $(OBJS_DBSCAN) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

OPTICS: $(OBJS_OPTICS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3

KMEANS: $(OBJS_KMEANS) | $(OBJDIR)
	$(CC) $(LIBS) $(LFLAGS) $(INCLUDE) $^ -o $@ -O3


.PHONY: clean

clean:
	rm -rf ./$(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)