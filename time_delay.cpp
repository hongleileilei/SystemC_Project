#include "systemc.h"

SC_MODULE(A) {
	sc_in<bool> clock;
	sc_in<bool> data;
	sc_out<bool> seq;

	sc_signal<bool> first, second, third;

	void prc_1(){
		first.write( data.read() );
		second.write( first.read() );
		third.write( second.read() );	

		
		cout << sc_time_stamp() << " " << data.read() << " " << seq.read() << endl;		
		
	}
	void prc_2() {
		seq = first & (!second) & third;
	}


	SC_CTOR(A) {
		SC_METHOD(prc_1);
		sensitive << clock.pos();
		SC_METHOD(prc_2);
		sensitive << first << second << third;
	}
};

SC_MODULE(B) {
	sc_in<bool> clk;
	sc_out<bool> out;
	sc_signal<bool> temp;

	void hanshu() {
		while(1) {	
			wait();
			temp = 1;
			out.write(temp);
			wait();
			temp = 0;
			out.write(temp);
		}
	}

	SC_CTOR(B) {
		SC_THREAD(hanshu);
		sensitive << clk.pos();
	}
};

int sc_main(int argc, char * argv[]) {
	sc_clock clk("clk", 10, SC_NS);
	sc_signal<bool> data, seq;

	A a("a");
	a << clk << data << seq;
	
	B b("b");
	b << clk << data;

	sc_trace_file *tfile = sc_create_vcd_trace_file("file");
	
	sc_trace(tfile, a.data, "a.data");
	sc_trace(tfile, a.first, "a.first");
	sc_trace(tfile, a.second, "a.second");
	sc_trace(tfile, a.third, "a.third");
	sc_trace(tfile, seq, "seq");
	sc_trace(tfile, clk, "clk");
	sc_trace(tfile, b.out, "b.out");
	sc_trace(tfile, b.temp, "b.temp");

	sc_start(100, SC_NS);
	sc_close_vcd_trace_file(tfile);


	return 0;
}
