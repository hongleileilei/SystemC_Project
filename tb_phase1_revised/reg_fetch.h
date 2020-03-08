#ifndef _REG_FETCH_H
#define _REG_FETCH_H
#include "systemc.h"

SC_MODULE(FETCH){
  sc_in<sc_uint<16> > pc_in;
  sc_out<sc_uint<16> > pc_out;
  sc_in<bool> clock;
  sc_uint<16> pc_temp;
  void proc();

  SC_CTOR(FETCH){
    SC_METHOD(proc);
    sensitive<<clock.pos();
  }
};

#endif
