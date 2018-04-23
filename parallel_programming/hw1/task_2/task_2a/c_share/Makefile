CXX=g++

CXXFLAGS=-std=c++11

TARGET=c_share

c_share:c_share.o

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).o

clean:
	rm *.o c_share 
