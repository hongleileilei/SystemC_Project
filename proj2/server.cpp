

#include "server.h"
#include "vector"
using namespace std;

void SERVER::transmit() {
    Ack_1.write(ack_1);
    Ack_2.write(ack_2);
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
}