

#include "alu.h"
#include "math.h"
#include "iostream"
using namespace std;

  void ALU::pipeline() {
//alu_in:
	imm_reg_in.write(IMM.read());
	control_reg_in.write(CONTROL.read());
	//cout<<sc_time_stamp()<<" "<<imm_reg_in<<" "<<control_reg_in<<" is the reg in signal"<<endl;
	d1_reg.write(D1.read());
	d2_reg.write(D2.read());
	d3_reg.write(D3.read());
	d4_reg.write(D4.read());
	wb_signal_reg_1 = WB.read();
	r_w_reg.write(R_W.read());
	d_w_reg.write(D_W.read());
	d_r_reg.write(D_R.read());
	rw_reg.write(RW.read());

//	
//alu_out:
	result_reg.write(result);
	if (mar_mux.read() == (sc_uint<1>)1) {
		MAR.write(0);
	}
	else if (mar_mux.read() == (sc_uint<1>)0){
		MAR.write(result_reg.read());
	}

	if (pc_alu_mux.read() == (sc_uint<1>)1 ) {
		RFF.write(PC.read());
	} 
	else if (pc_alu_mux.read() == (sc_uint<1>)0 ) {
		RFF.write(result_reg.read());
	}

	if (mdr_mux.read() == (sc_uint<1>)1 ) {
		MDR.write(0);
	}	
	else if (mdr_mux.read() == (sc_uint<1>)0) {

		MDR.write(result_reg.read());
	}
        r_w_reg_out.write(r_w_reg.read());
	wb_signal.write(wb_signal_reg_1);
	RW_OUT.write(rw_reg);
	R_W_OUT.write(r_w_reg_out);
	D_W_OUT.write(d_w_reg);
	D_R_OUT.write(d_r_reg);
	//cout << sc_time_stamp() << ", MAR: " << MAR << ", MDR: " << MDR << ", RFF: " << RFF << ", pc_alu_mux: " << pc_alu_mux.read() << ", mar_mux: " << mar_mux.read() << endl;
	//cout<<sc_time_stamp()<<" ,result_reg.read(): "<<result_reg.read()<<endl;		
  }


  void ALU:: proc(){
    data1 = DATA1.read();
    data2 = DATA2.read();
    control = control_reg_in.read();
    im = imm_reg_in.read();
    d1 = d1_reg.read();
    d2 = d2_reg.read();
    d3 = d3_reg.read();
    d4 = d4_reg.read();

    //define some useful lenth
    lendata1 = 16; lendata2 = 16; lenresult = 16; lenimm = 16; lenresult_cmp = 16;
    mmax = (1<<16);//comparison maximum, used for overflow and compare

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

    //get 1's complement for imm
    for(i=0;i<16;i++){
      imminv[i] = ~d4_out[i];
    }

///*
////////////////////   MUX   /////////////////
    if (d4 == (sc_uint<1>)0) {
	d4_out = imm;
    }
    else {
	d4_out = imm_ori;
    }
    if(d1 == (sc_uint<1>)0) {
	d1_out = imminv;
    }
    else {
	d1_out = d4_out;
    }
    if (d2 == (sc_uint<1>)0) {
	d2_out = data2;//d1_out;
    }
    else {
	d2_out = data2;
    }

    if (d3 == (sc_uint<1>)0) {
	d3_out = 0;
    }
    else if (d3 == (sc_uint<1>)1) {
	d3_out = data1;
    }
////////////////  MUX DONE  /////////////////
//*/

    //cout<<"error detect\n";
    if(control == (sc_uint<6>)1){//ADD
      ///overflow start
      if((mmax-d3_out) >= d2_out) {
	psr[2] = 0;
        result = d3_out + d2_out;
      }
      else {
        result = mmax;
	psr[2] = 1;
      }
        //overflow done

      //carrying manipulation
      for(i=15; i>= 0;i--){
        if(d3_out[i]==0)
          lendata1--;
        else
          break;
      }

      for(i=15;i>=0;i--){
        if(d2_out[i]==0)
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
      cout<<sc_time_stamp()<<" data1 and data2: "<<data1<<" "<<data2<<" and result for add is: "<<result<<endl;
      if(result == 0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[1] = 0;
      //psr done
    }
    else if(control == (sc_uint<6>)2){//ADDI
      //overflow start
      if((mmax-d3_out)>=imm) {
	psr[2] = 0;
        result = d3_out+imm;
      }
      else {
	psr[2] = 1;
        result=mmax;
      }
      //overflow done

      //carrying manipulation
      for(i=15;i>=0;i--){
        if(d3_out[i]==0)
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
      cout<<sc_time_stamp()<<" result for addi is:"<<result<<endl;
      //psr manipulation
      if(result == 0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[1] = 0;
      //psr manipulation
    }

    else if(control == (sc_uint<6>)3){//SUB
      //overflow detection
      if(d3_out >= d2_out){
	psr[2] = 0;
        result = d3_out - d2_out;
        //carrying manipulation
        for(i=15;i>=0;i++){
          if(d3_out[i] == 0)
            lendata1--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(d2_out[i]==0)
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
	psr[2] = 1;
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
      //psr value manipulation DONE
    }
    else if(control == (sc_uint<6>)4){//SUBI
      //overflow detection
      if(d3_out >= imm){
	psr[2] = 0;
        result = d3_out - imm;
        //carrying manipulation
        for(i=15;i>=0;i--){
          if(d3_out[i] == 0)
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
	psr[2] = 1;
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
    }
    else if (control == (sc_uint<6>)5){//CMP
      //overflow detection and manipulation
      if(d3_out>=d2_out){
	psr[2] = 0;
        result_cmp=d3_out-d2_out;
        for(i=15;i>=0;i--){
          if(d3_out[i]==0)
            lendata1--;
          else
            break;
        }
        for(i=15;i>=0;i--){
          if(d2_out[i]==0)
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
	psr[2] = 1;
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
    }
    else if (control == (sc_uint<6>)6){//CMPI
      if(d3_out>=imm){
	psr[2] = 0;
        result_cmp=d3_out-imm;
        //carrying manipulation
        for(i=15;i>=0;i--){
          if(d3_out[i]==0)
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
	psr[2] = 1;
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
    }

    else if (control == (sc_uint<6>)7){//AND
      for(i=0;i<16;i++)
        result[i]=d3_out[i] & d2_out[i];
    }
    else if (control == (sc_uint<6>)8){//ANDI
      for(i=0;i<16;i++)
        result[i]=d3_out[i] & imm[i];
    }
    else if (control == (sc_uint<6>)9){//OR
      for(i=0;i<16;i++)
        result[i]=d3_out[i] | d2_out[i];
    }
    else if (control == (sc_uint<6>)10){//ORI
      for(i=0;i<16;i++)
        result[i]=d3_out[i] | imm[i];
    }
    else if (control == (sc_uint<6>)11){//XOR
      for(i=0;i<16;i++)
        result[i]=d3_out[i] ^ d2_out[i];
    }
    else if (control == (sc_uint<6>)12){//XORI
      for(i=0;i<16;i++)
        result[i]=d3_out[i] ^ imm[i];
    }
    else if (control == (sc_uint<6>)13){//MOV
      result=d2_out;
        }
    else if (control == (sc_uint<6>)14){//MOVI
      result=imm_ori;
    }
    else if (control == (sc_uint<6>)15){//LSH
      if(d2_out[15]==1){//negative we need to take the reverse first
        for(i=0;i<16;i++)
          right1[i]=~d3_out[i];
        right1+=1;
        result=d3_out>>right1;
      }
      else{
        result=d3_out<<d2_out;
      }
    }
    else if (control == (sc_uint<6>)16){//LSHI
      if(imm[4]==1){//negative we need to take the reverse first
        for(i=0;i<4;i++)
          right2[i]=imm[i];
        result=d3_out>>right2;
      }
      else
        result=d3_out<<imm.range(3,0);
    }
    else if(control == (sc_uint<6>)17){//ASH
        left_most=d3_out[15];
        if(d2_out[15]==0)
          result=d3_out<<d2_out;
        else{
          for(i=0;i<16;i++)
            right3[i]=~d2_out[i];
          right3 += 1;
          result =d3_out>>right3;
          for(i=0;i<right3;i++){
            result[15-i]=left_most;
          }
        }
    }
    else if(control == (sc_uint<6>)18){//ASHI
      left_most=d3_out[15];
      if(imm[4]==0)
        result=d3_out<<imm.range(3,0);
      else{
        for(i=0;i<4;i++)
          right4[i]=imm[i];
        result = d3_out >> right4;
        for(i=0; i<right4; i++)
          result[15-i]=left_most;
      }

    }
    else if (control == (sc_uint<6>)19) { //LUI:The immediate value is shifted left 16 bits and stored in the register.
                              //The lower 16 bits are zeroes.
      result = imm << 8;
    }

    else if (control == (sc_uint<6>)20) { // LOAD
      result = d2_out;
    }

    else if (control == (sc_uint<6>)21) { //STOR
      result = d3_out;
    }

    else if (control == (sc_uint<6>)39) { // NOP
      ;
    }

    //cout << sc_time_stamp() << ", DATA1: " << DATA1.read() << ", DATA2: " << DATA2.read() << ", imm: " << imm_reg_in << ", contro: "<< control_reg_in << ", d1: " << d1_reg << ", d2: " << d2_reg << ", d3: " << d3_reg << ", d4: " << d4_reg << ", result: " << result << ", result_reg: " << result_reg.read() << ", pc: " << PC.read() << endl;
    PSR.write(psr);
  }	

