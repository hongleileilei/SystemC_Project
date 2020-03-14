

#include "systemc.h"
#include "ctrl.h"
#include "dp.h"


int sc_main(int argc, char* argv[]){

	sc_clock clk("clk", 10, SC_NS, 0.5, 10, SC_NS, true);
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
	sc_signal<sc_uint<1> > pc_alu_mux;
	sc_signal<sc_uint<1> > mdr_mux;
	sc_signal<sc_uint<1> > mar_mux;


	CTRL ctrl("ctrl");
	ctrl << clk << Instr << psr << Rtar_val << pc << Imm << alu_ctrl << m4 << m3 << m2 << m1 << rf_wr << rf_rd << dm_wr << dm_rd << R_src << R_dest << R_w << WB_C << pc_alu_mux << mdr_mux << mar_mux;
	
	DP dp("dp");	
	dp << clk << Instr << psr << Rtar_val << pc << Imm << alu_ctrl << R_src << R_dest << R_w << m1 << m2 << m3 << m4 << dm_rd << dm_wr << rf_rd << rf_wr << WB_C << pc_alu_mux << mdr_mux << mar_mux; 



	//sc_trace_file *file = sc_create_vcd_trace_file("aaa");

//	sc_trace(file, clk, "clk");
//	sc_trace(file, ctrl.Instr, "ctrl.Instr");
//	sc_trace(file, ctrl.temp_instr, "ctrl.tem_instr");
//	sc_trace(file, ctrl.counter_instr, "ctrl.counter_instr");

	
	sc_start(200, SC_NS);
//	
//	sc_close_vcd_trace_file(file);	
	return 0;
}



	

/*
	sc_trace(file, dp.pm->data, "dp.pm.pm_data");

	sc_trace(file, ctrl.pc, "ctrl.pc");
	sc_trace(file, ctrl.counter_instr, "ctrl.ctrl_counter_instr");
	sc_trace(file, ctrl.ctrl_in->instr_out, "ctrl.ctrl_in.instr_out");
	sc_trace(file, ctrl.lcl_instr, "ctrl.ctrl_lcl_instr");
	sc_trace(file, ctrl.rf_rd, "ctrl.ctrl_rf_rd");


	sc_trace(file, dp.rf->Raddr1, "dp.rf.rf_Raddr1");
	sc_trace(file, dp.rf->Raddr2, "dp.rf.rf_Raddr2");
	sc_trace(file, dp.rf->data1, "dp.rf.rf_data1");
	sc_trace(file, dp.rf->data2, "dp.rf.rf_data2");
	sc_trace(file, dp.rf->rd_en, "dp.rf.rf_rd");
	sc_trace(file, dp.rf->wr_en, "dp.rf.rf_wr");
	sc_trace(file, dp.rf->mem[2], "dp.rf.rf_mem");
	sc_trace(file, dp.rf->Waddr, "dp.rf.rf_Waddr");
	sc_trace(file, dp.rf->Wdata, "dp.rf.rf_Wdata");


	sc_trace(file, dp.alu_in->wb_signal, "dp.alu_in.aluin_wb_signal");
	sc_trace(file, dp.alu_in->wb_sig, "dp.alu_in.aluin_wb_sig");

	sc_trace(file, dp.alu->d3_out, "dp.alu.alu_d3_out");
	sc_trace(file, dp.alu->d2_out, "dp.alu.alu_d2_out");
	sc_trace(file, dp.alu->DATA2, "dp.alu.alu_DATA2");
	sc_trace(file, dp.alu->DATA1, "dp.alu.alu_DATA1");
	sc_trace(file, dp.alu->data1, "dp.alu.alu_data1");
	sc_trace(file, dp.alu->data2, "dp.alu.alu_data2");
	sc_trace(file, dp.alu->d1, "dp.alu.alu_d1");
	sc_trace(file, dp.alu->d2, "dp.alu.alu_d2");
	sc_trace(file, dp.alu->d3, "dp.alu.alu_d3");
	sc_trace(file, dp.alu->d4, "dp.alu.alu_d4");
	sc_trace(file, dp.WB_C, "dp.WB_C");



	sc_trace(file, dp.alu_out->MAR, "dp.alu_out.MAR");
	sc_trace(file, dp.alu_out->MDR, "dp.alu_out.MDR");
	sc_trace(file, dp.alu_out->RF_TEM, "dp.alu_out.RF_TEM");
	sc_trace(file, dp.alu_out->wb_signal, "dp.alu_out.aluout_wb_signal");



	sc_trace(file, dp.dm->dout, "dp.dm.dm_dout");
	sc_trace(file, dp.dm->addr, "dp.dm.dm_addr");
	sc_trace(file, dp.dm->wr_en, "dp.dm.dm_wr");
	sc_trace(file, dp.dm->rd_en, "dp.dm.dm_rd");

	sc_trace(file, dp.wb->ALU, "dp.wb.wb_ALU");
	sc_trace(file, dp.wb->PC, "dp.wb.wb_PC");
	sc_trace(file, dp.wb->DM, "dp.wb.wb_DM");
	sc_trace(file, dp.wb->WB_C, "dp.wb.wb_WB_C");
	sc_trace(file, dp.wb->Rw, "dp.wb.wb_Rw");
	sc_trace(file, dp.wb->out, "dp.wb.wb_out");
*/
