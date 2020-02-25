


#include "systemc.h"
#include "dm.h"

SC_MODULE(TB) {
	sc_out<sc_uint<16> > addr;
	sc_out<sc_uint<16> > din;
	sc_out<sc_uint<1> > wr_en, rd_en;
	sc_in<sc_uint<16> > dout;

	int i;

	void prc() {
		i = 0;
		wr_en.write( sc_uint<1>(1) );
		rd_en.write( sc_uint<1>(0) );
		wait(3, SC_NS);
		cout << "Begin RF input test:"<<endl<<endl;
		while (i < 5) {
			addr.write( sc_uint<4>(i) );
			din.write( sc_uint<16>(i) );
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
			addr.write( sc_uint<4>(i) );
			i++;
			wait(3, SC_NS);
		}
	}

	SC_CTOR(TB) {
		SC_THREAD(prc);
	}
};


int sc_main(int argc, char* argv[]) { 
	sc_signal<sc_uint<16> > addr;
	sc_signal<sc_uint<16> > din;
	sc_signal<sc_uint<1> > wr_en, rd_en;
	sc_signal<sc_uint<16> > dout;

	DM dm("dm");
	dm << addr << din << wr_en << rd_en << dout;
	TB tb("tb");
	tb << addr << din << wr_en << rd_en << dout;
	
	sc_start(100, SC_NS);

	return 0;
}
