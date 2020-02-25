


#include "systemc.h"
#include "rf.h"

SC_MODULE(TB) {
	sc_out<sc_uint<1> > wr_en, rd_en;
	sc_out<sc_uint<4> > Raddr1, Raddr2, Waddr;
	sc_out<sc_uint<16> > Wdata;
	sc_in<sc_uint<16> > data1, data2;

	sc_uint<4> addr1, addr2, waddr;	
	sc_uint<16> wdata;

	int i;

	void prc() {
		i = 0;
		wr_en.write( sc_uint<1>(1) );
		rd_en.write( sc_uint<1>(0) );
		wait(3, SC_NS);
		cout << "Begin RF input test:"<<endl<<endl;
		while (i < 5) {
			waddr = sc_uint<4>(i);
			wdata = sc_uint<16>(i);
			Waddr.write(waddr);
			Wdata.write(wdata);
			i++;
			wait(3, SC_NS);
		}
		
		wait(3, SC_NS);
		i = 0;	 
		wr_en.write( sc_uint<1>(0) );
		wait(3, SC_NS);

		rd_en.write( sc_uint<1>(1) );
		cout<<"Input done. Begin output test:"<<endl<<endl;
		cout << "///////////////////////////" <<endl;
		
		while(i < 5) {
			Raddr1.write( sc_uint<4>(i) );
			i++;
			wait(3, SC_NS);
		}
	}

	SC_CTOR(TB) {
		SC_THREAD(prc);
		sensitive << wr_en << rd_en;
	}
};



int sc_main(int argc, char* argv[]) {
	sc_signal<sc_uint<1> > wr, rd;
	sc_signal<sc_uint<4> > Raddr1, Raddr2, Waddr;
	sc_signal<sc_uint<16> > Wdata;
	sc_signal<sc_uint<16> > data1, data2;

	RF rf("rf");
	rf << wr << rd << Raddr1 << Raddr2 << Waddr << Wdata << data1 << data2;
	TB tb("tb");
	tb << wr << rd << Raddr1 << Raddr2 << Waddr << Wdata << data1 << data2;

	sc_trace_file *tf = sc_create_vcd_trace_file("myvcdfile");

	sc_trace(tf, rf.wr_en, "rf.wr_en");
	sc_trace(tf, rf.rd_en, "rf.rd_en");
	
	sc_trace(tf, rf.Waddr, "rf.Waddr");
	sc_trace(tf, rf.Wdata, "rf.Wdata");
	sc_trace(tf, rf.Raddr1, "rf.Raddr1");
	sc_trace(tf, rf.data1, "rf.data1");
	sc_trace(tf, rf.data2, "rf.data2");

	sc_start(100, SC_NS);

	sc_close_vcd_trace_file(tf);

	return 0;
}
