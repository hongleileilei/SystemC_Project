

#ifndef _PM_H
#define _PM_H
#include "systemc.h"

//PM finished
//PM is only sensitive to addr changes
//For inside, addr is for searching different instructions
//For outside, addr is consiered as PC, program counter
SC_MODULE(PM){
  sc_in<sc_uint<16> > addr;
  sc_out<sc_uint<16> > data;

  sc_uint<16> Data[100];
  sc_uint<16> temp_data;
  sc_uint<16> temp_addr;
  void proc();
  SC_CTOR(PM){
    //Initialization
    for(int i =0; i<50; i++){
      Data[i]=19588;
    }
    for(int i =50; i<100; i++){
      Data[i]=22655;
    }
    Data[0]=22655;
    SC_METHOD(proc);
    sensitive<<addr;
  }
};


#endif
