


#include "dm.h"


void DM:: proc(){
    if(wr_en.read() == 1){//write enable
        Data[addr.read()]=din.read();
    }
    else if(rd_en.read() == 1){//read enable
        dout.write(Data[addr.read()]);
    }
}

