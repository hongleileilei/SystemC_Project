

#ifndef _SERVER_H_
#define _SERVER_H_
#include "systemc.h"

SC_MODULE(SERVER) {
    sc_in<bool> clk;
    sc_in<int> Start1, Start2, Start3;
    sc_in<int> End1, End2, End3;
    sc_in<bool> Request1, Request2, Request3;
    sc_out<int> Free, If_ok1, If_ok2, If_ok3;
    sc_out<int> Tx_helper1, Tx_helper2, Tx_helper3;
    sc_out<int> Image_packet1, Image_packet2, Image_packet3;


    int free1, free2, free3;
    int lock;
    int packetnum_becauseof_size;
    int packet_num;
    int is_sending_image_packet;



    void proc();
    void TX();
    void MOBILE1_PICTURE();
    void MOBILE2_PICTURE();
    void MOBILE3_PICTURE();

    SC_CTOR(SERVER) {
        free1 = 1;
        free2 = 1;
        free3 = 1;
        lock = 0;

        packetnum_becauseof_size = 8;///////////////////////
        packet_num = 0;/////////////////////////////////////

        SC_THREAD(proc);
        sensitive << clk.pos();
        SC_THREAD(TX);
        sensitive << clk.pos();
    }
};


#endif