

#ifndef _RF_H
#define _RF_H
#include "systemc.h"


SC_MODULE(RF){
private:
  sc_uint<16>* pt;
  int N;
public:
  sc_in<bool> clk;
  sc_in<sc_uint<1> > wr_en, rd_en;
  sc_in<sc_uint<4> > Raddr1, Raddr2, Waddr;
  sc_in<sc_uint<16> > Wdata;
  sc_out<sc_uint<16> > data1, data2;

  sc_uint<16> mem[16];

  void proc_read();
  void proc_write();

  SC_HAS_PROCESS(RF);

  RF(sc_module_name nm, sc_uint<16>* coe, int n):sc_module(nm), pt(coe), N(n) {	
	SC_METHOD(proc_read);
	sensitive << clk.pos();

	SC_METHOD(proc_write);
	sensitive << clk.neg();

	for (int i = 0;i < N; i++) {
		mem[i] = pt[i];
	}
  }
};

#endif
