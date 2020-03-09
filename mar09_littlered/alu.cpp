

#include "alu.h"
#include "math.h"
#include "iostream"
using namespace std;

  void ALU:: proc(){
    //rtar_temp=rtar_in.read();
    data1 = DATA1.read();//Rdest
    data2 = DATA2.read();//Rsrc,Ramt,Raddr
    control = CONTROL.read();//read from DP
    im = IMM.read();//read from DP
    d1 = D1.read();
    d2 = D2.read();
    d3 = D3.read();
    d4 = D4.read();
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

/*
////////////////////   MUX   /////////////////
    if (d4 == 0) {
	d4_out = imm;
    }
    else {
	d4_out = imm_ori;
    }
    if(d1 == 0) {
	d1_out = imminv;
    }
    else {
	d1_out = d4_out;
    }
    if (d2 == 0) {
	d2_out = d1_out;
    }
    else {
	d2_out = data2;
    }
    if (d3 == 0) {
	d3_out = 0;
    }
    else (d3 == 1) {
	d3_out = data1;
    }
////////////////  MUX DONE  /////////////////
*/


    if(control == 1){//ADD
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

      if(result == 0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[1] = 0;
      //psr done
    }
    else if(control == 2){//ADDI
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

      //psr manipulation
      if(result == 0)
        psr[3] = 1;
      else
        psr[3] = 0;
      psr[1] = 0;
      //psr manipulation
    }

    else if(control==3){//SUB
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
    else if(control == 4){//SUBI
      //overflow detection
      if(d3_out >= imm){
	psr[2] = 0;
        result = d3_out - imm;
        //carrying manipulation
        for(i-15;i>=0;i--){
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
    else if (control==5){//CMP
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
    else if (control==6){//CMPI
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

    else if (control==7){//AND
      for(i=0;i<16;i++)
        result[i]=d3_out[i] & d2_out[i];
    }
    else if (control==8){//ANDI
      for(i=0;i<16;i++)
        result[i]=d3_out[i] & imm[i];
    }
    else if (control==9){//OR
      for(i=0;i<16;i++)
        result[i]=d3_out[i] | d2_out[i];
    }
    else if (control==10){//ORI
      for(i=0;i<16;i++)
        result[i]=d3_out[i] | imm[i];
    }
    else if (control==11){//XOR
      for(i=0;i<16;i++)
        result[i]=d3_out[i] ^ d2_out[i];
    }
    else if (control==12){//XORI
      for(i=0;i<16;i++)
        result[i]=d3_out[i] ^ imm[i];
    }
    else if (control==13){//MOV
      result=d2_out;
        }
    else if (control==14){//MOVI
      result=imm_ori;
    }
    else if (control==15){//LSH
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
    else if (control==16){//LSHI
      if(imm[4]==1){//negative we need to take the reverse first
        for(i=0;i<4;i++)
          right2[i]=imm[i];
        result=d3_out>>right2;
      }
      else
        result=d3_out<<imm.range(3,0);
    }
    else if(control==17){//ASH
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
    else if(control == 18){//ASHI
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
    else if (control == 19) { //LUI:The immediate value is shifted left 16 bits and stored in the register.
                              //The lower 16 bits are zeroes.
      result = imm << 8;
    }

    else if (control == 20) { // LOAD
      result = d2_out;
    }

    else if (control == 21) { //STOR
      result = d3_out;
    }

    else if (control == 22) { // NOP
      ;
    }

    MAR.write(result);
    MDR.write(result);
	  RF.write(result);
    PSR.write(psr);
    rtar_out.write(data2);
  }
