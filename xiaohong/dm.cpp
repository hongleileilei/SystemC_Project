


#include "dm.h"


void DM:: proc(){
	if(wr_en.read() == 1){
		Data[addr.read()]=din.read();
	}
	if(rd_en.read() == 1){
		out = Data[addr.read()];
	}

}


void DM:: pipeline() {
	//r_w.write(rf_w.read());
	//rw_signal = RW.read();
	//rw.write(rw_signal.read());
	//cout<<sc_time_stamp()<<" result in dm passing by is: "<<rw.read()<<endl;
		
	if (WB_C.read() == (sc_uint<2>)0) {		
		dout.write(ALU_in.read());
		cout<<sc_time_stamp()<<", ALU_in: "<<ALU_in.read()<<endl;
	}
	else if (WB_C.read() == (sc_uint<2>)1) {
		dout.write(out);		
	}
	r_w.write(rf_w.read());
	rw_signal = RW.read();
	rw.write(rw_signal.read());


	//cout << sc_time_stamp() << ",DM: dout: " << dout.read() << ", out: " << out << ", ALU_in: " << ALU_in.read() << ", addr: " << addr.read() << ", WB_C: " << WB_C.read() << endl;
	//cout << sc_time_stamp() << ", dm: rw_in:" << RW.read() << ", rw_signal: " << rw_signal.read() << ", rw_out: " << rw.read() << endl; 
}
