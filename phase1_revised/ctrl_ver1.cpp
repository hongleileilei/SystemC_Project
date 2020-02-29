#include "ctrl.h"

void CTRL::proc(){
  //load instruction,psr
  lcl_instr=Instr.read();
  psr_val = psr.read();
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
    inter_pc += 1;
  }//no operation
  else{
    Disp = lcl_instr.range(7,0);//Displacement
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
        rf_w = 1;
        rf_r = 1;
        dm_w = 0;
        dm_r = 0;
        m1_val = 0;
        m2_val = 1;
        m3_val = 1;
        m4_val = 0;
        ctrl_signal=0b000001;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b000011;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b000111;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b000101;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001001;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001011;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001101;
        inter_pc+=1;//counting up
      }//MOV
    }//RF_OP
    else{
      if(opcode==5){
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
        ctrl_signal=0b000010;
        inter_pc+=1;//counting up
      }//ADDI
      else if(opcode==9){
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
        ctrl_signal=0b000100;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001000;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b000110;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001010;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001100;
        inter_pc+=1;//counting up
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
        ctrl_signal=0b001110;
        inter_pc+=1;//counting up
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
          ctrl_signal=0b010000;
          inter_pc+=1;//counting up
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
          ctrl_signal=0b010010;
          inter_pc+=1;//counting up
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
          ctrl_signal=0b001111;
          inter_pc+=1;//counting up
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
          ctrl_signal=0b010001;
          inter_pc+=1;//counting up
        }//ASH
      }//SHIFT OP
      else if(opcode==4){
        if(op_ext==0){
          if(imm_sign==1){//negative
            m1_val = 0;
          }
          else{
            m1_val = 1;
          }
          rf_w = 1;
          rf_r = 0;
          dm_w = 0;
          dm_r = 1;
          m2_val = 1;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b010100;
          inter_pc+=1;//counting up
        }//LOAD
        else if(op_ext==4){
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
          ctrl_signal=0b010101;
          inter_pc+=1;//counting up
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
          ctrl_signal=100110;
          inter_pc+=1;//counting up
        }//JAL
        else if(op_ext==12){
          if(cond_bit == 0){//if condition check bit is for EQ
            if(z==1){
              //BEQ TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              //BEQ FALSE
              inter_pc+= 1;
              pc.write(inter_pc);
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b011110;
          }//EQ
          else if(cond_bit==1){
            if(z==0){
              //BNE TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              //BNE FALSE
              inter_pc+= 1;
              pc.write(inter_pc);
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b011111;
          }//NE
          else if(cond_bit==13){
            if(z== 1 || n==1){
              //GE TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              //GE FALSE
              inter_pc += 1;
              pc.write(inter_pc);
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b100000;
          }//GE
          else if(cond_bit==2){
            if(c==1){
              //CS TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              inter_pc+= 1;
              pc.write(inter_pc);
              //CS FALSE
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b100001;
          }//CS
          else if(cond_bit==3){
            if(c==0){
              //CC TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              inter_pc+= 1;
              pc.write(inter_pc);
              //CC FALSE
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b100010;
          }//CC
          else if(cond_bit==6){
            if(n==1){
              //GT TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              inter_pc+= 1;
              pc.write(inter_pc);
              //GT FALSE
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b100011;
          }//GT
          else if(cond_bit==7){
            if(n==0){
              //LE TRUE
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
            }
            else{
              inter_pc+= 1;
              pc.write(inter_pc);
              //LE FALSE
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b100100;
          }//LE
          else if(cond_bit==12){
            if(n==0 & z==0){
              inter_pc = Rtar_val.read();
              pc.write(inter_pc);
              //LT TRUE
            }
            else{
              inter_pc+= 1;
              pc.write(inter_pc);
              //LT FALSE
            }
            rf_w = 0;
            rf_r = 1;
            dm_w = 0;
            dm_r = 0;
            m1_val = 0;
            m2_val = 0;
            m3_val = 0;
            m4_val = 0;
            ctrl_signal=0b100101;
          }//LT
        }//JC
      }//LOAD/STOR/JC/JAL
      else if(opcode==12){
        if(cond_bit==0){
          if(z==1){//ZNCF == 1XXX
          //BEQ TRUE
            inter_pc+= Disp;
            pc.write(inter_pc);
          //PC = PC+DISP
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
          //BEQ FALSE
          //PC = PC + 2
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b010110;
        }//EQ
        else if(cond_bit==1){
          if(z==0){
            //BNE TRUE
            inter_pc+= Disp;
            pc.write(inter_pc);
            //PC = PC + DISP
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
            //BNE FALSE
            //PC = PC + 2
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b010111;
        }//NE
        else if(cond_bit==13){
          if(z== 1 || n==1){
            inter_pc+= Disp;
            pc.write(inter_pc);
            //GE TRUE
          }
          else{
            inter_pc += 1;
            pc.write(inter_pc);
            //GE FALSE
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b011000;
        }//GE
        else if(cond_bit==2){
          if(c==1){
            inter_pc+= Disp;
            pc.write(inter_pc);
            //CS TRUE
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
            //CS FALSE
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b011001;
        }//CS
        else if(cond_bit==3){
          if(c==0){
            inter_pc+= Disp;
            pc.write(inter_pc);
            //CC TRUE
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
            //CC FALSE
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b011010;
        }//CC
        else if(cond_bit==6){
          if(n==1){
            inter_pc+= Disp;
            pc.write(inter_pc);
            //GT TRUE
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
            //GT FALSE
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b011011;
        }//GT
        else if(cond_bit==7){
          if(n==0){
            inter_pc+= Disp;
            pc.write(inter_pc);
            //LE TRUE
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
            //LE FALSE
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
          ctrl_signal=0b011100;
        }//LE
        else if(cond_bit==12){
          if(n==0 & z==0){
            inter_pc+= Disp;
            pc.write(inter_pc);
            //LT TRUE
          }
          else{
            inter_pc+= 1;
            pc.write(inter_pc);
            //LT FALSE
          }
          rf_w = 0;
          rf_r = 0;
          dm_w = 0;
          dm_r = 0;
          m1_val = 0;
          m2_val = 0;
          m3_val = 0;
          m4_val = 0;
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
        ctrl_signal=0b010011;
      }//LUI
    }//Other_OP
  }


  if(opcode != 4 && op_ext != 8){
  //writing to the otuput ports
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
    pc.write(inter_pc);
    Imm.write(imm);
    ///////
  }
  else{
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
    pc.write(inter_pc);
    Imm.write(imm);
    inter_pc=Rtar_val.read();
    pc.write(inter_pc);
  }

  ///
}
