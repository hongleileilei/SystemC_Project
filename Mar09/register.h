#include "systemc.h"

SC_MODULE(ALU_OUT) {
	sc_in<bool> clk;
	sc_in<sc_uint<16> > MMAR, MMDR, RRF_TEM;
	sc_out<sc_uint<16> > MAR, MDR, RF_TEM;
	sc_uint<16> mar, mdr, rf;                                                                                                                                             
	
	void prc();
	
	SC_CTOR(ALU_OUT) {
		SC_METHOD(prc);
		sensitive << clk.pos();
	}
};

SC_MODULE(ALU_IN) {
	sc_in<bool> clk;
	sc_in<sc_uint<16> > DDATA1, DDATA2;
	sc_in<sc_uint<8> > IIMM;
	sc_in<sc_uint<6> >  CCONTROL;
	sc_in<sc_uint<1> > D1, D2, D3, D4;
	
	sc_out<sc_uint<1> > d1, d2, d3, d4;
	sc_out<sc_uint<16> > DATA1, DATA2;
	sc_out<sc_uint<8> > IMM;
	sc_out<sc_uint<6> > CONTROL;
	
	sc_uint<16> data1, data2, imm;
	sc_uint<6> control;

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
	sc_out<sc_uint<16> > out;
	sc_signal<sc_uint<16> > temp, pc_delay;
	sc_signal<sc_uint<2> > c;


	void prc();

	SC_CTOR(WB) {
		SC_METHOD(prc);
		sensitive << clk.pos();
	}
};

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

