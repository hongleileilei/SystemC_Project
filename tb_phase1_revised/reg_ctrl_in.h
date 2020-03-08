#ifndef _REG_CTRL_IN_H
#define _REG_CTRL_IN_H
#include "systemc.h"

SC_MODULE(CTRL_IN){
  sc_in<sc_uint<16> > instr_in;
  sc_in<sc_uint<4> > psr_in;
  sc_in<bool> clock;
  sc_out<sc_uint<16> > instr_out;
  sc_out<sc_uint<4> > psr_out;

  sc_uint<16> instr_temp;
  sc_uint<4> psr_temp;
  void proc();
  SC_CTOR(CTRL_IN){
    SC_METHOD(proc);
    sensitive<<clock.pos();
  }
};
#endif
