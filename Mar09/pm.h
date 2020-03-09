

#ifndef _PM_H
#define _PM_H
#include "systemc.h"

//PM finished
//PM is only sensitive to addr changes
//For inside, addr is for searching different instructions
//For outside, addr is consiered as PC, program counter
SC_MODULE(PM){
private:
  sc_uint<16> Data[100];
  sc_uint<16>* pt;
  int n;
public:
  sc_in<sc_uint<16> > addr;
  sc_out<sc_uint<16> > data;


  sc_uint<16> temp_data;
  sc_uint<16> temp_addr;
  void proc();

  SC_HAS_PROCESS(PM);
  PM(sc_module_name nm, sc_uint<16>* coe, int N):sc_module(nm), pt(coe), n(N) {
	SC_METHOD(proc);
	sensitive << addr;

	for(int i = 0; i < n; i++) {
		Data[i] = pt[i];
	}
  }




/*
  SC_CTOR(PM){
    for(int i =0; i<100; i++){
      Data[i]=0;
    }
    SC_METHOD(proc);
    sensitive<<addr;
  }*/
};


#endif
