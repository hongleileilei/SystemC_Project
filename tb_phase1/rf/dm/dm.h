


#ifndef _DM_H
#define _DM_H
#include "systemc.h"



//DM finished
//DM is sensitive to write enable and read enable signal
//it has a hundred 16-bit memory storing data from STOR function
SC_MODULE(DM){
  //clock is needed?
  sc_in<sc_uint<16> > addr;//address input
  sc_in<sc_uint<16> > din;//data input
  sc_in<sc_uint<1> > wr_en, rd_en;// write_enable && read_enable
  //sc_in<bool> clk;//clock
  sc_out<sc_uint<16> > dout;// data out

  sc_uint<16> Data[100];
  void proc();
  SC_CTOR(DM){
    for(int i=0; i<100; i++){
      Data[i] == 0;
    }
    SC_METHOD(proc);
    sensitive<< wr_en << addr << din << rd_en;
  }
};

#endif
