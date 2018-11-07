# complier type
CXX = g++
# flag for showing all warnings, debugging information, support c++11
CXXFLAGS = -Wall -g -std=c++0x

BaseNodeTest: BaseNodeTest.o N.o N4.o N16.o N48.o N256.o
	$(CXX) $(CXXFLAGS) -o BaseNodeTest BaseNodeTest.o N.o N4.o N16.o N48.o N256.o

BaseNodeTest.o: BaseNodeTest.cpp N.h
	$(CXX) $(CXXFLAGS) -c BaseNodeTest.cpp

N.o: N.cpp N.h
	$(CXX) $(CXXFLAGS) -c N.cpp

N4.o: N4.cpp N.h
	$(CXX) $(CXXFLAGS) -c N4.cpp

N16.o: N16.cpp N.h
	$(CXX) $(CXXFLAGS) -c N16.cpp

N48.o: N48.cpp N.h
	$(CXX) $(CXXFLAGS) -c N48.cpp

N256.o: N256.cpp N.h
	$(CXX) $(CXXFLAGS) -c N256.cpp

Tree.o: Tree.cpp Tree.h N.h
	$(CXX) $(CXXFLAGS) -c Tree.cpp

clean:
	rm -rf *.o
