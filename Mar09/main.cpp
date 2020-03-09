

#include "systemc.h"
#include "ctrl.h"
#include "dp.h"


int sc_main(int argc, char* argv[]){

	sc_clock clk("clk", 10, SC_NS);
	sc_signal<sc_uint<16> > Instr;
	sc_signal<sc_uint<4> > psr;//ZNCF
	sc_signal<sc_uint<16> > Rtar_val;//////////////////////// Rtar
	sc_signal<sc_uint<16> > pc;
	sc_signal<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
	/////////////////////////////
	sc_signal<sc_uint<6> > alu_ctrl;
	sc_signal<sc_uint<1> > m4;
	sc_signal<sc_uint<1> > m3;
	sc_signal<sc_uint<1> > m2;
	sc_signal<sc_uint<1> > m1;
	sc_signal<sc_uint<1> > rf_wr;
	sc_signal<sc_uint<1> > rf_rd;
	sc_signal<sc_uint<1> > dm_wr;
	sc_signal<sc_uint<1> > dm_rd;
	sc_signal<sc_uint<4> > R_src, R_dest;
	sc_signal<sc_uint<4> > R_w;
	sc_signal<sc_uint<2> > WB_C;

	

	CTRL ctrl("ctrl");
	ctrl << Instr << psr << Rtar_val << pc << Imm << alu_ctrl << m4 << m3 << m2 << m1 << rf_wr << rf_rd << dm_wr << dm_rd << R_src << R_dest << R_w;
	
	DP dp("dp");	
	dp << clk << Instr << psr << Rtar_val << pc << Imm << alu_ctrl << R_src << R_dest << R_w << m1 << m2 << m3 << m4 << dm_rd << dm_wr << rf_rd << rf_wr << WB_C; 
	
	sc_start(100, SC_NS);
	
	return 0;
}
