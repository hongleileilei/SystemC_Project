

#include "process.h"
#include "robot.h"
#include "server.h"
#include "vector"
using namespace std;

int sc_main(int a, char* b[]) {
    sc_clock clk("clk", 10, SC_NS);

    sc_signal<int> ptr_trig_1, ptr_trig_2; // process to robot trigger signal
    sc_signal<int> ptr_cg_1, ptr_cg_2; // process to robot now grid
    sc_signal<int> ptr_ng_1, ptr_ng_2; // process to robot next grid
    sc_signal<int> ptr_r1_status, ptr_r2_status; // process to robot

    sc_signal<int> str_ack_1, str_ack_2; // sever to robot
    sc_signal<int> str_trig_1, str_trig_2;
 
    sc_signal<int> rts_status_1, rts_status_2; // robot to server
    sc_signal<int> rts_cg_1, rts_cg_2; // robot to server now grid
    sc_signal<int> rts_ng_1, rts_ng_2; // robot to server next grid
    sc_signal<int> rts_trig_1, rts_trig_2;

    sc_signal<int> rtp_trig_1, rtp_trig_2; // robot to process trigger signal
    sc_signal<int> rtp_ack_1, rtp_ack_2;


    int map[8][8] = {0};
    int (*p1)[8] = map;
    int map_server[2][3] = {0};
    int (*p2)[3] = map_server;
    vector<int> path_1, path_2;
    path_1.push_back(0);
    path_2.push_back(0);


    PROCESS process("process", p1, 0, 0, path_1, path_2);
    process(clk, rtp_trig_1, rtp_trig_2, rtp_ack_1, rtp_ack_2, ptr_trig_1, ptr_trig_2, ptr_r1_status, ptr_r2_status, ptr_cg_1, ptr_cg_2, ptr_ng_1, ptr_ng_2);

    ROBOT r1("r1");
    r1(ptr_trig_1, str_trig_1, ptr_r1_status, ptr_cg_1, ptr_ng_1, str_ack_1, rtp_ack_1, rts_cg_1, rts_ng_1, rts_trig_1, rtp_trig_1, rts_status_1);
    ROBOT r2("r2");
    r2(ptr_trig_2, str_trig_2, ptr_r2_status, ptr_cg_2, ptr_ng_2, str_ack_2, rtp_ack_2, rts_cg_2, rts_ng_2, rts_trig_2, rtp_trig_2, rts_status_2);

    SERVER server("server", p2, path_1, path_2);
    server(rts_trig_1, rts_trig_2, rts_status_1, rts_status_2, rts_cg_1, rts_cg_2, rts_ng_1, rts_ng_2, str_ack_1, str_ack_2);

    sc_start(100, SC_NS);



    return 0;
}