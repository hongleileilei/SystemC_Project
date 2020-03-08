#include "reg_fetch.h"
#include "systemc.h"

using namespace std;

void FETCH::proc(){
  pc_temp=pc_in.read();
  pc_out.write(pc_temp);
}
