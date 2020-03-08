#include "systemc.h"
#include "reg_ctrl_in.h"
using namespace std;
void CTRL_IN::proc(){
  instr_temp=instr_in.read();
  psr_temp=psr_in.read();
  psr_out.write(psr_temp);
  instr_out.write(instr_temp);
}
