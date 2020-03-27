src = $(wildcard *.cpp)
obj = $(src: .cpp=.o)

LDFLAGS =  -lsystemc -lm
SYSTEMC_HOME = /usr/local/systemc2.3.3

CFLAGS = -I. -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -Wl,-rpath=$(SYSTEMC_HOME)/lib-linux64

a: $(obj)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
clear:
	rm ./a
