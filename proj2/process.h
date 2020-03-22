

#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "systemc.h"


// statuc:0--crossing, 1--stop, 2--move
SC_MODULE(PROCESS) {
public:
    sc_in<bool> clk;    
    sc_in<int> Trigger;  // receiver对trigger敏感
    sc_in<int> Next_grid_1, Next_grid_2;
    sc_in<int> If_ok_to_cross_1, If_ok_to_cross_2;

    sc_out<int> R1_trigger, R2_trigger;  //  receiver对于trigger上升延敏感
    sc_out<int> R1_status, R2_status;

    sc_signal<int> Flag; // tansmit对Flag敏感

    bool flag;

    int r1_trigger, r2_trigger;
    int r1_status, r2_status;

    int if_ok_to_cross_1, if_ok_to_cross_2;

    int stop_temp; // 遇到obstacle发送stop， obstacle清除发送moving

    int next_grid_1, next_grid_2;
    int now_grid_1, now_grid_2;

    int map[8][8];  // 地图是一个8行8列的数据结构, 0代表没有， grid编号从1开始
    int* pt = NULL; // 用于初始化map

    int obstacle_x_1, obstacle_x_2;
    int obstacle_y_1, obstacle_y_2;

    int robot_x_1, robot_x_2;
    int robot_y_1, robot_x_2;

    int robot_1_before_boundary_0_5;  //表示位， 用在robot位置更新中：若为1， 则说明 可能 需要停在boundary上
    int robot_2_before_boundary_0_5;

    int direction_1, direction_2; // 表示robot_1， robot_2 的运动方向



    void proc_robot_position(int &x, int &y, int &direction);
    void proc_obstacle_position(int &x);
    int if_reach_boundary(int &x, int &y, int &robot_before_boundary_0_5, int &direction, int &now_grid, int &next_grid)
    void updata_helper(int &robot_x, int &robot_y, int &direction, int &robot_before_boundary_0_5, int &now_grid, int &next_grid, int &if_ok_to_cross, int &status);
    void updata();
    void transmite();
    void receive();
    SC_HAS_PROCESS(PROCESS);
    ROBOT(sc_module_name process, const int* coeffs, int obstacle_y_1, int obstacle_y_2) : sc_module(process), pt(coeffs), obstacle_y_1(obstacle_y_1, obstacle_y_2(obstacle_y_2)) {
        obstacle_x_1 = 0; // 两个obstacle都在x方向移动
        obstacle_x_2 = 0;
        robot_x = 0;
        robot_y = 0;
        rotot_1_before_boundary_0_5 = 0;
        rotot_2_before_boundary_0_5 = 0;
        flag = 0;
        r1_trigger = 0;
        r2_trigger = 0;

        SC_METHOD(updata);
        sensitive << clk.pos();
        SC_METHOD(transmit);
        sensitive << Flag;
        SC_METHOD(receive);
        sensitive << Trigger.pos();

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                map[i][j] = pt[i][j];
            }
        }
    }
};

#endif