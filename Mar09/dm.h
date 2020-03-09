
#ifndef _DM_H
#define _DM_H
#include "systemc.h"


SC_MODULE(DM){
private:
  sc_uint<16>* pt;
  int N;
public:
  sc_in<sc_uint<16> > addr;
  sc_in<sc_uint<16> > din;
  sc_in<sc_uint<1> > wr_en, rd_en;
  sc_out<sc_uint<16> > dout;

  sc_uint<16> Data[100];

  void proc();

  SC_HAS_PROCESS(DM);
  DM(sc_module_name dm, sc_uint<16>* coe, int n):sc_module(dm), pt(coe), N(n) {
	SC_METHOD(proc);
	sensitive << wr_en << rd_en;
	for (int i = 0; i < sizeof(Data)/sizeof(Data[0]); i++) {
		Data[i] = 0;
	}

	for (int i = 0; i < N; i++) {
		Data[i] = pt[i];
	}
  }
};



#endif
