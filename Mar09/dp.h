
#ifndef _DP_H
#define _DP_H
#include "systemc.h"
#include "alu.h"
#include "rf.h"
#include "dm.h"
#include "pm.h"
#include "register.h"

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
  sc_in<sc_uint<2> > WB_control;

  sc_signal<sc_uint<1> > d1, d2, d3, d4;
  sc_signal<sc_uint<8> > imm;
  sc_signal<sc_uint<16> > dm_addr_reg, dm_datain_reg, rf_Wdata_reg_1, rf_Wdata_reg_2;
  sc_signal<sc_uint<16> > dm_addr, dm_datain, rf_Wdata, dm_dataout; 
  sc_signal<sc_uint<16> > alu_data1, alu_data2, alu_data1_reg, alu_data2_reg;
  sc_signal<sc_uint<16> > MAR, MDR;
  sc_signal<sc_uint<6> > ctrl;



  DM *dm;
  PM *pm;
  RF *rf;
  ALU *alu;
  ALU_OUT *alu_out;
  ALU_IN *alu_in;
  WB *wb;

////////////////////////////////////////////////////////////////////////////////////
  sc_uint<16> coeffs_DM[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 0, 12, 0, -24};
  sc_uint<16> coeffs_RF[16] = {0, 0, 0, 10, 0, 12, 0, 14, 12, 0, 0 ,0 ,0, 0, 0, 0};
  sc_uint<16> coeffs_PM[100] = {"0b0100001000000011", "0b0100010000000101", "0b0101100000010001", "0b0000100001010010", "0b0100011000000111", "0b1001100000001111", "0b0100001000000101", "0b0000001001011000", "0b0100100001000010", "0b0100010001000101"};
  int N_DM = sizeof(coeffs_DM)/sizeof(coeffs_DM[0]);
  int N_RF = sizeof(coeffs_RF)/sizeof(coeffs_RF[0]);
  int N_PM = sizeof(coeffs_PM)/sizeof(coeffs_PM[0]);
////////////////////////////////////////////////////////////////////////////////////

  SC_CTOR(DP){
    dm = new DM("DM_dm", &coeffs_DM[0], N_DM);
    pm = new PM("PM_pm", &coeffs_PM[0], N_PM);
    rf = new RF("RF_rf", &coeffs_RF[0], N_RF);
    alu = new ALU("ALU_alu");
    alu_in = new ALU_IN ("alu_in");
    alu_out  =new ALU_OUT ("alu_out");
    wb = new WB("wb");


    pm->addr(PC);
    pm->data(Instr);


    rf->clk(clk);
    rf->wr_en(RF_W);
    rf->rd_en(RF_R);
    rf->Raddr1(RDEST);//Rdestination//
    rf->Raddr2(RSRC);//Rsource//
    rf->Waddr(RDEST);
    rf->Wdata(rf_Wdata);//
    rf->data1(alu_data1_reg);//Destination
    rf->data2(alu_data2_reg);


    alu_in->clk(clk);
    alu_in->DDATA1(alu_data1_reg);
    alu_in->DDATA2(alu_data2_reg);
    alu_in->IIMM(Imm);
    alu_in->CCONTROL(CTRL);
    alu_in->DATA1(alu_data1);
    alu_in->DATA2(alu_data2);
    alu_in->IMM(imm);
    alu_in->CONTROL(ctrl);
    alu_in->D1(D1);
    alu_in->D2(D2);
    alu_in->D3(D3);
    alu_in->D4(D4);
    alu_in->d1(d1);
    alu_in->d2(d2);
    alu_in->d3(d3);
    alu_in->d4(d4);



    alu->DATA1(alu_data1);
    alu->DATA2(alu_data2);
    alu->IMM(imm);
    alu->CONTROL(ctrl);
    alu->D1(d1);
    alu->D2(d2);
    alu->D3(d3);
    alu->D4(d4);
    alu->PSR(PSR);
    alu->MAR(dm_addr_reg);
    alu->MDR(dm_datain_reg);
    alu->RFF(rf_Wdata_reg_1);
    alu->MAKE(RTAR);


    alu_out->clk(clk);
    alu_out->MMAR(dm_addr_reg);
    alu_out->MMDR(dm_datain_reg);
    alu_out->RRF_TEM(rf_Wdata_reg_1);
    alu_out->MAR(dm_addr);
    alu_out->MDR(dm_datain);
    alu_out->RF_TEM(rf_Wdata_reg_2);


    wb->clk(clk);
    wb->ALU(rf_Wdata_reg_2);
    wb->PC(PC);
    wb->DM(dm_dataout);
    wb->out(rf_Wdata);
    wb->WB_C(WB_control);


    dm->wr_en(DM_W);
    dm->rd_en(DM_R);
    dm->addr(dm_addr);
    dm->din(dm_datain);
    dm->dout(dm_dataout);
  }
};

#endif

