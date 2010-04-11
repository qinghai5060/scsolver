PRJ=../..

SOURCE_PATH=$(PRJ)/source
NUMERIC_PATH=$(SOURCE_PATH)/numeric

CPPFLAGS=-I$(PRJ)/source/inc -DSCSOLVER_UNITTEST
CXXFLAGS=-Wall -O0 -g

TESTFILES = \
	matrix \
	polyeqnsolver \
	quadfitlinesearch \
	bisectionsearch \
	diff \
	cycliccoordinate \
	funcobj \
	hookejeeves \
	quasinewton \
	rosenbrock \
	penalty

build: $(TESTFILES)

global.o: $(SOURCE_PATH)/tool/global.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

timer.o: $(SOURCE_PATH)/tool/timer.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

matrix.o: $(NUMERIC_PATH)/matrix.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

baselinesearch.o: $(NUMERIC_PATH)/baselinesearch.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

OBJFILES_FUNCOBJ = \
	funcobj.o \
	funcobj_test.o \
	exception.o

funcobj.o: $(NUMERIC_PATH)/funcobj.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

funcobj_test.o: $(NUMERIC_PATH)/funcobj_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

funcobj: $(OBJFILES_FUNCOBJ)
	$(CXX) -o $@ $(OBJFILES_FUNCOBJ)

matrix: matrix.o matrix_test.o global.o
	$(CXX) -o $@ matrix.o matrix_test.o global.o

matrix_test.o: $(NUMERIC_PATH)/matrix_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

polyeqnsolver.o: $(NUMERIC_PATH)/polyeqnsolver.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

polyeqnsolver_test.o: $(NUMERIC_PATH)/polyeqnsolver_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

polyeqnsolver: polyeqnsolver.o polyeqnsolver_test.o matrix.o global.o
	$(CXX) -o $@ polyeqnsolver.o polyeqnsolver_test.o matrix.o global.o

quadfitlinesearch.o: $(NUMERIC_PATH)/quadfitlinesearch.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

quadfitlinesearch_test.o: $(NUMERIC_PATH)/quadfitlinesearch_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

OBJFILES_QUADFITLINESEARCH = \
	baselinesearch.o \
	quadfitlinesearch.o \
	quadfitlinesearch_test.o \
	polyeqnsolver.o \
	matrix.o \
	global.o \
	timer.o \
	funcobj.o \
	diff.o \
	nlpmodel.o \
	nlpbase.o \
	exception.o

quadfitlinesearch: $(OBJFILES_QUADFITLINESEARCH)
	$(CXX) -o $@ $(OBJFILES_QUADFITLINESEARCH)

OBJFILES_DIFF = \
	diff_test.o \
	diff.o \
	funcobj.o \
	exception.o \
	timer.o

diff.o: $(NUMERIC_PATH)/diff.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

diff_test.o: $(NUMERIC_PATH)/diff_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

diff: $(OBJFILES_DIFF)
	$(CXX) -o $@ $(OBJFILES_DIFF)

exception.o: $(NUMERIC_PATH)/exception.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

nlpbase.o: $(NUMERIC_PATH)/nlpbase.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

nlpmodel.o: $(NUMERIC_PATH)/nlpmodel.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

OBJFILES_CYCLICCOORDINATE = \
	cycliccoordinate.o \
	cycliccoordinate_test.o \
	nlpbase.o \
	nlpmodel.o \
	exception.o \
	funcobj.o \
	quadfitlinesearch.o \
	baselinesearch.o \
	diff.o \
	timer.o \
	polyeqnsolver.o \
	matrix.o \
	global.o

cycliccoordinate.o: $(NUMERIC_PATH)/cycliccoordinate.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

cycliccoordinate_test.o: $(NUMERIC_PATH)/cycliccoordinate_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

cycliccoordinate: $(OBJFILES_CYCLICCOORDINATE)
	$(CXX) -o $@ $(OBJFILES_CYCLICCOORDINATE)

OBJFILES_HOOKEJEEVES = \
	hookejeeves.o \
	hookejeeves_test.o \
	nlpbase.o \
	nlpmodel.o \
	exception.o \
	funcobj.o \
	baselinesearch.o \
	quadfitlinesearch.o \
	diff.o \
	timer.o \
	polyeqnsolver.o \
	matrix.o \
	global.o

hookejeeves_test.o: $(NUMERIC_PATH)/hookejeeves_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

hookejeeves.o: $(NUMERIC_PATH)/hookejeeves.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

hookejeeves: $(OBJFILES_HOOKEJEEVES)
	$(CXX) -o $@ $(OBJFILES_HOOKEJEEVES)

OBJFILES_QNEWTON = \
	quasinewton_test.o \
	quasinewton.o \
	matrix.o \
	nlpbase.o \
	nlpmodel.o \
	exception.o \
	funcobj.o \
	quadfitlinesearch.o \
	polyeqnsolver.o \
	baselinesearch.o \
	diff.o \
	timer.o \
	global.o

quasinewton_test.o: $(NUMERIC_PATH)/quasinewton_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

quasinewton.o: $(NUMERIC_PATH)/quasinewton.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

quasinewton: $(OBJFILES_QNEWTON)
	$(CXX) -o $@ $(OBJFILES_QNEWTON)

OBJFILES_BISECTIONSEARCH = \
	bisectionsearch.o \
	bisectionsearch_test.o \
	baselinesearch.o \
	funcobj.o \
	nlpmodel.o \
	nlpbase.o \
	exception.o \
	diff.o \
	timer.o

bisectionsearch_test.o: $(NUMERIC_PATH)/bisectionsearch_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

bisectionsearch.o: $(NUMERIC_PATH)/bisectionsearch.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

bisectionsearch: $(OBJFILES_BISECTIONSEARCH)
	$(CXX) -o $@ $(OBJFILES_BISECTIONSEARCH)

OBJFILES_ROSENBROCK = \
	rosenbrock_test.o \
	rosenbrock.o \
	nlpbase.o \
	nlpmodel.o \
	exception.o \
	funcobj.o \
	baselinesearch.o \
	quadfitlinesearch.o \
	diff.o \
	timer.o \
	polyeqnsolver.o \
	matrix.o \
	global.o

rosenbrock_test.o: $(NUMERIC_PATH)/rosenbrock_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

rosenbrock.o: $(NUMERIC_PATH)/rosenbrock.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

rosenbrock: $(OBJFILES_ROSENBROCK)
	$(CXX) -o $@ $(OBJFILES_ROSENBROCK)

OBJFILES_PENALTY = \
	penalty_test.o \
	penalty.o \
	nlpbase.o \
	nlpmodel.o \
	funcobj.o \
	exception.o \
	baselinesearch.o

penalty_test.o: $(NUMERIC_PATH)/penalty_test.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

penalty.o: $(NUMERIC_PATH)/penalty.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

penalty: $(OBJFILES_PENALTY)
	$(CXX) -o $@ $(OBJFILES_PENALTY)

clean:
	rm -f *.o $(TESTFILES)

