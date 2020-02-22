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
    sc_in<sc_uint<4> > psr;//ZNCF
    sc_in<sc_uint<16> > Rtar_val;//////////////////////// Rtar
    sc_out<sc_uint<8> > pc;
    sc_out<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
    sc_out<sc_uint<6> > ctrl;
    sc_out<sc_uint<4> > R_src, R_dest, R_amt;

    //displacement is defined locally， and some other decompositions
    sc_uint<16> lcl_instr;
    sc_uint<8> Disp;
    sc_uint<4> cond_bit;
    sc_uint<4> opcode;
    sc_uint<4> Rdest;
    sc_uint<4> op_ext;
    sc_uint<4> Rsrc;
    sc_uint<8> imm;
    sc_uint<4> psr_val;
    sc_uint<1> z;
    sc_uint<1> n;
    sc_uint<1> c;
    sc_uint<1> f;
    sc_uint<8> inter_pc;
    sc_uint<3> shift_op;
    sc_uint<1> sf
    //local method
    void proc(){
      //Done Op: add, addi, sub, subi, and, andi, cmp, cmpi
      //         or, ori, xor, xori, mov, movi, stor, load,
      //         jal, jcond, bcond, lui, lsh, lshi, ash, ashi
      //Pending: 
      //ATTENTION: jal NOT SURE FOR ACTIVITIES SEQUENCE
      lcl_instr=Instr.read();//retrieve instruction
      psr_val = psr.read();//retrieve psr value
      z = psr_val.range(3,3);//z value from psr
      n = psr_val.range(2,2);//n value from psr
      c = psr_val.range(1,1);//c value from psr
      f = psr_val.range(0,0);//f value from psr
      if(lcl_instr==0){
        //no operation should be Done
        //only counting up the internal
        //program counter to retrieve
        //the next instruction
        ctrl.write("NOP");//NOP
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
            ctrl.write("ADD");//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//ADD instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 9){
            ctrl.write("SUB");//pasing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//SUB instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 1){
            ctrl.write("AND");//pasing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsrouce
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//AND instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 11){
            ctrl.write("CMP");//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//CMP instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 2){
            ctrl.write("OR");//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//OR instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 3){
            ctrl.write("XOR");//passing signals
            R_dest.write(Rdest);//Rdestination
            R_src.write(Rsrc);//Rsource
            inter_pc+=1;//counting up
            pc.write(inter_pc);//program counter write
          }//XOR instruction. Passing Rdest, Rsrc, PC, CTRL signal
          else if(op_ext == 13){
             ctrl.write("MOV");//passing signals
             R_dest.write(Rdest);//Rdestination
             R_src.write(Rsrc);//Rsource
             inter_pc+=1;//counting up
             pc.write(inter_pc);//program counter write
          }//MOV instruction. Passing Rdest, Rsrc, PC, CTRL signal
        }
        //Immediate Operation or ELSE
        else{
          if(opcode == 5){
            ctrl.write("ADDI");//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//ADDI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 9){
            ctrl.write("SUBI");//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//SUBI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 1){
            ctrl.write("ANDI");//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//ANDI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 11){
            ctrl.write("CMPI");//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//CMPI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 2){
            ctrl.write("ORI");//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//ORI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 3){
            ctrl.write("XORI");//CTRL signal
            Imm.write(imm);//Immediate value write
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//internal program counter counting up
            pc.write(inter_pc);//program counter write
          }//XORI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 13){
             ctrl.write("MOVI");//CTRL signal
             Imm.write(imm);//Immediate value write
             R_dest.write(Rdest);//Rdestination
             inter_pc += 1;//internal program counter counting up
             pc.write(inter_pc);//program counter write
          }//MOVI instruction. Passing Imm, Rdest, PC, CTRL signal
          else if(opcode == 8){//SHIFT OPERATION
            if(shift_op == 0){//LSHI
              Imm.write(imm);//Immediate
              R_dest.write(Rdest);//Rdestination
              inter_pc += 1;//counting up for next instruction
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write("LSHI");//control signal
            }
            else if(shift_op == 1){//ASHI
              Imm.write(imm);//Immediate
              R_dest.write(Rdest);//Rdestination
              inter_pc += 1;//counting up for next instruction
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write("ASHI");//control signal
            }
            else if(shift_op == 2){//LSH
              R_dest.write(Rdest);//Rdestination
              R_src.write(Rsrc);//Ramount
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write("LSH");//control signal
            }
            else if(shift_op == 3){//ASH
              R_dest.write(Rdest);//Rdestination
              R_src.write(Rsrc);//Ramount
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//retrieve new instruction
              ctrl.write("ASH");//control signal
            }
          }//SHIFT instruction...PENDING for tutorial
          else if(opcode == 4){//LOAD/STORE/JC/JAL
            if(op_ext == 0){
              ctrl.write("LOAD");//Load signal
              R_dest.write(Rdest);//Rdestination
              R_src.write(Rsrc);//Address from DataMemory
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//write to program counter
            }//LOAD instruction. Passing DM address (Srouce), Rdest, PC, CTRL signal
            else if(op_ext == 4){
              ctrl.write("STORE");//Store signal
              R_dest.write(Rdest);//Rsource
              R_src.write(Rsrc);//Address from DataMomery
              inter_pc += 1;//program counter counting up
              pc.write(inter_pc);//write to program counter
            }//STOR instruction. Passing DM address (Destination), Rdest (Srouce), PC, CTRL signal
            else if(op_ext == 8){
              ctrl.write("JAL");// JAL signal
              R_dest.write(Rdest);//Rlink
              R_src.write(Rsrc);//Rtarget
              inter_pc += 1;//counting up the program counter
              pc.write(inter_pc);//write back the original counter
              //right now we need to make sure that the Rtarget need to get the value in RF[Rtar]
              //fast enough to meet clcok cycle requirement
              inter_pc = Rtar_val.read();//program counter counting up
              pc.write(inter_pc);//write the new program counter
            }//JAL instruction. Passing Rlink, Rtarget, PC, CTRL signal.
            else if(op_ext == 12){//Jcond
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
                ctrl.write("EQ");
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
                ctrl.write("NE");
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
                ctrl.write("GE");
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
                ctrl.write("CS");
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
                ctrl.write("CC");
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
                ctrl.write("GT");
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
                ctrl.write("LE");
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
                ctrl.write("LT");
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
              ctrl.write("EQ");
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
              ctrl.write("NE");
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
              ctrl.write("GE");
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
              ctrl.write("CS");
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
              ctrl.write("CC");
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
              ctrl.write("GT");
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
              ctrl.write("LE");
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
              ctrl.write("LT");
            }
            ctrl.write("Bcond");
          }
          else if(opcode == 15){
            Imm.write(imm);//Immediate
            R_dest.write(Rdest);//Rdestination
            inter_pc += 1;//program counter counting up
            pc.write(inter_pc);//retrieve new instructions
            ctrl.write("LUI");//control signal
          }
        }
      }
    }
    //constructor
    SC_CTOR(CTRL){
      SC_METHOD(proc);
      sensitive<<Instr;
    }

};

int sc_main(int argc, char* argv[]){
  return(0);
}
