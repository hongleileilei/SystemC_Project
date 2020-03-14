

#include "systemc.h"
#include "register.h"

void ALU_OUT::prc() {
	mar = MMAR.read();
	mdr = MMDR.read();
	rf = RRF_TEM.read();
	wb_sig.write(wb_signal.read());
	MAR.write(mar);
	MDR.write(mdr);
	RF_TEM.write(rf);
	Rw.write(RW.read());
	rf_w.write(r_w.read());
	dm_w.write(d_w.read());
	dm_r.write(d_r.read());
}


void ALU_IN::prc() {
	IMM.write(IIMM.read());
	CONTROL.write(CCONTROL.read());
	d1.write(D1.read());
	d2.write(D2.read());
	d3.write(D3.read());
	d4.write(D4.read());
	wb_sig.write(wb_signal.read());

	Rwrite.write(Rw.read());
	rf_w.write(r_w.read());
	dm_w.write(d_w.read());
	dm_r.write(d_r.read());

}



void WB::prc() {
  c = WB_C.read();

  if (c == (sc_uint<2>)0) {
    temp = ALU.read();
    out.write(temp);
  }
  else if(c == (sc_uint<2>)1) {
    temp = DM.read();
    out.write(temp);
  }
  else if (c == (sc_uint<2>)2) {
    temp = PC.read();
    pc_delay = temp;
    out.write(pc_delay);
  }

  Rw.write(RW.read());
  //rf_w.write(r_w.read());	
}





void FETCH::proc(){
  pc_temp=pc_in.read();
  pc_out.write(pc_temp);
}

