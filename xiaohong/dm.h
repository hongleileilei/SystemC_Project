
#ifndef _DM_H
#define _DM_H
#include "systemc.h"


SC_MODULE(DM){
private:
  sc_uint<16>* pt;
  int N;
public:
  sc_in<bool> clk;
  sc_in<sc_uint<16> > addr;
  sc_in<sc_uint<16> > din;
  sc_in<sc_uint<1> > wr_en, rd_en;

  sc_in<sc_uint<16> > ALU_in;
  sc_in<sc_uint<2> > WB_C;
  sc_in<sc_uint<1> > rf_w;
  sc_in<sc_uint<4> > RW;  

  sc_out<sc_uint<4> > rw; 
  sc_out<sc_uint<1> > r_w;
  sc_out<sc_uint<16> > dout;


  sc_signal<sc_uint<4> > rw_signal;

  sc_uint<16> Data[100];
  sc_uint<16> out;

  void proc();
  void pipeline();


  SC_HAS_PROCESS(DM);
  DM(sc_module_name dm, sc_uint<16>* coe, int n):sc_module(dm), pt(coe), N(n) {
	SC_METHOD(proc);
	sensitive << wr_en << rd_en << addr << din << ALU_in << WB_C << RW << rf_w;
	SC_METHOD(pipeline);
	sensitive << clk.pos();

	for (int i = 0; i < sizeof(Data)/sizeof(Data[0]); i++) {
		Data[i] = 0;
	}

	for (int i = 0; i < N; i++) {
		Data[i] = pt[i];
	}
  }
};



#endif
