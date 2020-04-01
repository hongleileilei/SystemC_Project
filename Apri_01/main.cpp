

#include "process.h"
#include "robot.h"
#include "server.h"
#include "vector"
using namespace std;

int sc_main(int a, char* b[]) {
    sc_clock clk("clk", 12.5, SC_MS, 0.5, 12.5, SC_MS, true);

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

    // 地图是 16x8
    int map[16][8] = {{0, 2, 2, 0, 0, 5, 0, 2},
                     {2, 2, 4, 0, 0, 6, 1, 3}, 
                     {4, 2, 6, 0, 0, 7, 2, 4}, 
                     {6, 2, 8, 0, 0, 8, 3, 0}, 
                     
                     {0, 4, 2, 2, 1, 9, 0, 6}, 
                     {2, 4, 4, 2, 2, 10, 5, 7}, 
                     {4, 4, 6, 2, 3, 11, 6, 8}, 
                     {6, 4, 8, 2, 4, 12, 7, 0},
                     
                     {0, 6, 2, 4, 5, 13, 0, 10}, 
                     {2, 6, 4, 4, 6, 14, 9, 11}, 
                     {4, 6, 6, 4, 7, 15, 10, 12}, 
                     {6, 6, 8, 4, 8, 16, 11, 0},

                     {0, 8, 2, 6, 9, 0, 0, 14}, 
                     {2, 8, 4, 6, 10, 0, 13, 15}, 
                     {4, 8, 6, 6, 11, 0, 14, 16}, 
                     {6, 8, 8, 6, 12, 0, 15, 0}};
    // p1指针指向地图
    int (*p1)[8] = map;

    int map_server[2][3] = {0, 0, 0,
                            0, 0, 0 };
    int (*p2)[3] = map_server;
    vector<int> path_1, path_2;
    path_1.push_back(1);
    path_1.push_back(2);
    path_1.push_back(6);
    path_1.push_back(7);
    path_1.push_back(3);
    path_1.push_back(2);

    path_2.push_back(1);
    path_2.push_back(2);


    PROCESS process("process", p1, 6, 7, path_1, path_2);
    process(clk, rtp_trig_1, rtp_trig_2, rtp_ack_1, rtp_ack_2, ptr_trig_1, ptr_trig_2, ptr_r1_status, ptr_r2_status, ptr_cg_1, ptr_cg_2, ptr_ng_1, ptr_ng_2);

    ROBOT r1("r1");
    r1(ptr_trig_1, str_trig_1, ptr_r1_status, ptr_cg_1, ptr_ng_1, str_ack_1, rtp_ack_1, rts_cg_1, rts_ng_1, rts_trig_1, rtp_trig_1, rts_status_1);
    ROBOT r2("r2");
    r2(ptr_trig_2, str_trig_2, ptr_r2_status, ptr_cg_2, ptr_ng_2, str_ack_2, rtp_ack_2, rts_cg_2, rts_ng_2, rts_trig_2, rtp_trig_2, rts_status_2);

    SERVER server("server", p2, path_1, path_2);
    server(rts_trig_1, rts_trig_2, rts_status_1, rts_status_2, rts_cg_1, rts_cg_2, rts_ng_1, rts_ng_2, str_ack_1, str_ack_2, str_trig_1, str_trig_2);

    sc_start(4400, SC_MS);


    return 0;
}