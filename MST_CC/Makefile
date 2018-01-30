#g++ main_code1.cpp -I../ESSENS/Basic_IO/Process/Preprocess/Level0 -I../ESSENS/Basic_IO/Process/Preprocess/Exceptions/ -I../ESSENS/Basic_IO/Process/Preprocess//Level1 -I../ESSENS/Basic_IO/Process/Preprocess/Level2 -I../ESSENS/Basic_IO/Format/Input_Format/Level0/ADJ/ -I../ESSENS/Basic_IO/Format/Input_Format/Level0/CSR/ -I../ESSENS/Basic_IO/Format/Input_Format/Level2

#Change thos line to
#ESSENS=<Path to Where ESSENS is Stored>/ESSENS
ESSENS=/Users/alexbaker1/Desktop/ESSENS
ifeq ($(shell uname -s ),Darwin)
	CFLAGS += -mmacosx-version-min=10.7 -Wall
else
	CFLAGS += -w
endif


#Including BASIC_IO
BASIC_IO = $(ESSENS)/Core/Basic_IO

BASIC_IO_PREPROCESS=$(BASIC_IO)/PreProcess

BASIC_IO_FORMAT=$(BASIC_IO)/Format
BASIC_IO_FORMAT_INPUT=$(BASIC_IO_FORMAT)/Input
BASIC_IO_FORMAT_OUTPUT=$(BASIC_IO_FORMAT)/Output

BASIC_IO_INC=-I$(BASIC_IO_PREPROCESS)/Exceptions\
 -I$(BASIC_IO_PREPROCESS)/Level0 \
 -I$(BASIC_IO_PREPROCESS)/Level1 \
 -I$(BASIC_IO_PREPROCESS)/Level2 \
 -I$(BASIC_IO_FORMAT_INPUT_FORMAT)/Exceptions \
 -I$(BASIC_IO_FORMAT)/Level0/ \
 -I$(BASIC_IO_FORMAT_INPUT)/Level1/ -I$(BASIC_IO_FORMAT_INPUT)/Level2 \
-I$(BASIC_IO_FORMAT_OUTPUT)/Level1/ -I$(BASIC_IO_FORMAT_OUTPUT)/Level2 


BASIC_SETOP=$(ESSENS)/Core/Basic_SetOps

BASIC_SETOP_INC=-I$(BASIC_SETOP)/Level0 \
-I$(BASIC_SETOP)/Level1/ \
-I$(BASIC_SETOP)/Level2

BASIC_TRAVERSAL=$(ESSENS)/Core/Basic_Traversal

BASIC_TRAVERSAL_INC=-I$(BASIC_TRAVERSAL)/Level0 \
-I$(BASIC_TRAVERSAL)/Level1 \
-I$(BASIC_TRAVERSAL)/Level2

BASIC_CHANGE=$(ESSENS)/Basic_Change

BASIC_CHANGE_INC=-I$(BASIC_CHANGE)/Level0 \
-I$(BASIC_CHANGE)/Level1 \
-I$(BASIC_CHANGE)/Level2

BASIC_ANALYSIS=$(ESSENS)/Basic_Analysis
BASIC_ANALYSIS_INC=-I$(BASIC_ANALYSIS)/Level0

all: 	main_code1x.cpp
	$(CXX) -c -g $(CFLAGS) -fopenmp $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) main_code1x.cpp -o main_code1x.o
	$(CXX) -g $(CFLAGS) -fopenmp $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) main_code1x.o $(RAPL_INC)
cE: 	create_edgelist.cpp
	$(CXX) -g $(CFLAGS) -o cE.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) create_edgelist.cpp
tEx: 	traversalEx.cpp
	$(CXX) -g $(CFLAGS) -o tEx.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) traversalEx.cpp
bfs: 	BFS.cpp
	g++ -g $(CFLAGS) -o bfs.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) BFS.cpp
chChk: 	chordalchk.cpp
	$(CXX) -g $(CFLAGS) -o chChk.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) chordalchk.cpp
lpath: 	longest_path.cpp
	$(CXX) -g $(CFLAGS) -o lpath.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) longest_path.cpp

permF: 	permute_file.cpp
	$(CXX) -g $(CFLAGS) -o permF.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) permute_file.cpp

BFS_try: BFS_try.cpp
	$(CXX) -g -fopenmp $(CFLAGS) -o BFST.out $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) $(BASIC_CHANGE_INC) $(BASIC_ANALYSIS_INC) BFS_try.cpp
test1: 	test_SetOps.cpp
	$(CXX) $(BASIC_IO_INC) $(BASIC_SETOP_INC) $(BASIC_TRAVERSAL_INC) test_SetOps.cpp



