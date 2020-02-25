

#include "alu.h"
#include "math.h"
#include "iostream"
using namespace std;

  void ALU:: proc(){
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
      if((mmax-data1) >= data2) {
	psr[2] = 0;
        result = data1 + data2;
      }
      else {
        result = mmax;
	psr[2] = 1;
      }
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
      //psr done
    }
    else if(control == 2){//ADDI
      //overflow start
      if((mmax-data1)>=imm) {
	psr[2] = 0;
        result = data1+imm;
      }
      else {
	psr[2] = 1;
        result=mmax;
      }
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
      //psr manipulation
    }

    else if(control==3){//SUB
      //overflow detection
      if(data1 >= data2){
	psr[2] = 0;
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
      if(data1 >= imm){
	psr[2] = 0;
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
      if(data1>=data2){
	psr[2] = 0;
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
      if(data1>=imm){
	psr[2] = 0;
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
        for(i=0;i<4;i++)
          right2[i]=imm[i];
        result=data1>>right2;
      }
      else
        result=data1<<imm.range(3,0);
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
        result=data1<<imm.range(3,0);
      else{
        for(i=0;i<4;i++)
          right4[i]=imm[i];
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
