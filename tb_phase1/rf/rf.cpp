



#include "rf.h"

  void RF::proc(){
	cout <<"ok asdfsafssfsafdsasfdsafd"<<endl;
    if( rd_en.read()==sc_uint<1>(1) ){
      cout << " 1 asdfsadfsafd"<<endl;
      data1.write(mem[Raddr1.read()]);
      data2.write(mem[Raddr2.read()]);
    }
    else if( wr_en.read()==(sc_uint<1>)1){
      cout << " 2 asdfdsasdfsadf" <<endl;
      mem[Waddr.read()]=Wdata.read();
    }
  }
