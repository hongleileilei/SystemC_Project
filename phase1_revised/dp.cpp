


#include "dp.h"

  void DP:: proc(){
    rf_wr = CTRL.read().range(9, 9);
    rf_rd = CTRL.read().range(8, 8);
    dm_wr = CTRL.read().range(7, 7);
    dm_rd = CTRL.read().range(6, 6);
    alu_ctrl = CTRL.read().range(5, 0);
    // immediate value operation
    // ADDI SUBI ANDI ORI XORI MOVI LSHI ASHI LUI
    if (alu_ctrl == sc_uint<6>(2) || alu_ctrl == sc_uint<6>(4) || alu_ctrl == sc_uint<6>(8) || alu_ctrl == sc_uint<6>(10) || alu_ctrl == sc_uint<6>(12) || alu_ctrl == sc_uint<6>(14) || alu_ctrl == sc_uint<6>(16) || alu_ctrl == sc_uint<6>(18) || alu_ctrl == sc_uint<6>(19) ) {
      rf_Wdata.write( alu_result.read() );
 	  }

    // regular value operation
    // ADD SUB AND OR XOR MOV LSH ASH
    else if (alu_ctrl == sc_uint<6>(1) || alu_ctrl == sc_uint<6>(3) || alu_ctrl== sc_uint<6>(7) || alu_ctrl == sc_uint<6>(9) || alu_ctrl == sc_uint<6>(11) || alu_ctrl == sc_uint<6>(13) || alu_ctrl == sc_uint<6>(15) || alu_ctrl == sc_uint<6>(17) ){
      rf_Wdata.write( alu_result.read() );
	  }
    //
    // LOAD
	  else if (alu_ctrl == sc_uint<6>(20) ) {
      dm_addr.write( alu_result.read() );
		  rf_Wdata.write( dm_dataout.read() );
	  }
    // STOR operation, ALU output connected to dm, rf output connected to dm address
    else if (alu_ctrl == sc_uint<6>(21) ) {
		  dm_datain.write( alu_result.read() );
		  dm_addr.write( rf_Raddr1.read() );
	  }
    // CMP oparations, wr, rd are all 0s.
    else if (alu_ctrl == sc_uint<6>(5) ) {
		  ;
	  }
    // CMPI
    else if (alu_ctrl == sc_uint<6>(6) ) {
		  ;
	  }
    // JUMP condition, which does not pass ALU
    else if (alu_ctrl == sc_uint<6>(30) || alu_ctrl == sc_uint<6>(31) || alu_ctrl == sc_uint<6>(32) || alu_ctrl == sc_uint<6>(33) || alu_ctrl == sc_uint<6>(34) || alu_ctrl == sc_uint<6>(35) || alu_ctrl == sc_uint<6>(36) || alu_ctrl == sc_uint<6>(37) ) {
      RTAR.write( rf_data2.read() );
	  }
    //JAL operation, which does not pass ALU
    else if (alu_ctrl == sc_uint<6>(38) ) {
		  rf_Wdata.write( PC.read() );
		  RTAR.write( rf_data2.read() );
	  }
  }
