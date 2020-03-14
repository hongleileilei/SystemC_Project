



#include "rf.h"

void RF::proc_read(){
	if( rd_en.read() == sc_uint<1>(1) ){
		data1.write(mem[Raddr1.read()]);
		data2.write(mem[Raddr2.read()]);
	}

	cout<<sc_time_stamp() << ", rf[2]: " << mem[2] << " rf[4]: " << mem[4] << " rf[6]: "<<mem[6]<<" rf[8]: " << mem[8] << ", rd: " << rd_en.read() << ", wr: " << wr_en << ", Waddr: " << Waddr.read() << ", Wdata: " << Wdata.read() << ", data1: " << data1.read() << ", data2: " << data2.read() << endl;

}

void RF::proc_write() {
	//cout<<sc_time_stamp()<<" Waddr: "<<Waddr.read()<<" Wdata: "<<Wdata.read()<<" wr enable: "<<wr_en.read()<<endl;
	if(wr_en.read() == sc_uint<1>(1) ){
		mem[Waddr.read()] = Wdata.read();
	}
}
