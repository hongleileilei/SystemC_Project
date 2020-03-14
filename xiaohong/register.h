#include "systemc.h"

SC_MODULE(ALU_OUT) {
	sc_in<bool> clk;
	sc_in<sc_uint<16> > MMAR, MMDR, RRF_TEM;
	sc_in<sc_uint<2> > wb_signal;   
	sc_in<sc_uint<4> > RW;  
	sc_in<sc_uint<1> > r_w;  
	sc_in<sc_uint<1> > d_r;	  
	sc_in<sc_uint<1> > d_w;

	sc_out<sc_uint<16> > MAR, MDR, RF_TEM;
	sc_out<sc_uint<2> > wb_sig;
	sc_out<sc_uint<4> > Rw;
	sc_out<sc_uint<1> > rf_w;
	sc_out<sc_uint<1> > dm_r;
	sc_out<sc_uint<1> > dm_w;

	sc_uint<16> mar, mdr, rf; 

	void prc();

	SC_CTOR(ALU_OUT) {
		SC_METHOD(prc);
		sensitive << clk.pos();
	}
};

SC_MODULE(ALU_IN) {
	sc_in<bool> clk;
	sc_in<sc_uint<8> > IIMM;
	sc_in<sc_uint<6> >  CCONTROL;
	sc_in<sc_uint<1> > D1, D2, D3, D4;
	sc_in<sc_uint<2> > wb_signal;
	sc_in<sc_uint<1> > r_w, d_w, d_r;
	sc_in<sc_uint<4> > Rw;
	
	sc_out<sc_uint<1> > d1, d2, d3, d4;
	sc_out<sc_uint<8> > IMM;
	sc_out<sc_uint<6> > CONTROL;
	sc_out<sc_uint<2> > wb_sig;
	sc_out<sc_uint<1> > rf_w, dm_w, dm_r;
	sc_out<sc_uint<4> > Rwrite;

	void prc();

	SC_CTOR(ALU_IN) {
		SC_METHOD(prc);
		sensitive << clk.pos();
	}
};

SC_MODULE(WB) {
	sc_in<bool> clk;
	sc_in<sc_uint<16> > ALU, PC, DM;
	sc_in<sc_uint<2> > WB_C;
	sc_in<sc_uint<4> > RW;
	//sc_in<sc_uint<1> > r_w;

	sc_out<sc_uint<16> > out;
	sc_out<sc_uint<4> > Rw;
	//sc_out<sc_uint<1> > rf_w;

	sc_uint<16> temp, pc_delay;
	sc_uint<2> c;

	void prc();

	SC_CTOR(WB) {
		SC_METHOD(prc);
		sensitive << clk.pos();
	}
};



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

