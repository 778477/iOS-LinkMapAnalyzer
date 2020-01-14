CXX = clang++
CXXFLAGS = -target x86_64-apple-macos10.10 -std=c++11
OBJECTS = LinkMapAnalyzer.o main.o

LinkMapAnalyzer : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

LinkMapAnalyzer.o : LinkMapAnalyzer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm $(OBJECTS) LinkMapAnalyzer

format:
	find . -name "*.cpp" | clang-format -style=google $0