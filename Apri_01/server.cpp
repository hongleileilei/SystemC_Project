

#include "server.h"
#include "vector"
using namespace std;

void SERVER::transmit() {
    trigger_out_1 = 1 - trigger_out_1;
    trigger_out_2 = 1 - trigger_out_2;
    Ack_1.write(ack_1);
    Ack_2.write(ack_2);

    Trigger_out_1.write(trigger_out_1);
    Trigger_out_2.write(trigger_out_2);
    //cout << sc_time_stamp() << ", ack1: " << Ack_1.read() << ", ack2: " << Ack_2.read() << ", server transmit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    cout << "Table in server: " << endl;
    cout << "    " << sc_time_stamp() << " robot 1 current grid: " << map[0][0] << ", next grid: " << map[0][1] << ",status: " << map[0][2] << endl;
    cout << "    " << sc_time_stamp() << " robot 2 current grid: " << map[1][0] << ", next grid: " << map[1][1] << ",status: " << map[1][2] << endl;
}


void SERVER::receive() {
    map[0][0] = Now_grid_1.read();
    map[0][1] = Next_grid_1.read();
    map[0][2] = Status_1.read();

    map[1][0] = Now_grid_2.read();
    map[1][1] = Next_grid_2.read();
    map[1][2] = Status_2.read();

    if (Status_1.read() == 0) { // robot要穿越boundary， 查看是否冲突
        ack_1 = 1; // 默认可以通过
        for (int i = 0; i < 2; i++) {
            if (map[i][0] == Next_grid_1.read()) {
                cout << "****************************************************************************" << endl;
                cout << "////////////////////////////////////////////////////////////////////////////" << endl;
                cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
                ack_1 = 0;  // 若冲突则不能通过
                break;
            }
        }
    }


    if (Status_2.read() == 0) { // robot要穿越boundary， 查看是否冲突
        ack_2 = 1;
        for (int i = 0; i < 2; i++) {
            if (map[i][0] == Next_grid_2.read()) {
                ack_2 = 0;
                break;
            }
        }
    }

    Flag = 1 - Flag;
    //cout << "server receive " << endl;
}