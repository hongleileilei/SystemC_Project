//1 does DM PM CTRL has clock sensitive
//2 JAL activity sequence
//3 pc 16bit, disp 8bit, how to deal with pc+disp



//ESE501Project1_Phase1
//AUTHOR
//Ke Ma.       SID: 112846615
//Honglei Liu. SID: 112848525

#include "systemc.h"
#include "pm.h"
#include "dm.h"
#include "rf.h"
#include "alu.h"
#include "dp.h"
#include "ctrl.h"
using namespace std;

//4 modules completion required
//DM, PM, RF, ALU
//2 upper level modules
//DP, CTRL




int sc_main(int argc, char* argv[]){
  sc_signal<sc_uint<16> > instr;
  sc_signal<sc_uint<4> > psr;
  sc_signal<sc_uint<16> > rtar;
  sc_signal<sc_uint<16> > pc;
  sc_signal<sc_uint<8> > imm;
  sc_signal<sc_uint<10> > ctrl;
  sc_signal<sc_uint<4> > rdsrc, rdest, r_w;


  DP dp("dp");
  dp << instr << psr << rtar << pc << imm << ctrl << rdsrc << rdest << r_w;

  CTRL ct("ct");
  ct << instr << psr << rtar << pc << imm << ctrl << rdsrc << rdest << r_w;


  sc_start(100, SC_NS);

  return(0);
}
