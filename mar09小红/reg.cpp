

#include "systemc.h"
#include "reg.h"

void ALU_OUT::prc() {
	mar = MMAR.read();
	mdr = MMDR.read();
	rf = RRF_TEM.read();
	wb_sig.write(wb_signal.read());
	MAR.write(mar);
	MDR.write(mdr);
	RF_TEM.write(rf);
}


void ALU_IN::prc() {
	//DATA1.write(DDATA1.read());
	//DATA2.write(DDATA2.read());
	IMM.write(IIMM.read());
	CONTROL.write(CCONTROL.read());
	d1.write(D1.read());
	d2.write(D2.read());
	d3.write(D3.read());
	d4.write(D4.read());
	wb_sig.write(wb_signal.read());
	Rsrc.write(Rs.read());
	Rdest.write(Rd.read());
	Rwrite.write(Rw.read());
	rf_w.write(r_w.read());
	rf_r.write(r_r.read());
	dm_w.write(d_w.read());
	dm_r.write(d_r.read());
}



void WB::prc() {
  c = WB_C.read();
  if (c == (sc_uint<2>)0) {
    temp = ALU.read();
    out.write(temp);
  }
  else if (c == (sc_uint<2>)2) {
    temp = PC.read();
    pc_delay = temp;
    out.write(pc_delay);
  }
  else if(c == (sc_uint<2>)1) {
    temp = DM.read();
    out.write(temp);
  }

}


void CTRL_IN::proc(){
  instr_temp=instr_in.read();
  psr_temp=psr_in.read();
  psr_out.write(psr_temp);
  instr_out.write(instr_temp);
}


void FETCH::proc(){
  pc_temp=pc_in.read();
  pc_out.write(pc_temp);
}
