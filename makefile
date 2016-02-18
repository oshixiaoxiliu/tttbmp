CC  = g++
EXE = run.exe
OPT = 
SRC = main.cpp bmpreadwrite.cpp
OBJ = $(SRC:.cpp=.o) $(CUDASRC:.cu=.o)

all:$(EXE)
$(EXE):$(OBJ)
	  $(CC) -o $(EXE) $(OPT) $(OBJ) 

.SUFFIXES : .cpp .cu
.cpp.o:
	$(CC) -c $<
.cu.o:
	$(NVCC) $(INCLUDE) -c $<

clean:
	rm -rf *.o *.so

