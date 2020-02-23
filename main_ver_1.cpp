//1 does DM PM has clock sensitive
//2 JAL activity sequence
//3 pc 16bit, disp 8bit, how to deal with pc+disp





#include "systemc.h"
#include "math.h"
//4 modules completion required
//DM, PM, RF, ALU
//2 upper level modules
//DP, CTRL




using namespace std;
SC_MODULE(DM){
  sc_in<sc_uint<16> > addr;//address input
  sc_in<sc_uint<16> > din;//data input
  sc_in<sc_uint<1> > wr_en, rd_en;// write_enable && read_enable
  //sc_in<bool> clk;//clock
  sc_out<sc_uint<16> > dout;// data out

  sc_uint<16> Data[100];
  void proc(){
    if(wr_en.read() == 1){//write enable
      Data[addr.read()]=din.read();
    }
    else if(rd_en.read() == 1){//read enable
      dout.write(Data[addr.read()]);
    }
  }

  SC_CTOR(DM){
    for(int i=0; i<100; i++){
      Data[i] == 0;
    }

  }


};

SC_MODULE(PM){
  sc_in<sc_uint<16> > addr;
  sc_out<sc_uint<16> > data;

  sc_uint<16> Data[100];
  sc_uint<16> temp_data;
  sc_uint<16> temp_addr;
  void proc(){
    temp_addr = addr.read();
    temp_data = Data[temp_addr];
    data.write(temp_data);
  }
  SC_CTOR(PM){
    //Initialization
    for(int i =0; i<100; i++){
      Data[i]=0;
    }
  }


};

SC_MODULE(RF){
  sc_in<bool> clk;
  sc_in<sc_uint<1> > wr_en, rd_en;
  sc_in<sc_uint<4> > Raddr1, Raddr2, Waddr;
  sc_in<sc_uint<16> > Wdata;
  sc_out<sc_uint<16> > data1, data2;

  sc_uint<16> mem[16];
  void proc(){
    if(rd_en.read()==1){
      data1.write(mem[Raddr1.read()]);
      data2.write(mem[Raddr2.read()]);
    }
    else if(wr_en.read()==1){
      mem[Waddr.read()]=Wdata.read();
    }
  }
  SC_CTOR(RF){
    for(int i=0;i<16;i++){
      mem[i]=0;
    }
    SC_METHOD(proc);
    sensitive<<clk.pos();
  }
};

//Error exists: Immediate value signed or unsigned
SC_MODULE(ALU){
  //////////////////////////////////////
  //PORTS
  //////////////////////////////////////
  sc_in<bool> clk;
  sc_in<sc_uint<16> > DATA1, DATA2;
  sc_in<sc_int<8> > IMM;
  sc_in<sc_uint<6> > CONTROL;
  sc_out<sc_uint<4> > PSR;//PORTS SEQUENCE: cnfz
  sc_out<sc_uint<16> > RESULT;

  sc_uint<16> data1;
  sc_uint<16> data2;
  sc_uint<16> result;

  sc_int<8> im;
  sc_uint<6> control;
  sc_uint<4> psr;
  sc_uint<16> imm, imm_ori;
  int i;

  sc_uint<16> data2inv, imminv;
  int lendata1, lendata2, lenresult, lenimm, lenresult_cmp;
  sc_uint<17> mmax;
  sc_uint<16> result_cmp;
  sc_uint<16> right1;
  sc_uint<4> right2;
  sc_uint<16> right3;
  sc_uint<4> right4;
  sc_uint<1> left_most;

  void proc(){
    data1 = DATA1.read();//read from RF
    data2 = DATA2.read();//read from RF
    control = CONTROL.read();//read from DP
    im = IMM.read();//read from DP
    //define some useful lenth
    lendata1 = 16; lendata2 = 16; lenresult = 16; lenimm = 16; lenresult_cmp = 16;
    mmax = (1<<16);//comparison maximum, used for overflow and compare
    for(i = 0; i<16; i++){//get the NOT
      data2inv[i] = ~data2[i];
    }
    data2inv += 1;

    //sign extension for further usage
    for(i=8;i<16;i++){
      imm[i] = im[7];
    }
    for(i=0;i<8;i++){
      imm[i] = im[i];
    }

    //sign extension for ORI, XORI, MOVI operation
    for(i=8;i<16;i++){
      imm_ori[i]=0;
    }
    for(i=0;i<8;i++){
      imm_ori[i] = im[i];
    }

    //get NOT for imm
    for(i=0;i<16;i++){
      imminv[i] = ~imm[i];
    }
    imminv+1;

    //ADD
    if(control == 1){
      ///overflow start
      if((mmax-data1) >= data2)
        result = data1 + data2;
      else
        result = mmax;
        //overflow done

      //carrying manipulation
      for(i=15; i>= 0;i--){
        if(data1[i]==0)
          lendata1--;
        else
          break;
      }

      for(i=15;i>=0;i--){
        if(data2[i]==0)
          lendata2--;
        else
          break;
      }

      for(i=15;i>=0;i--){
        if(result[i]==0)
          lenresult--;
        else
          break;
      }
      if(lenresult > max(lendata1, lendata2))
        psr[0] = 1;
      else
        psr[0] = 0;
      lendata1 = 16; lendata2 = 16; lenresult = 16;
      //carrying manipulation FINISHED

      if(result == 0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[1] = 0;
      psr[2] = 0;
      //psr done
    }
    else if(control == 2){//ADDI
      //overflow start
      if((mmax-data1)>=imm)
        result = data1+imm;
      else
        result=mmax;
      //overflow done

      //carrying manipulation
      for(i=15;i>=0;i--){
        if(data1[i]==0)
          lendata1--;
        else
          break;
      }
      for(i=15;i>=0;i--){
        if(imm[i]==0)
          lenimm--;
        else
          break;
      }
      for(i=15;i>=0;i--){
        if(result[i] == 0)
          lenresult--;
        else
          break;
      }
      if(lenresult>max(lendata1,lenimm))
        psr[0]=1;
      else
        psr[0] = 0;
      lendata1=16; lendata2=16; lenimm=16;
      //carrying manipulation

      //psr manipulation
      if(result == 0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[1] = 0;
      psr[2] = 0;
      //psr manipulation
    }

    else if(control==3){//SUB
      //overflow detection
      if(data1 >= data2){
        result = data1 - data2;
        //carrying manipulation
        for(i=15;i>=0;i++){
          if(data1[i] == 0)
            lendata1--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(data2[i]==0)
            lendata2--;
          else
            break;
        }
        if(lenresult < max(lendata1, lendata2))
          psr[0] = 1;
        else
          psr[0] = 0;
        lendata1 = 16; lendata2 = 16; lenresult = 16;
        //carrying manipulation FINISHED
        psr[1]= 0;
      }
      else{
        psr[1]=1;
        psr[0]=1;
        result=0;
      }
      //overflow done

      //psr value manipulation
      if(result==0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[2] = 0;
      //psr value manipulation DONE
    }
    else if(control == 4){//SUBI
      //overflow detection
      if(data1 >= imm){
        result = data1 - imm;
        //carrying manipulation
        for(i-15;i>=0;i--){
          if(data1[i] == 0)
            lendata1--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(imm[i]==0){
            lenimm--;
          }
          else{
            break;
          }
        }
        for(i=15;i>=0;i--){
          if(result[i]==0){
            lenresult--;
          }
          else{
            break;
          }
        }
        if(lenresult<max(lendata1,lenimm))
          psr[0]=1;
        else
          psr[0]=0;
        lendata1=16;
        lenimm=16;
        lenresult=16;
        //carrying manipulation Done
        psr[1]=0;
      }
      else{
        psr[1]=1;
        psr[0]=1;
        result=0;
      }
      //overflow detection down
      //psr manipulation
      if(result==0)
        psr[3]=1;
      else
        psr[3]=0;
      psr[2]=0;
    }
    else if (control==5){//CMP
      //overflow detection and manipulation
      if(data1>=data2){
        result_cmp=data1-data2;
        for(i=15;i>=0;i--){
          if(data1[i]==0)
            lendata1--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(data2[i]==0)
            lendata2--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(result_cmp[i]==0)
            lenresult_cmp--;
          else
            break;
        }
        if(lenresult_cmp<max(lendata1,lendata2))
          psr[0]=1;
        else
          psr[0]=0;
        lendata1=16;lendata2=16;lenresult_cmp=16;
        //carrying manipulation is done
        psr[1]=0;
      }
      else{
        psr[1]=1;
        psr[0]=1;
        result_cmp=0;
      }
      //overflow detection and manipulation is done
      //psr manipulation
      if(result_cmp==0)
        psr[3]=1;
      else
        psr[3]=0;
      psr[2]=0;
    }
    else if (control==6){//CMPI
      if(data1>=imm){
        result_cmp=data1-imm;
        //carrying manipulation
        for(i=15;i>=0;i--){
          if(data1[i]==0)
            lendata1--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(imm[i]==0)
            lenimm--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(result_cmp[i]==0)
            lenresult_cmp--;
          else
            break;
        }
        if(lenresult_cmp<max(lendata1,lenimm))
          psr[0]=1;
        else
          psr[0]=0;
        lendata1=16;lendata2=16;lenimm=16;
        //carrying manipulation is done
        psr[1]=0;
      }
      else{
        psr[1]=1;
        psr[0]=1;
        result_cmp=0;
      }
      //overflow detection and manipulation is done
      //psr manipulation
      if(result_cmp==0)
        psr[3]=1;
      else
        psr[3]=0;
      psr[2]=0;
    }

    else if (control==7){//AND
      for(i=0;i<16;i++)
        result[i]=data1[i] & data2[i];
    }
    else if (control==8){//ANDI
      for(i=0;i<16;i++)
        result[i]=data1[i] & imm[i];
    }
    else if (control==9){//OR
      for(i=0;i<16;i++)
        result[i]=data1[i] | data2[i];
    }
    else if (control==10){//ORI
      for(i=0;i<16;i++)
        result[i]=data1[i] | imm[i];
    }
    else if (control==11){//XOR
      for(i=0;i<16;i++)
        result[i]=data1[i] ^ data2[i];
    }
    else if (control==12){//XORI
      for(i=0;i<16;i++)
        result[i]=data1[i] ^ imm[i];
    }
    else if (control==13){//MOV
      result=data2;
        }
    else if (control==14){//MOVI
      result=imm_ori;
    }
    else if (control==15){//LSH
      if(data2[15]==1){//negative we need to take the reverse first
        for(i=0;i<16;i++)
          right1[i]=~data1[i];
        right1+=1;
        result=data1>>right1;
      }
      else{
        result=data1<<data2;
      }
    }
    else if (control==16){//LSHI
      if(imm[4]==1){//negative we need to take the reverse first
        for(i=0;i<5;i++)
          right2[i]=~imm[i];
      }
    }

  }
  SC_CTOR(ALU){
  }
};

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
  sc_out<sc_uint<4> > psr_val;
  sc_out<sc_uint<16> > Rtar_val;
  sc_in<sc_uint<8> > pc;
  sc_in<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
  sc_in<sc_uint<6> > ctrl;
  sc_in<sc_uint<4> > Rsrc, Rdest;

  sc_uint<16> temp_instr;
  sc_uint<4> temp_psr_val;
  sc_uint<16> temp_Rtar_val;
  sc_uint<8> temp_pc;
  sc_uint<8> temp_Imm;
  void proc(){

  }
  SC_CTOR(DP){
    //need to connect DP with the four module above
  }
};

SC_MODULE(CTRL){
    //////////////////////////
    //PORTS
    /////////////////////////
    sc_in<sc_uint<16> > Instr;
    sc_in<sc_uint<4> > psr;//ZNCF
    sc_in<sc_uint<16> > Rtar_val;//////////////////////// Rtar
    sc_out<sc_uint<16> > pc;
    sc_out<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
    sc_out<sc_uint<6> > ctrl;
    sc_out<sc_uint<4> > R_src, R_dest;
    /////////////////////////
    //PORTS
    /////////////////////////


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
    sc_uint<16> inter_pc;
    sc_uint<3> shift_op;
    //local method
    void proc(){
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
          }//LUI operation
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
