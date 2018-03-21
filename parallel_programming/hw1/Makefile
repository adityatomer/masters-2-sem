CXX=g++

CXXFLAGS=-std=c++11

TARGET=inplace_matrixmul

inplace_matrixmul:inplace_matrixmul.o

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).o

clean:
	rm *.o inplace_matrixmul 
