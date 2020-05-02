
#ifndef _MOBILE_H_
#define _MOBILE_H_
#include "systemc.h"
#include "vector"
using namespace std;


struct X_Y {
    int x;
    int y;
};

struct IMAGE {
    //int a[7][4];
    int x;
};


struct TUPLE {
    int ROI;
    int ts;
    int te;
};

SC_MODULE(MOBILE) {
    sc_in<bool> clk;
    sc_in<int> Free; // 0代表不空闲，1代表空闲
    sc_in<int> If_ok;
    sc_in<int> Tx_helper;
    sc_in<int> Image_packet; // image的packet
    sc_out<int> Begin;
    sc_out<int> End;
    sc_out<bool> Request;

    int flag;

    sc_event e1, e2, e3;

    int read_file; // 只读一次文件
    int data[4800];
    int i; // 遍历文件用的
    int clk_num;
    int tuple_num;
    int packet_num;    // TX里面的
    int rx_packet_num; // RX里面的


    X_Y x_y;
    TUPLE tuple;
    vector<TUPLE> vec;
    int start_transmit_time;
    int request;
    int mobile_number;
    int image_1_time, image_2_time, image_3_time, image_4_time, image_5_time;

    int packetnum_becauseof_size;
    int image_num;
    int img1_lock, img2_lock, img3_lock, img4_lock, img5_lock;


    int img[5][7][4] = {50, 20, 400, 320,
                  50, 370, 450, 1000, 
                  470, 20, 600, 900,
                  670, 40, 950, 550, 
                  680, 700, 1000, 1000, 
                  -1, -1, -1, -1,
                  -1, -1, -1, -1,

                  10, 10, 150, 700, 
                  300, 10, 980, 250, 
                  300, 270, 980, 700,
                  10, 740, 950, 1000,
                  -1, -1, -1, -1, 
                  -1, -1, -1, -1,
                  -1, -1, -1, -1,

                  10, 10, 260, 900, 
                  270, 10, 520, 1000,
                  570, 20, 700, 950, 
                  730, 10, 950, 950,
                  -1, -1, -1, -1,
                  -1, -1, -1, -1,
                  -1, -1, -1, -1,

                  10, 10, 160, 1000,
                  170, 10, 220, 1000, 
                  230, 10, 380, 1000,
                  390, 10, 540, 1000,
                  550, 10, 700, 1000,
                  710, 10, 860, 1000,
                  870, 10, 1010, 1000,

                  10, 10, 1000, 160, 
                  10, 170, 1000, 220, 
                  10, 230, 1000, 380,
                  100, 390, 1000, 540,
                  10, 550, 1000, 700,
                  10, 710, 1000, 860,
                  10, 870, 1000, 1010};
 
    
    X_Y sensor();
    int conversion();
    int compress();
    void packetizer();
    void TX();
    void TX_HELPER();

    void log_file();
    void RX();
    void RX_HELPER();

    SC_HAS_PROCESS(MOBILE);

    MOBILE(sc_module_name nm, int num):sc_module(nm), mobile_number(num) {


        read_file = 0;
        i = 0;
        clk_num = 0;
        tuple_num = -1;
        packet_num = 0;
        request = 0;

        image_1_time = 20001;///////////////// x10 = 200 000ms = 200s
        image_2_time = 22000;/////////////////
        image_3_time = 22500;/////////////////
        image_4_time = 23000;/////////////////
        image_5_time = 25000;/////////////////
        img1_lock = 0;////////////////////////
        img2_lock = 0;////////////////////////
        img3_lock = 0;////////////////////////
        img4_lock = 0;////////////////////////
        img5_lock = 0;////////////////////////

        rx_packet_num = 0;/////////////////////
        packetnum_becauseof_size = 8;//////////
        image_num = 0;/////////////////////////

        SC_METHOD(packetizer);
        sensitive << clk.pos();
        SC_THREAD(TX);
        sensitive << clk.pos();
        SC_THREAD(RX);
        sensitive << clk.pos();

        SC_THREAD(RX_HELPER);
        sensitive << clk.pos();

        SC_THREAD(TX_HELPER);
        sensitive << clk.pos();

        SC_THREAD(log_file);
        sensitive << clk.pos();
        
    }
};

#endif