

#ifndef _SERVER_H_
#define _SERVER_H_
#include "systemc.h"
#include "vector"
using namespace std;


SC_MODULE(SERVER) {
public:
    sc_in<int> Trigger_1,Trigger_2;   // receive对trigger信号电平敏感
    sc_in<int> Status_1, Status_2;
    sc_in<int> Now_grid_1, Now_grid_2;
    sc_in<int> Next_grid_1, Next_grid_2;

    sc_out<int> Ack_1, Ack_2; // 1:同意穿过boundary， 0: 不同意

    sc_signal<int> Flag;

    int ack_1, ack_2;
    int map[2][3];
    int (*pt)[3] = NULL;
    vector<int> vec_1, vec_2; // 存储path 

    void transmit();
    void receive();

    SC_HAS_PROCESS(SERVER);
    SERVER(sc_module_name server, int (*coeffs)[3], vector<int> path_1, vector<int> path_2):sc_module(server), pt(coeffs), vec_1(path_1), vec_2(path_2) {
        Flag = 0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                map[i][j] = pt[i][j];
            }
        }

        SC_METHOD(transmit);
        sensitive << Flag;
        SC_METHOD(receive);
        sensitive << Trigger_1 << Trigger_2;
    }
};



#endif