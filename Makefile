CXXFLAGS += -O2

jsMinifier: jsMinifier.o
	$(CXX) -O2 -o jsMinifier jsMinifier.o

