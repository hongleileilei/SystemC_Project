
#ifndef _DP_H
#define _DP_H
#include "systemc.h"
#include "alu.h"
#include "rf.h"
#include "dm.h"
#include "pm.h"
//#include "xiaohong.h"
//#include "register.h"

SC_MODULE(DP){
  sc_in<bool> clk;
  sc_out<sc_uint<16> > Instr;
  sc_out<sc_uint<4> > PSR;
  sc_out<sc_uint<16> > RTAR;
  sc_in<sc_uint<16> > PC;
  sc_in<sc_uint<8> > Imm;
  sc_in<sc_uint<6> > CTRL;
  sc_in<sc_uint<4> > RSRC, RDEST, R_W;				
  sc_in<sc_uint<1> > D1, D2, D3, D4;
  sc_in<sc_uint<1> > DM_R, DM_W, RF_R, RF_W;
  sc_in<sc_uint<2> > WB_C;
  sc_in<sc_uint<1> > pc_alu_mux;
  sc_in<sc_uint<1> > mdr_mux;
  sc_in<sc_uint<1> > mar_mux;



  sc_signal<sc_uint<1> > d1, d2, d3, d4;
  sc_signal<sc_uint<8> > imm;
  sc_signal<sc_uint<16> > dm_addr_reg, dm_datain_reg, rf_Wdata_reg_1, rf_Wdata_reg_2;
  sc_signal<sc_uint<16> > dm_addr, dm_datain, rf_Wdata, dm_dataout; 
  sc_signal<sc_uint<16> > alu_data1, alu_data2, alu_data1_reg, alu_data2_reg;
  sc_signal<sc_uint<16> > MAR, MDR;
  sc_signal<sc_uint<6> > ctrl;
  sc_signal<sc_uint<2> > wb_signal;
  sc_signal<sc_uint<4> > RWaddr_2, RWaddr_3;
  sc_signal<sc_uint<1> > rw1, rw2;
  sc_signal<sc_uint<1> > dr2;
  sc_signal<sc_uint<1> > dw2;
  sc_signal<sc_uint<1> > pc_alu;
  sc_signal<sc_uint<1> > mdr;
  sc_signal<sc_uint<1> > mar;

  DM *dm;
  PM *pm;
  RF *rf;
  ALU *alu;


////////////////////////////////////////////////////////////////////////////////////
  sc_uint<16> coeffs_DM[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 0, 12, 0, -24};
  sc_uint<16> coeffs_RF[16] = {0, 0, 0, 10, 0, 12, 0, 14, 12, 0, 0 ,0 ,0, 0, 0, 0};
  sc_uint<16> coeffs_PM[100] = {"0b0100001000000011", "0b0100010000000101", "0b0101100000010001", "0b000000000000", "0b000000000000", "0b0000010001010010", "0b000000000000","0b0100011000000111", "0b1001100000001111", "0b0100001000000101", "0b000000000000","0b000000000000","0b000000000000","0b0000001001011000", "0b0100100001000010", "0b0100010001000101"};
  int N_DM = sizeof(coeffs_DM)/sizeof(coeffs_DM[0]);
  int N_RF = sizeof(coeffs_RF)/sizeof(coeffs_RF[0]);
  int N_PM = sizeof(coeffs_PM)/sizeof(coeffs_PM[0]);
////////////////////////////////////////////////////////////////////////////////////

  SC_CTOR(DP){
    dm = new DM("DM_dm", &coeffs_DM[0], N_DM);
    pm = new PM("PM_pm", &coeffs_PM[0], N_PM);
    rf = new RF("RF_rf", &coeffs_RF[0], N_RF);
    alu = new ALU("ALU_alu");


    pm->addr(PC);
    pm->data(Instr);


    rf->clk(clk);
    rf->wr_en(rw2);
    rf->rd_en(RF_R);
    rf->Raddr1(RDEST);//Rdestination//
    rf->Raddr2(RSRC);//Rsource//
    rf->Waddr(RWaddr_2);
    rf->Wdata(rf_Wdata);//
    rf->data1(alu_data1_reg);//Destination
    rf->data2(alu_data2_reg);


    alu->clk(clk);
    alu->DATA1(alu_data1_reg);
    alu->DATA2(alu_data2_reg);
    alu->IMM(Imm);
    alu->CONTROL(CTRL);
    alu->D1(D1);
    alu->D2(D2);
    alu->D3(D3);
    alu->D4(D4);
    alu->PSR(PSR);
    alu->MAR(dm_addr_reg);
    alu->MDR(dm_datain_reg);
    alu->RFF(rf_Wdata_reg_1);
    alu->D_W(DM_W);
    alu->D_W_OUT(dw2);
    alu->D_R(DM_R);
    alu->D_R_OUT(dr2);
    alu->R_W(RF_W);
    alu->R_W_OUT(rw1);
    alu->RW(R_W);
    alu->RW_OUT(RWaddr_3);
    alu->WB(WB_C);
    alu->wb_signal(wb_signal);
    alu->pc_alu_mux(pc_alu);
    alu->mdr_mux(mdr);
    alu->mar_mux(mar);
    alu->PC(PC);




    dm->clk(clk);
    dm->ALU_in(rf_Wdata_reg_1);
    dm->wr_en(dw2);
    dm->rd_en(dr2);
    dm->addr(dm_addr_reg);
    dm->din(dm_datain_reg);
    dm->WB_C(wb_signal);
    dm->rf_w(rw1);
    dm->r_w(rw2);
    dm->RW(RWaddr_3);
    dm->rw(RWaddr_2);
    dm->dout(rf_Wdata);
  }
};

#endif

