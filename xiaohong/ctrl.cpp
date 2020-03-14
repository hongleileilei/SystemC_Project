#include "ctrl.h"

void CTRL::ctrl_in_reg(){
  temp_instr=Instr.read();
  temp_psr=psr.read();
  //cout<< sc_time_stamp() <<" "<<Instr.read()<< " is the instructions we read before reg" << " pc: " << inter_pc << endl;
}

void CTRL::counter(){
  counter_instr = temp_instr.read();
  counter_psr = temp_psr.read();
  //cout<< sc_time_stamp() <<" "<<counter_instr<< " 1_is the instructions we read after ctrl in reg" << ", pc: " << inter_pc << endl;
  counter_z = counter_psr.range(3,3);
  counter_f = counter_psr.range(2,2);
  counter_n = counter_psr.range(1,1);
  counter_c = counter_psr.range(0,0);
  counter_cond = counter_instr.range(11,8);
  counter_disp = counter_instr.range(7,0);

  if(counter_instr.range(15,12) == 4){
    //cout << "wo shi " << endl;
    if(counter_instr.range(7,4) == 8){
      inter_pc++;
      pc.write(inter_pc);
      inter_pc=Rtar_val.read();
      pc.write(inter_pc);
      inter_pc++;
    }//JAL
    else if(counter_instr.range(7,4)==12){
      if(counter_cond == 0){//if condition check bit is for EQ
        if(counter_z==1){
          //BEQ TRUE
          inter_pc = Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          //BEQ FALSE
          inter_pc+= 1;
          pc.write(inter_pc);
        }
      }
      else if(counter_cond == 1){
        if(counter_z==0){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);

        }
      }
      else if(counter_cond==13){
        if(counter_z==1 || counter_n==1){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);
        }
      }
      else if(counter_cond==2){
        if(counter_c==1){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);
        }
      }
      else if(counter_cond==3){
        if(counter_c==0){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);
        }
      }
      else if(counter_cond==6){
        if(counter_n==1){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);
        }
      }
      else if(counter_cond==7){
        if(counter_n==0){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);
        }
      }
      else if(counter_cond==12){
        if(counter_n==0 && counter_z==0){
          inter_pc=Rtar_val.read();
          pc.write(inter_pc);
        }
        else{
          inter_pc++;
          pc.write(inter_pc);
        }
      }
    }//Jcon
    else{
      if(start == 0){
        inter_pc=0;
        start=1;
      }
      pc.write(inter_pc);
      inter_pc++;      
    }
  }
  else if(counter_instr.range(15,12)==12){
    //cout << "xiao hong" << endl;
    if(counter_cond == 0){//if condition check bit is for EQ
      if(counter_z==1){
        //BEQ TRUE
        inter_pc += counter_disp;
        pc.write(inter_pc);
      }
      else{
        //BEQ FALSE
        inter_pc+= 1;
        pc.write(inter_pc);
      }
    }
    else if(counter_cond == 1){
      if(counter_z==0){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);

      }
    }
    else if(counter_cond==13){
      if(counter_z==1 || counter_n==1){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);
      }
    }
    else if(counter_cond==2){
      if(counter_c==1){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);
      }
    }
    else if(counter_cond==3){
      if(counter_c==0){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);
      }
    }
    else if(counter_cond==6){
      if(counter_n==1){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);
      }
    }
    else if(counter_cond==7){
      if(counter_n==0){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);
      }
    }
    else if(counter_cond==12){
      if(counter_n==0 && counter_z==0){
        inter_pc+=counter_disp;
        pc.write(inter_pc);
      }
      else{
        inter_pc++;
        pc.write(inter_pc);
      }
    }
  }//Bcond
  else{
    //cout << "wo kao le " << endl;
    if(start == 0){
      inter_pc=0;
      start=1;
    }
    pc.write(inter_pc);
    inter_pc++;
  }
  //cout<< sc_time_stamp() <<" "<<counter_instr<< " 2_is the instructions we read after ctrl in reg" << ", pc: " << inter_pc << endl;
}


void CTRL::proc(){
  //load instruction,psr
  lcl_instr=temp_instr.read();
  //cout<<lcl_instr<< " " << sc_time_stamp() << " is the instructions we read\n";
  psr_val = temp_psr.read();
  z = psr_val.range(3,3);//z value from psr
  f = psr_val.range(2,2);//f value from psr
  n = psr_val.range(1,1);//n value from psr
  c = psr_val.range(0,0);//c value from psr
  //if no operation, operated otherwise
  if(lcl_instr==0){
    rf_w = 0;
    rf_r = 0;
    dm_w = 0;
    dm_r = 0;
    m1_val = 0;
    m2_val = 0;
    m3_val = 0;
    m4_val = 0;
    ctrl_signal=0b100111;
    cout<<sc_time_stamp()<<" no operation\n";
  }//nop
  else{
    cond_bit = lcl_instr.range(11,8);//condition bit. ie, BEQ, BNE, etc.
    opcode = lcl_instr.range(15,12);//op code
    Rdest = lcl_instr.range(11,8);//Rdestination
    op_ext = lcl_instr.range(7,4);//op extension
    Rsrc = lcl_instr.range(3,0);//Rsource
    imm = lcl_instr.range(7,0);//Immediate
    imm_sign = lcl_instr.range(7,7);//sign of the imm
    shift_op = lcl_instr.range(7,5);//Shift Operation
    //RF data operation
    if(opcode==0){
      if(op_ext==5){
	cout<<sc_time_stamp()<<" add, Rsrc: " <<Rsrc<<", Rdest: "<<Rdest<<endl;
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 0;
        mar_m = 0;
        ctrl_signal=0b000001;
      }//ADD
      else if(op_ext==9){
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        ctrl_signal=0b000011;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
      }//SUB
      else if(op_ext==1){
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b000111;
      }//AND
      else if(op_ext==11){
        rf_w = 0;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b000101;
      }//CMP
      else if(op_ext==2){
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001001;
      }//OR
      else if(op_ext==3){
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001011;
      }//XOR
      else if(op_ext==13){
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001101;
      }//MOV
    }//RF_OP
    else{
      if(opcode==5){
	cout<<sc_time_stamp()<<" addi, Imm: " <<imm<<", Rdest: "<<Rdest<<endl;
        //cout<<"we shuold see it is a addi instruction\n";
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{//positive
          m1_val = 1;
        }
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b000010;
      }//ADDI
      else if(opcode==9){
	cout<<sc_time_stamp()<<" subi\n";
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{
          m1_val = 1;
        }
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b000100;
      }//SUBI
      else if(opcode==1){
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{
          m1_val = 1;
        }
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001000;
      }//ANDI
      else if(opcode==11){
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{
          m1_val = 1;
        }
        rf_w = 0;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 0;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b000110;
      }//CMPI
      else if(opcode==2){
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{
          m1_val = 1;
        }
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 1;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001010;
      }//ORI
      else if(opcode==3){
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{
          m1_val = 1;
        }
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 1;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001100;
      }//XORI
      else if(opcode==13){
        if(imm_sign==1){//negative
          m1_val = 0;
        }
        else{
          m1_val = 1;
        }
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m2_val = 0;
        m3_val = 1;
        m4_val = 1;
        wb_sig_temp=0;
        pc_alu_m = 0;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b001110;
      }//MOVI
      else if(opcode==8){
        if(shift_op==0){
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 1;
          dm_w = 0;
          dm_r = 0;
          m2_val = 0;
          m3_val = 1;
          m4_val = 1;
          wb_sig_temp=0;
          pc_alu_m = 0;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b010000;
        }//LSHI
        else if(shift_op==1){
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 1;
          dm_w = 0;
          dm_r = 0;
          m2_val = 0;
          m3_val = 1;
          m4_val = 1;
          wb_sig_temp=0;
          pc_alu_m = 0;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b010010;
        }//ASHI
        else if(shift_op==2){
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 1;
          dm_w = 0;
          dm_r = 0;
          m2_val = 1;
          m3_val = 1;
          m4_val = 1;
          wb_sig_temp=0;
          pc_alu_m = 0;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b001111;
        }//LSH
        else if(shift_op==3){
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 1;
          dm_w = 0;
          dm_r = 0;
          m2_val = 1;
          m3_val = 1;
          m4_val = 1;
          wb_sig_temp=0;
          pc_alu_m = 0;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b010001;
        }//ASH
      }//SHIFT OP
      else if(opcode==4){
        if(op_ext==0){
	  cout<<sc_time_stamp()<<" LOAD\n";
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 1;
          dm_w = 0;
          dm_r = 1;
          m2_val = 1;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=1;
          pc_alu_m = 0;
          mdr_m = 1;
          mar_m = 0;
          ctrl_signal=0b010100;
        }//LOAD
        else if(op_ext==4){
	  cout<<sc_time_stamp()<<" stor\n";
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 0;
          rf_r = 1;
          dm_w = 1;
          dm_r = 0;
          m2_val = 1;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=0;
          pc_alu_m = 0;
          mdr_m = 0;
          mar_m = 0;
          ctrl_signal=0b010101;
        }//STOR
        else if(op_ext==8){
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 1;
          dm_w = 0;
          dm_r = 0;
          m2_val = 1;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=100110;
        }//JAL
        else if(op_ext==12){
          if(cond_bit == 0){//if condition check bit is for EQ
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            wb_sig_temp=2;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            ctrl_signal=0b011110;
          }//EQ
          else if(cond_bit==1){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            wb_sig_temp=2;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            ctrl_signal=0b011111;
          }//NE
          else if(cond_bit==13){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            wb_sig_temp=2;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            ctrl_signal=0b100000;
          }//GE
          else if(cond_bit==2){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            wb_sig_temp=2;
            ctrl_signal=0b100001;
          }//CS
          else if(cond_bit==3){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            wb_sig_temp=2;
            ctrl_signal=0b100010;
          }//CC
          else if(cond_bit==6){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            wb_sig_temp=2;
            ctrl_signal=0b100011;
          }//GT
          else if(cond_bit==7){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            wb_sig_temp=2;
            ctrl_signal=0b100100;
          }//LE
          else if(cond_bit==12){
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            pc_alu_m = 1;
            mdr_m = 1;
            mar_m = 1;
            wb_sig_temp=2;
            ctrl_signal=0b100101;
          }//LT
        }//JC
      }//LOAD/STOR/JC/JAL
      else if(opcode==12){
        if(cond_bit==0){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b010110;
        }//EQ
        else if(cond_bit==1){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          wb_sig_temp=2;
          ctrl_signal=0b010111;
        }//NE
        else if(cond_bit==13){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b011000;
        }//GE
        else if(cond_bit==2){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b011001;
        }//CS
        else if(cond_bit==3){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b011010;
        }//CC
        else if(cond_bit==6){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b011011;
        }//GT
        else if(cond_bit==7){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b011100;
        }//LE
        else if(cond_bit==12){
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          wb_sig_temp=2;
          pc_alu_m = 1;
          mdr_m = 1;
          mar_m = 1;
          ctrl_signal=0b011101;
        }//LT
      }//BC
      else if(opcode==15){
        rf_w = 1;
        rf_r = 0;
        dm_w = 0;
        dm_r = 0;
        m1_val = 1;
        m2_val = 0;
        m3_val = 1;
        m4_val = 1;
        wb_sig_temp=2;
        pc_alu_m = 1;
        mdr_m = 1;
        mar_m = 1;
        ctrl_signal=0b010011;
      }//LUI
    }//Other_OP
  }
  alu_ctrl.write(ctrl_signal);
  m1.write(m1_val);
  m2.write(m2_val);
  m3.write(m3_val);
  m4.write(m4_val);
  rf_wr.write(rf_w);
  rf_rd.write(rf_r);
  dm_rd.write(dm_r);
  dm_wr.write(dm_w);
  R_src.write(Rsrc);
  R_dest.write(Rdest);
  R_w.write(Rdest);
  Imm.write(imm);
  wb_sig.write(wb_sig_temp);
  pc_alu_mux.write(pc_alu_m);
  mdr_mux.write(mdr_m);
  mar_mux.write(mar_m);
  //cout<< sc_time_stamp() <<" "<<counter_instr<< " 3_is the instructions we read after ctrl in reg" << ", pc: " << inter_pc << endl;
  //cout << sc_time_stamp() << ", ctrl contro: " << ctrl_signal << endl;
}

