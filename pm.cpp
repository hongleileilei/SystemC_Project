


#include "pm.h"

  void PM::proc(){
    temp_addr = addr.read();
    temp_data = Data[temp_addr];
    data.write(temp_data);
    Data[0] = 234;
    Data[1] = 112;
    Data[2] = 456;
    Data[3] = 1;
  }
