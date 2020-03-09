
#include "rf.h"

void RF::proc(){
    if( rd_en.read()==sc_uint<1>(1) ){
      data1.write(mem[Raddr1.read()]);
      data2.write(mem[Raddr2.read()]);
      //cout << sc_time_stamp()<<data1.read()<<endl;
    }
    else if(wr_en.read()==sc_uint<1>(1) ){
      mem[Waddr.read()]=Wdata.read();
    }
}
