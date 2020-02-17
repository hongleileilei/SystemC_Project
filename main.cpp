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
    //   缺少decoder
    sc_in<sc_uint<16> > data1, data2;
    sc_in<sc_uint<5> > op;     //  调整op位数，25个操作


    sc_out<sc_uint<4> > psr;
    sc_out<sc_uint<16> > dataout;
    sc_signal<sc_uint<16> > d1 = data1.read(), d2 = data2.read();


    // 缺少op, psr, sign_ext, 补码
    // Had done the decoding and partition.
    // we can directly use operation code with 8 bits
    void prc() {
        // ADD ADDI
        if (d1.range(15:12) == 0 && d1.range(7:4) == 5 || d1.range(15:12) == 5) {
            dataout.write(d1 + d2);
        }

        // SUB SUBI
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 9 || d1.range(15:12) == 9) {
            dataout.write(d1 + d2);
        }
        // CMP CMPI
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 11 || d1.range(15:12) == 11) {
            dataout.write(d1 + d2);
        }
        // AND ANDI
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 1 || d1.range(15:12) == 1) {
            dataout.write(d1 + d2);
        }
        // OR ORI
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 2 || d1.range(15:12) == 2) {
            dataout.write(d1 + d2);
        }
        // XOR XORI
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 3 || d1.range(15:12) == 3) {
            dataout.write(d1 + d2);
        }
        // MOV MOVI
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 13 || d1.range(15:12) == 13) {
            dataout.write(d1 + d2);
        }
        // LSH LSHI
        else if (d1.range(15:12) == 8 && d1.range(7:4) == 4 || d1.range(15:12) == 8) {
            dataout.write(d1 + d2);
        }
        // ASH ASHI
        else if (d1.range(15:12) == 8 && d1.range(7:4) == 6 || d1.range(15:12) == 8) {
            dataout.write(d1 + d2);
        }
        // LUI
        else if (d1.range(15:12) == 15) {
            dataout.write(d1 + d2);
        }
        // LOAD
        else if (d1.range(15:12) == 8 && d1.range(7:4) == 0) {
            dataout.write(d1 + d2);
        }
        // STOR
        else if (d1.range(15:12) == 8 && d1.range(7:4) == 8) {
            dataout.write(d1 + d2);
        }
        // Bcond
        else if (d1.range(15:12) == 12) {
            dataout.write(d1 + d2);
        }
        //Jcond
        else if (d1.range(15:12) == 8 && d1.range(7:4) == 12) {
            dataout.write(d1 + d2);
        }
        // JAL
        else if (d1.range(15:12) == 4 && d1.range(7:4) == 8) {
            dataout.write(d1 + d2);
        }
        // NOP
        else if (d1.range(15:12) == 0 && d1.range(7:4) == 0) {
            dataout.write(d1 + d2);
        }
    }
};

SC_MODULE(MUX) {
    sc_in<sc_uint<16> > data1, data2;
    sc_in<sc_uint<1> > op;
    sc_out<sc_uint<16> > dataout;

    void prc() {
        if (op.read() == 0)
            dataout.write(data1.read());
        else if (op.read() == 1)
            dataout.write(data2.read());
    }

    SC_CTOR() {
        SC_METHOD(prc);
        sensitive << op << data1 << data2;
    }
};

    // int? unint?
    // Question: where the result is supposed to received? TOP module or ALU module
SC_MODULE(SIGN_EXT) {
    sc_in<sc_int<8> > Imm;
    sc_out<sc_int<16> > Immout;
    sc_int<8> imm = Imm.read();
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

    SC_CTOR() {
        SC_METHOD(prc);
        sensitive << Imm;
    }
};


SC_MODULE(TOP) {
    sc_in<sc_uint<16> > instr;
    sc_in<bool> clock;
    //extra output for wave
    //extra output push decoded instr to other module


    //decoding partition
    sc_uint<4> opcode;
    sc_uint<4> op_ext;
    sc_uint<4> rs;
    sc_uint<4> rd;
    sc_uint<8> im;


    //signals
    sc_out<sc_uint<1> > RF_w, RF_r, DM_w, DM_r, PM_w, PM_r;
    sc_out<sc_uint<1> > demux, mux_im, mux_ld;
    sc_out<sc_uint<4> > op;
    sc_out<sc_uint<8> > shift_op;
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
                ;
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

    void ctrl_sig(){//control signal definition
        //ALU operation signal
        op.write(op_temp);

    }


    SC_CTOR(){
        sign_ext = new SIGN_EXT("SIGN_EXT_sign_ext");
        sign_ext->Imm(im);




        sensitive<<clock;



    }
};




int sc_main(int argv, char* argc[]) {

}
