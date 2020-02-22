#include "systemc.h"
//4 modules completion required
//DM, PM, RF, ALU
//2 upper level modules
//DP, CTRL
SC_MODULE(DM){
  SC_CTOR(DM){
  }
};

SC_MODULE(PM){
  SC_CTOR(PM){
  }
};

SC_MODULE(RF){
  SC_CTOR(RF){
  }
};

SC_MODULE(ALU){
  SC_CTOR(ALU){
  }
};

SC_MODULE(DP){
  sc_out<sc_uint<16> > Instr;
  sc_out<sc_uint<4> > psr_val;
  sc_out<sc_uint<16> > Rtar;
  sc_in<sc_uint<8> > pc;
  sc_in<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
  sc_in<sc_uint<16> > ctrl;
  sc_in<sc_uint<4> > Rsrc, Rdest, Ramt;

  void proc();
  SC_CTOR(DP){
    //need to connect DP with the four module above
  }
};

SC_MODULE(CTRL){
    sc_in<sc_uint<16> > Instr;
    sc_in<sc_uint<4> > psr_val;
    sc_in<sc_uint<16> > Rtar;
    sc_out<sc_uint<8> > pc;
    sc_out<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
    sc_out<sc_uint<16> > ctrl;
    sc_out<sc_uint<4> > Rsrc, Rdest, Ramt;
    
    //displacement is defined locally
    sc_uint<8> Disp;
    sc_uint<4> cond_bit;

    //local method
    void proc();
    //constructor
    SC_CTOR(CTRL){
    }
};

int sc_main(int argc, char* argv[]){
  return(0);
}
