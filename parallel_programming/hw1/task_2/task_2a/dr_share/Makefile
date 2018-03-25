CXX=icpc

CXXFLAGS=-std=c++11

TARGET=dr_share

dr_share:dr_share.o

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).o

clean:
	rm *.o dr_share 
