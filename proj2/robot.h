
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "systemc.h"


SC_MODULE(ROBOT) {
    sc_in<int> Trigger_in_process;
    sc_in<int> Trigger_in_server;
    sc_in<int> Status_in;
    sc_in<int> Now_grid_in;
    sc_in<int> Next_grid_in;
    sc_in<int> Ack_in;

    sc_out<int> Ack_out;
    sc_out<int> Now_grid_out;
    sc_out<int> Next_grid_out;
    sc_out<int> Trigger_out_to_server;
    sc_out<int> Trigger_out_to_process;
    sc_out<int> Status_out;

    sc_signal<int> flag_transmit_server;
    sc_signal<int> flag_transmit_process;

    int trigger_out_to_server;
    int trigger_out_to_process;

    void transmit_server();
    void receive_server();

    void transmit_process();
    void receive_process();

    SC_CTOR(ROBOT) {
        trigger_out_to_server = 0;
        trigger_out_to_process = 0;
        flag_transmit_server = 0;
        flag_transmit_process = 0;

        SC_METHOD(transmit_server);
        sensitive << flag_transmit_server;
        SC_METHOD(receive_process);
        sensitive << Trigger_in_process;

        SC_METHOD(transmit_process);
        sensitive << flag_transmit_process;
        SC_METHOD(receive_server);
        sensitive << Trigger_in_server;
    }
};


#endif