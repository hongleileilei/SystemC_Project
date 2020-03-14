
#include "xiaohong.h"
void CTRL_IN::proc(){
  instr_temp=instr_in.read();
  psr_temp=psr_in.read();
  psr_out.write(psr_temp);
  instr_out.write(instr_temp);
}
