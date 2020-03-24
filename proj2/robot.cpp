

#include "robot.h"

void ROBOT::transmit_server() {
    trigger_out_to_server = 1 - trigger_out_to_server;

    Now_grid_out.write(Now_grid_in.read());
    Next_grid_out.write(Next_grid_in.read());
    Status_out.write(Status_in.read());

    Trigger_out_to_server.write(trigger_out_to_server);
}

void ROBOT::receive_process() {
    flag_transmit_server = 1 - flag_transmit_server;
}





void ROBOT::transmit_process() {
    trigger_out_to_process = 1 - trigger_out_to_process;
    Ack_out.write(Ack_in.read());
}
void ROBOT::receive_server() {
    flag_transmit_process = 1 - flag_transmit_process;
}