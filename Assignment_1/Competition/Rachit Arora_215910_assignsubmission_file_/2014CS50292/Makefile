CC=g++ -std=c++11
CFLAGS= -O3
# CFLAGS= -g

INC=-I./include
SRC=./src
OBJ=bin
EXE=freq_itemset

SRCFILES=$(addprefix $(OBJ)/, $(subst .c,.o, $(subst .cpp,.o, $(subst $(SRC)/,,$(wildcard $(SRC)/*)))))

all:
	make clean
	mkdir $(OBJ)
	make proxy

proxy: $(SRCFILES)
	mkdir -p $(OBJ)
	$(CC) -o $(EXE) $(SRCFILES) $(INC) $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	$(CC) -c $< -o $@ $(INC) $(CFLAGS)

clean:
	rm -rf $(OBJ)
	rm -f $(EXE)
