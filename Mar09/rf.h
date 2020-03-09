

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

  void proc();


  SC_HAS_PROCESS(RF);

  RF(sc_module_name nm, sc_uint<16>* coe, int n):sc_module(nm), pt(coe), N(n) {	
	SC_METHOD(proc);
	sensitive << clk.pos();

	for (int i = 0;i < N; i++) {
		mem[i] = pt[i];
	}
  }


/*
  SC_CTOR(RF){
    for(int i=0;i<16;i++){
      mem[i]=0;
    }
    SC_METHOD(proc);
    sensitive << clk.pos();
  }*/
};

#endif
