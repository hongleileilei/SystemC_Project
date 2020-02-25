


#ifndef _DP_H
#define _DP_H
#include "systemc.h"

SC_MODULE(DP){
  /////////////////////////////////
  //PORTS
  /////////////////////////////////
  //Instr: From PM to DP to CTRL
  //psr_val: From ALU to DP to CTRL
  //Rtar_val: From RF to DP to CTRL
  //pc: From CTRL to DP to PM (finding specific instruction)
  //Imm: From CTRL to DP to ALU
  //ctrl: From CTRL to DP, control separator of DP Register Controller
  //Rsrc, Rdest: From CTRL to Register Controller and then to RF
  sc_out<sc_uint<16> > Instr;
  sc_out<sc_uint<4> > PSR;
  sc_out<sc_uint<16> > RTAR;
  sc_in<sc_uint<16> > PC;
  sc_in<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
  sc_in<sc_uint<10> > CTRL;
  sc_in<sc_uint<4> > RSRC, RDEST, R_W;

  sc_signal<sc_uint<1> > dm_wr,dm_rd,rf_wr,rf_rd;
  sc_signal<sc_uint<4> > rf_Raddr1, rf_Raddr2, rf_Waddr, alu_psr;
  sc_signal<sc_uint<6> > alu_ctrl;
  sc_signal<sc_uint<8> > alu_imm;
  sc_signal<sc_uint<16> > dm_addr, dm_datain, dm_dataout, pm_addr, pm_data, rf_Wdata;
  sc_signal<sc_uint<16> > rf_data1, rf_data2, alu_data1, alu_data2, alu_result;


  DM *dm;
  PM *pm;
  RF *rf;
  ALU *alu;


    //  [9:8] rf_w, rf_r; [7:6] dm_w, dm_r
  void proc();
  SC_CTOR(DP){
    dm = new DM("dm");
    pm = new PM("pm");
    rf = new RF("rf");
    alu = new ALU("alu");
    dm->wr_en(dm_wr);
    dm->rd_en(dm_rd);
    dm->addr(dm_addr);//
    dm->din(dm_datain);//
    dm->dout(dm_dataout);//
    pm->addr(pm_addr);//
    pm->data(pm_data);//
    rf->wr_en(rf_wr);//
    rf->rd_en(rf_rd);//
    rf->Raddr1(rf_Raddr1);//Rdestination//
    rf->Raddr2(rf_Raddr2);//Rsource//
    rf->Waddr(rf_Waddr);//
    rf->Wdata(rf_Wdata);//
    rf->data1(rf_data1);//Destination
    rf->data2(rf_data2);//Source
    alu->DATA1(alu_data1);//
    alu->DATA2(alu_data2);//
    alu->IMM(alu_imm);//
    alu->CONTROL(alu_ctrl);//
    alu->PSR(alu_psr);//
    alu->RESULT(alu_result);//
    }
    SC_METHOD(prc);
}

#endif