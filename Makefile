
SEAL_INCLUDE=${HOME}/.local/include/SEAL-3.7
SEAL_LIB=seal
NTT_LIB=nttw
SRC=./ensei_seal.cpp
EXE=ensei_bench

$(EXE): $(SRC)
	$(CXX) -gdwarf-3 -I$(SEAL_INCLUDE) -l$(SEAL_LIB) -l$(NTT_LIB) $(SRC) -o $(EXE)

all: $(EXE)

clean:
	rm $(EXE)
