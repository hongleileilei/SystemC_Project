src = $(wildcard *.cpp)
obj = $(src: .cpp=.o)

LDFLAGS =  -lsystemc -lm
#SYSTEMC_HOME = /usr/local/systemc-2.3.0

CFLAGS = -I. -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -Wl,-rpath=$(SYSTEMC_HOME)/lib-linux64

myprog: $(obj)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
clear:
	rm -f ./myprog

