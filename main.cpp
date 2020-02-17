#include "systemc.h"


SC_MODULE(DM) {
    sc_in<sc_uint<16> > Addr;
    sc_inout<sc_uint<16> > Data;
    sc_in<sc_uint<1> > wen, ren;

    sc_uint<16> mem[1<<16];

    void prc() {
        sc_uint<16> addr = Addr.read();
        if (wen.read() == 1)
            mem[addr] = Data.read();
        else if (ren.read() == 1)
            Data.write(mem[addr]);
    }

    SC_CTOR(DM) {
        for (int i = 0; i < (1<<16); i++){
            mem[0] = 0;
        }
        SC_METHOD(prc);
        sensitive << wen << ren;
    }
};

SC_MODULE(PM) {
    sc_in<sc_uint<16> >
};

SC_MODULE(RF) {
    sc_in<bool> clk;
    sc_in<sc_uint<4> > Addr1, Addr2, Addr3;
    sc_in<sc_uint<1> > wen, ren;
    sc_out<sc_uint<16> > Data1, Data2;
    sc_in<sc_uint<16> > Data3;

    sc_uint<16> mem[1<<4];

    void prc() {
        if (wen.read() == 1) {
            mem[Addr3.read()] = Data3.read();
        }
        else if (ren.read() == 1) {
            Data1.write(mem[Addr1.read()]);
            Data2.write(mem[Addr2.read()]);
        }
    }
    SC_CTOR(RF) {
        for (int i = 0; i < (1<<4); i++)
        {
            mem[i] = 0;
        }
        SC_METHOD(prc);
        sensitive << clk.pos();
    }
};


SC_MODULE(ALU) {
    //   MUX在TOP模块里面实现
    sc_in<sc_uint<16> > data1, data2;
    sc_in<sc_uint<3> > op;     //  调整op位数，25个操作
    sc_out<sc_uint<4> > psr;
    sc_out<sc_uint<16> > dataout;    
    sc_uint<16>  d1 = data1.read(), d2 = data2.read();


    // 缺少op, psr, 补码 

};

SC_MODULE(MUX) {
    sc_in<sc_int<16> > data1, data2;
    sc_in<sc_int<1> > op;
    sc_out<sc_uint<16> > dataout1;
    sc_out<>

    void prc() {
        if (op.read() == 1)
            dataout.write(data1.read());
        else if (op.read() == 0)
            dataout.write(data2.read());
    }

    SC_CTOR() {
        SC_METHOD(prc);
        sensitive << op << data1 << data2;
    }
};


SC_MODULE(DEMUX) {
    sc_in<sc_uint<1> > op; 
    sc_in<sc_uint<16> > in;
    sc_out<sc_uint<16> > out1, out2;

    void prc() {
        if (op == 1)
        {
            out1.write(in.read());
            out1.write(0);
        } 
            
        else if (op == 0)
        {
            out1.write(0);
            out2.write(in.read());
        }
    }
    SC_CTOR(DEMUX) {
        SC_METHOD(prc);
        sensitive << in << op;
    }
};

    // int? unint?
SC_MODULE(SIGN_EXT) {
    sc_in<sc_uint<8> > Imm;
    sc_out<sc_int<16> > Immout;
    sc_uint<8> imm = Imm.read();
    sc_int<16> immout;
    
    void prc() {
        for (int i = 8; i < 16; i++) {
            immout[i] = imm[7];
        }
        for (int j = 0; j < 8; j++) {
            immout[j] = imm[j];
        }
        Immout.write(immout);
    }

    SC_CTOR(SIGN_EXT) {
        SC_METHOD(prc);
        sensitive << Imm;
    }
};


SC_MODULE(DFF) {
    sc_in<bool> clk;
    sc_in<sc_uint<16> > datain;
    sc_out<sc_uint<16> > dataout;
    void prc() {
        dataout.write(datain.read());
    }

    SC_CTOR(DFF) {
        SC_METHOD(prc);
        sensitive << clk.pos();
    }
};

// 两个DFF串联
SC_MODULE(DELAY_2) {
    sc_in<bool> clk;
    sc_in<sc_uint<1> > datain;
    sc_out<sc_out<1> > dataout;
    sc_signal<sc_uint<1> > ss;

    void prc() {}
    
    SC_CTOR(DELAY_2) {
        DFF d1("d1"), d2("d2"); 
        d1 << clk << datain << ss;
        d2 << clk << ss << dataout; 
        SC_METHOD(prc);
        sensitive << clk;
    }
};

// 3个DFF串联
SC_MODULE(DELAY_3) {
    sc_in<bool> clk
    sc_in<sc_uint<1> > datain;
    sc_out<sc_uint<1> > dataout;
    sc_signal<sc_uint<1> > s1, s2;

    void prc() {}

    SC_CTOR(DELAY_3) {
        DFF d1("d1"), d2("d2"), d3("d3");
        d1 << clk << datain << s1;
        d2 << clk << s1 << s2;
        d3 << clk << s2 << dataout; 
        SC_METHOD(prc);
        sensitive << clk.pos();
    }
};

SC_MODULE(DECODER) {
    sc_in<bool> clock;
    sc_in<sc_uint<16> > instr;

    sc_out<sc_uint<1> > RF_w, RF_r, DM_w, DM_r, PM_w, PM_r;
    sc_out<sc_uint<1> > demux, mux_im, mux_ld, mux_wr;  // 三个MUX， 一个imm， 一个load， 最后一个用在writeback
    sc_out<sc_uint<3> > op;         // 加法一类（0），减法比较一类（1），mov load一类（2），移位一类（3）：暂定4类
    sc_out<sc_uint<4> > Rs, Rd;
    sc_out<sc_uint<4> > flag;
    //sc_out<sc_uint<8> > shift_op;  没必要
    
    //decoding partition
    sc_uint<4> opcode;
    sc_uint<4> op_ext;
    sc_uint<4> rs;
    sc_uint<4> rd;
    sc_uint<8> im;

    //inner temp
    sc_uint<4> op_temp;//using for temprarily store the operation

    //calling for different modules
    SIGN_EXT* sign_ext;
    void decode(){
        opcode = instr.rang(15,12);
        rd = instr.range(11,8);
        op_ext = instr.range(7,4);
        rs = instr.range(3,0);
        im = instr.range(7,0);

        
        ///////////////////////

        if(opcode == 0){
            //add sub cmp or xor mov nop
            if(op_ext == 5){//add
                ///////   重点注意RF_w信号  /////////
                RF_w.write(0); RF_r.write(1);
                //////////////////////////////////// 
                mux_ld.write(1); mux_im.write(1);  
                op.write(0);     demux.write(0);
                DM_w.write(0);   DM_r.write(0);
                mux_wr.write(0);  // 每个操作以此类推
            }
            else if(op_ext == 9){//sub
                ;
            }
            else if(op_ext == 11){//cmp
                ;
            }
            else if(op_ext == 1){//and
                ;
            }
            else if(op_ext == 2){//or
                ;
            }
            else if(op_ext == 3){//xor
                ;
            }
            else if(op_ext == 13){//mov
                ;
            }
            else if(op_ext == 0){//NOP
                ;
            }
        }
        else if(opcode == 4){
            if(op_ext == 0){//load
                ;
            }
            else if(op_ext == 4){//stor
                ;
            }
            else if(op_ext == 12){//Jcond
                ;
            }
            else if(op_ext == 8){//JAL
                ;
            }
            //load/write/jump
        }
        else if(opcode == 8){
            if(op_ext == 4){//lsh
                ;
            }
            else if(op_ext == 6){//ash
                ;
            }
            else if(op_ext == (sc_uint<4>) "0b000s"){//lshi
                ;
            }
            else if(op_ext == (sc_uint<4>) "0boo1s"){//ashi
                ;
            }
            //shift (either regular or imme)
        }
        else if(opcode == 12){
            //Bcond
            ;
        }
        else{
            if(opcode == 5){//add
                ;
            }
            else if(opcode == 9){//sub
                ;
            }
            else if(opcode == 11){//cmp
                ;
            }
            else if(opcode == 1){//and
                ;
            }
            else if(opcode == 2){//or
                ;
            }
            else if(opcode == 3){//xor
                ;
            }
            else if(opcode == 13){//mov
                ;
            }
            else if(opcode == 0){//NOP
                ;
            }
            else if(opcode == 15){//LUI
                
            }
            //LUI or immediate
        }
        ////////////////////////
    }




};



SC_MODULE(TOP) {

};


int sc_main(int argv, char* argc[]) {
    sc_clock clk("clk", 10, SC_NS, 0.5, 10, SC_NS, true); // 开始10秒低电平

}
