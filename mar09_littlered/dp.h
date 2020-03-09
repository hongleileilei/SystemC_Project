
#ifndef _DP_H
#define _DP_H
#include "systemc.h"
#include "alu.h"

#include "rf.h"
#include "dm.h"
#include "pm.h"

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
  sc_in<bool> clk;
  sc_out<sc_uint<16> > Instr;
  sc_out<sc_uint<4> > PSR;
  sc_out<sc_uint<16> > RTAR;
  sc_in<sc_uint<16> > PC;
  sc_in<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
  sc_in<sc_uint<6> > CTRL;
  sc_in<sc_uint<4> > RSRC, RDEST, R_W;
  sc_in<sc_uint<1> > D1, D2, D3, D4;
  sc_in<sc_uint<1> > DM_R, DM_W, RF_R, RF_W;

  sc_signal<sc_uint<16> > dm_addr, dm_datain, dm_dataout, rf_Wdata;
  sc_signal<sc_uint<16> > alu_data1, alu_data2;
  sc_signal<sc_uint<16> > MAR, MDR;

  DM *dm;
  PM *pm;
  RF *rf;
  ALU *alu;


  SC_CTOR(DP){
    dm = new DM("DM_dm");
    pm = new PM("PM_pm");
    rf = new RF("RF_rf");
    alu = new ALU("ALU_alu");

    dm->wr_en(DM_W);
    dm->rd_en(DM_R);
    dm->addr(dm_addr);//
    dm->din(dm_datain);//
    dm->dout(dm_dataout);//

    pm->addr(PC);//
    pm->data(Instr);//

    rf->clk(clk);
    rf->wr_en(RF_W);//
    rf->rd_en(RF_R);//
    rf->Raddr1(RDEST);//Rdestination//
    rf->Raddr2(RSRC);//Rsource//
    //rf->Raddr2(RTAR);/////////////////////////////////////////////////////  BE CAREFULL //////////////////////////////////////////////
    rf->Waddr(RDEST);//  //////////////////  YOU WEN TI  //////////////////////////////////////
    rf->Wdata(rf_Wdata);//
    //rf->data2(RTAR);
    rf->data1(alu_data1);//Destination
    rf->data2(alu_data2);//Source

    alu->DATA1(alu_data1);//
    alu->DATA2(alu_data2);//
    alu->IMM(Imm);//
    alu->CONTROL(CTRL);//
    alu->D1(D1);
    alu->D2(D2);
    alu->D3(D3);
    alu->D4(D4);
    alu->rtar_out(RTAR);
    alu->PSR(PSR);//
    alu->MAR(dm_addr);//
    alu->MDR(dm_datain);
    alu->RF(rf_Wdata);
  }
};

#endif
