


#include "ctrl.h"

    void CTRL::proc(){
      //Done Op: add, addi, sub, subi, and, andi, cmp, cmpi
      //         or, ori, xor, xori, mov, movi, stor, load,
      //         jal, jcond, bcond, lui, lsh, lshi, ash, ashi
      //Pending:
      //ATTENTION: jal NOT SURE FOR ACTIVITIES SEQUENCE
      //ATTENTION: Disp is 8bit but program counter is 16bit, how to deal with pc+Disp

      lcl_instr=Instr.read();//retrieve instruction
      psr_val = psr.read();//retrieve psr value
      z = psr_val.range(3,3);//z value from psr
      f = psr_val.range(2,2);//f value from psr
      n = psr_val.range(1,1);//n value from psr
      c = psr_val.range(0,0);//c value from psr
      if(lcl_instr==0){
        //no operation should be Done
        //only counting up the internal
        //program counter to retrieve
        //the next instruction
        ctrl_signal=0b0000100111;
        ctrl.write(ctrl_signal);//NOP
        inter_pc += 1;
      }//NOP
      else{
        //there shuold be some operations to be done
        //in either datapath or controller
        //first retrieve the instruction and decoding it
        Disp = lcl_instr.range(7,0);//Displacement
        cond_bit = lcl_instr.range(11,8);//condition bit. ie, BEQ, BNE, etc.
        opcode = lcl_instr.range(15,12);//op code
        Rdest = lcl_instr.range(11,8);//Rdestination
        op_ext = lcl_instr.range(7,4);//op extension
        Rsrc = lcl_instr.range(3,0);//Rsource
        imm = lcl_instr.range(7,0);//Immediate
        shift_op = lcl_instr.range(7,5);//Shift Operation
        //if first 4 digits equals to 0, UN-IMM operation
        //Finished except specific CTRL signals
        if(opcode == 0){
          if(op_ext == 5){
            ctrl_signal=0b1100000001;
            ctrl.write(ctrl_signal);//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//ADD instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 9){
            ctrl_signal=0b1100000011;
            ctrl.write(ctrl_signal);//pasing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//SUB instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 1){
            ctrl_signal=0b1100000111;
            ctrl.write(ctrl_signal);//pasing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsrouce
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//AND instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 11){
            ctrl_signal=0b0100000101;
            ctrl.write(ctrl_signal);//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//CMP instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 2){
            ctrl_signal=0b1100001001;
            ctrl.write(ctrl_signal);//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//OR instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 3){
            ctrl_signal=0b1100001011;
            ctrl.write(ctrl_signal);//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//XOR instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 13){
            ctrl_signal=0b1100001101;
            ctrl.write(ctrl_signal);//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            R_w.write(Rdest);
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//MOV instruction. Passing Rdest, Rsrc, PC, CTRL signal
        }
        //Immediate Operation or ELSE
        else{
          if(opcode == 5){
            ctrl_signal=0b1100000010;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//ADDI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 9){
            ctrl_signal=0b1100000100;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//SUBI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 1){
            ctrl_signal=0b1100001000;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//ANDI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 11){
            ctrl_signal=0b0100000110;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//CMPI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 2){
            ctrl_signal=0b1100001010;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//ORI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 3){
            ctrl_signal=0b1100001100;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//XORI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 13){
            ctrl_signal=0b1100001110;
            ctrl.write(ctrl_signal);//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            R_w.write(Rdest);
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//MOVI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 8){//SHIFT OPERATION
            if(shift_op == 0){//LSHI
              ctrl_signal=0b1100010000;
              Imm.write(imm);//Immediate
              R_dest.write(Rdest);//Rdestination
              R_w.write(Rdest);
              inter_pc += 1;//counting up for next instruction
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write(ctrl_signal);//control signal
            }
            else if(shift_op == 1){//ASHI
              ctrl_signal=0b1100010010;
              Imm.write(imm);//Immediate
              R_dest.write(Rdest);//Rdestination
              R_w.write(Rdest);
              inter_pc += 1;//counting up for next instruction
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write(ctrl_signal);//control signal
            }
            else if(shift_op == 2){//LSH
              ctrl_signal=0b1100001111;
              R_dest.write(Rdest);//Rdestination
              R_src.write(Rsrc);//Ramount
              R_w.write(Rdest);
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write(ctrl_signal);//control signal
            }
            else if(shift_op == 3){//ASH
              ctrl_signal=0b1100010001;
              R_dest.write(Rdest);//Rdestination
              R_src.write(Rsrc);//Ramount
              R_w.write(Rdest);
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write(ctrl_signal);//control signal
            }
          }//SHIFT instruction...PENDING for tutorial
          else if(opcode == 4){//LOAD/STORE/JC/JAL
            if(op_ext == 0){
              ctrl_signal=0b0001010100;
              ctrl.write(ctrl_signal);//Load signal
              R_dest.write(Rdest);//Rdestination
              R_src.write(Rsrc);//Address from DataMemory
              R_w.write(Rdest);
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//write to program counter

            }//LOAD instruction. Passing DM address (Srouce), Rdest, PC, CTRL signal
            else if(op_ext == 4){
              ctrl_signal=0b0010010101;
              ctrl.write(ctrl_signal);//Store signal
              R_dest.write(Rdest);//Rsource
              R_src.write(Rsrc);//Address from DataMomery
              R_w.write(Rdest);
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//write to program counter
            }//STOR instruction. Passing DM address (Destination), Rdest (Srouce), PC, CTRL signal
            else if(op_ext == 8){
              ctrl_signal=0b1100100110;
              ctrl.write(ctrl_signal);// JAL signal
              R_dest.write(Rdest);//Rlink
              R_src.write(Rsrc);//Rtarget
              R_w.write(Rdest);
              inter_pc += 1;//counting up the program counter
              pc.write(inter_pc);//write back the original counter
              //right now we need to make sure that the Rtarget need to get the value in RF[Rtar]
              //fast enough to meet clcok cycle requirement
              inter_pc = Rtar_val.read();//program counter counting up
              pc.write(inter_pc);//write the new program counter
            }//JAL instruction. Passing Rlink, Rtarget, PC, CTRL signal.
            else if(op_ext == 12){//Jcond
              //RF read required
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100011110;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 1){//if condition check bit is for NE
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100011111;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 13){//if condition check bit is for GE
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100100000;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 2){//if condition check bit is for CS
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100100001;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 3){//if condition check bit is for CC
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100100010;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 6){//if condiotion check bit is for GT
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100100011;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 7){//if condition check bit is for LE
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100100100;
                ctrl.write(ctrl_signal);
              }
              else if(cond_bit == 12){//if condition check bit is for LT
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
                R_dest.write(Rdest);
                R_src.write(Rsrc);
                R_w.write(Rdest);
                ctrl_signal=0b0100100101;
                ctrl.write(ctrl_signal);
              }
              //ctrl.write("Jcond");
            }
          }
          else if(opcode == 12){//NEED PSR ZNCF VALUE FOR FURTHER OP
            if(cond_bit == 0){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000010110;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 1){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000010111;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 13){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000011000;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 2){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000011001;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 3){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000011010;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 6){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000011011;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 7){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000011100;
              ctrl.write(ctrl_signal);
            }
            else if(cond_bit == 12){
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
              R_dest.write(Rdest);
              R_src.write(Rsrc);
              R_w.write(Rdest);
              ctrl_signal=0b0000011101;
              ctrl.write(ctrl_signal);
            }
            //ctrl.write("Bcond");
          }
          else if(opcode == 15){
            ctrl_signal=0b1000010011;
            Imm.write(imm);//Immediate
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);
            R_w.write(Rdest);
            inter_pc += 1;//program counter counting up
            pc.write(inter_pc);//retrieve new instructions
            ctrl.write(ctrl_signal);//control signal
          }//LUI operation
        }
      }
    }
