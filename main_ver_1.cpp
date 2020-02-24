//1 does DM PM CTRL has clock sensitive
//2 JAL activity sequence
//3 pc 16bit, disp 8bit, how to deal with pc+disp



//ESE501Project1_Phase1
//AUTHOR
//Ke Ma. SID: 112846615
//Honglei Liu. SID:112848525

#include "systemc.h"
#include "math.h"
//4 modules completion required
//DM, PM, RF, ALU
//2 upper level modules
//DP, CTRL




using namespace std;

//DM finished
//DM is sensitive to write enable and read enable signal
//it has a hundred 16-bit memory storing data from STOR function
SC_MODULE(DM){
  //clock is needed?
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
    SC_METHOD(proc);
    sensitive<<wr_en;
    sensitive<<rd_en;
  }


};

//PM finished
//PM is only sensitive to addr changes
//For inside, addr is for searching different instructions
//For outside, addr is consiered as PC, program counter
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
    SC_METHOD(proc);
    sensitive<<addr;
  }


};

//RF finished
//RF is sensitive to clock positive edge, for highest priority
SC_MODULE(RF){
  //sc_in<bool> clk;
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
    //sensitive<<clk.pos();
  }
};

//ALU finished
//ALU should be sensitive to clock, for highest priority, if clock enabled
//ALU is also controlled by control signals
//Error exists: Immediate value signed or unsigned
SC_MODULE(ALU){
  //////////////////////////////////////
  //PORTS
  //////////////////////////////////////
  //sc_in<bool> clk;//clock
  sc_in<sc_uint<16> > DATA1, DATA2;//as indicated below
  sc_in<sc_uint<8> > IMM;//immediate value
  sc_in<sc_uint<6> > CONTROL;//control signal passing from DP, 6-bit
  sc_out<sc_uint<4> > PSR;//PORTS SEQUENCE: cnfz
  sc_out<sc_uint<16> > RESULT;//computation result
  //data1:Rdest data2:Rsrc,Ramt,Raddr
  sc_uint<16> data1;
  sc_uint<16> data2;
  sc_uint<16> result;

  sc_uint<8> im;
  sc_uint<6> control;
  sc_uint<4> psr;//private psr value
  sc_uint<16> imm, imm_ori;
  int i;

  sc_uint<16> data2inv, imminv;//inverse-bit sequence
  int lendata1, lendata2, lenresult, lenimm, lenresult_cmp;//as indicated
  sc_uint<17> mmax;//used to determine overflow, only one bit larger than 16-bit maximum
  sc_uint<16> result_cmp;//compare operation result
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

    if(control == 1){//ADD
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
        right2+=1;
        result=data1>>right2;
      }
      else
        result=data1<<imm.range(4,0);
    }
    else if(control==17){//ASH
        left_most=data1[15];
        if(data2[15]==0)
          result=data1<<data2;
        else{
          for(i=0;i<16;i++)
            right3[i]=~data2[i];
          right3 += 1;
          result =data1>>right3;
          for(i=0;i<right3;i++){
            result[15-i]=left_most;
          }
        }
    }
    else if(control == 18){//ASHI
      left_most=data1[15];
      if(imm[4]==0)
        result=data1<<imm.range(4,0);
      else{
        for(i=0;i<5;i++)
          right4[i]=~imm[i];
        right4 += 1;
        result = data1 >> right4;
        for(i=0; i<right4; i++)
          result[15-i]=left_most;
      }

    }
    else if (control == 19) { //LUI:The immediate value is shifted left 16 bits and stored in the register.
                              //The lower 16 bits are zeroes.
      result = imm << 8;
    }

    else if (control == 20) { // LOAD
      result = data2;
    }

    else if (control == 21) { //STOR
      result = data1;
    }

    else if (control == 22) { // NOP
      ;
    }

    RESULT.write(result);
    PSR.write(psr);
  }
  SC_CTOR(ALU){
    SC_METHOD(proc);
    //sensitive<<clk.pos();
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
  void proc(){
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
    sc_out<sc_uint<10> > ctrl;//RF-2bit, DM-2bit, CTRL-6bit //RF,DM r/w enable port
    //9-RF w; 8-RF r;
    //7-DM w; 6-DM r;
    //5-0 CTRL signal
    sc_out<sc_uint<4> > R_src, R_dest;
    sc_out<sc_uint<4> > R_w;
    ////R_Waddr
    /////////////////////////
    //PORTS
    /////////////////////////

    //local variables
    //displacement is defined locally， and some other decompositions
    sc_uint<16> lcl_instr;//local retrieved instructions
    //local variables that are decoded from the instructions
    //usage of each variables are indicated as their names
    sc_uint<8> Disp;//local displacement
    sc_uint<4> cond_bit;//condition bit, as indicated in Discription page_2
    sc_uint<4> opcode;//
    sc_uint<4> Rdest;//
    sc_uint<4> op_ext;//
    sc_uint<4> Rsrc;
    sc_uint<8> imm;
    sc_uint<4> psr_val;
    sc_uint<1> z;
    sc_uint<1> n;
    sc_uint<1> c;
    sc_uint<1> f;
    sc_uint<16> inter_pc;//internal program counter
    sc_uint<3> shift_op;
    //ctrl_signal is a local variables for ctrl in output ports
    //9-8 Register File Write and Read
    //7-6 Data Memory Write and Read
    //5-0 Control Signals for ALU and etc.
    sc_uint<10> ctrl_signal;
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
    //constructor
    SC_CTOR(CTRL){
      SC_METHOD(proc);
      sensitive<<Instr;
    }
};

int sc_main(int argc, char* argv[]){
  sc_signal<sc_uint<16> > instr;
  sc_signal<sc_uint<4> > psr;
  sc_signal<sc_uint<16> > rtar;
  sc_signal<sc_uint<16> > pc;
  sc_signal<sc_uint<8> > imm;
  sc_signal<sc_uint<10> > ctrl;
  sc_signal<sc_uint<4> > rdsrc, rdest, r_w;


  DP dp("dp");
  dp << instr << psr << rtar << pc << imm << ctrl << rdsrc << rdest << r_w;

  CTRL ct("ct");
  ct << instr << psr << rtar << pc << imm << ctrl << rdsrc << rdest << r_w;


  sc_start(100, SC_NS);

  return(0);
}
