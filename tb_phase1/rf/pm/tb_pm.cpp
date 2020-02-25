#include "systemc.h"
#include "pm.h"

SC_MODULE(TB_PM){
  sc_out<sc_uint<16> > Addr;
  sc_in<sc_uint<16> > Data;

  sc_uint<16> temp_addr;
  int i;

  void proc(){
    cout<<"testbench of program memory"<<endl;
    i=0;


    while(i<4){
      temp_addr=sc_uint<16>(i);
      wait(3, SC_NS);
      Addr.write(temp_addr);
      cout << sc_time_stamp() << " asddd: " << Addr.read() <<endl;
      wait(3, SC_NS);
      cout<<temp_addr<<sc_time_stamp()<<" with value ";
      cout<<Data.read()<<"with addr "<< Addr.read() << endl;
      i++;
      wait(3,SC_NS);
    }
  }

  SC_CTOR(TB_PM){
    SC_THREAD(proc);
    sensitive << Addr << Data;
  }
};

int sc_main(int argc, char* argv[]){
  sc_signal<sc_uint<16> > addr;
  sc_signal<sc_uint<16> > data;

  PM pm("pm");
  pm<< addr << data;
  TB_PM tb_pm("tb_pm");
  tb_pm<< addr <<data;

  sc_trace_file *tf = sc_create_vcd_trace_file("tb_pm");

  sc_trace(tf,pm.addr,"pm_addr");
  sc_trace(tf,pm.data,"pm_data");

  sc_start(100, SC_NS);

  sc_close_vcd_trace_file(tf);

  return 0;
}
