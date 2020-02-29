



#ifndef _ALU_H
#define _ALU_H
#include "systemc.h"

//ALU finished
//ALU should be sensitive to clock, for highest priority, if clock enabled
//ALU is also controlled by control signals
//Error exists: Immediate value signed or unsigned
SC_MODULE(ALU){
  //////////////////////////////////////
  //PORTS
  //////////////////////////////////////
  //sc_in<bool> clk;//clock
  sc_in<sc_uint<16> > DATA1, DATA2;//as indicated below
  sc_in<sc_uint<8> > IMM;//immediate value
  sc_in<sc_uint<6> > CONTROL;//control signal passing from DP, 6-bit
  sc_in<sc_uint<1> > D1, D2, D3, D4;
  sc_in<sc_uint<16> > DM_DATAOUT,PC;
  sc_out<sc_uint<4> > PSR;//PORTS SEQUENCE: cnfz
  sc_out<sc_uint<16> > MAR, MDR, RF;//computation result
  //data1:Rdest data2:Rsrc,Ramt,Raddr
  
  sc_uint<16> dm_dataout, pc;
  sc_uint<16> data1;
  sc_uint<16> data2;
  sc_uint<16> result;

  sc_uint<1> d1, d2, d3, d4;
  sc_uint<16> d1_out, d2_out, d3_out, d4_out;

  sc_uint<8> im;
  sc_uint<6> control;
  sc_uint<4> psr;//private psr value
  sc_uint<16> imm, imm_ori;
  int i;

  sc_uint<16> data2inv, imminv;//inverse-bit sequence
  int lendata1, lendata2, lenresult, lenimm, lenresult_cmp;//as indicated
  sc_uint<17> mmax;//used to determine overflow, only one bit larger than 16-bit maximum
  sc_uint<16> result_cmp;//compare operation result
  sc_uint<16> right1;
  sc_uint<4> right2;
  sc_uint<16> right3;
  sc_uint<4> right4;
  sc_uint<1> left_most;

  void proc();
  SC_CTOR(ALU){
    SC_METHOD(proc);
    //sensitive<<clk.pos();
  }
};

#endif
