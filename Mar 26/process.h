

#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "systemc.h"
#include "vector"
using namespace std;


// statuc:0--crossing, 1--stop, 2--move
SC_MODULE(PROCESS) {
private:
    int map[16][8];  // 地图是一个16行8列的数据结构, 0代表没有， grid编号从1开始
    int (*pt)[8] = NULL; // 用于初始化map, 注意二维数组的传递
    vector<int> vec_1, vec_2; //path, path从起始位置（1， 1）开始
public:
    sc_in<bool> clk;    
    sc_in<int> Receive_trigger_1, Receive_trigger_2;  // receiver对trigger敏感
    sc_in<int> If_ok_to_cross_1, If_ok_to_cross_2;

    sc_out<int> R1_trigger, R2_trigger;  //  receiver对于trigger上升延敏感
    sc_out<int> R1_status, R2_status;
    sc_out<int> Now_grid_1, Now_grid_2;
    sc_out<int> Next_grid_1, Next_grid_2;

    sc_signal<int> Flag_1, Flag_2; // tansmit对Flag敏感

    int r1_trigger, r2_trigger;
    int r1_status, r2_status;

    int if_ok_to_cross_1, if_ok_to_cross_2;

    int stop_temp_1, stop_temp_2; // 遇到obstacle发送stop， obstacle清除发送moving

    int next_grid_1, next_grid_2;
    int now_grid_1, now_grid_2;

    float obstacle_x_1, obstacle_x_2;
    float obstacle_y_1, obstacle_y_2;

    float robot_x_1, robot_x_2;
    float robot_y_1, robot_y_2;

    int robot_1_before_boundary_0_5;  //表示位， 用在robot位置更新中：若为1， 则说明 可能 需要停在boundary上
    int robot_2_before_boundary_0_5;

    int direction_1, direction_2; // 表示robot_1， robot_2 的运动方向

    int lock; // 用于去除 vector 中第一个元素
    int lock2_robot1, lock2_robot2; // 用在当robot到0.5时发送信号， 当没有接受到确认信号时就在被lock的loop里面进行地址加加

    int arrive_at_destination_1, arrive_at_destination_2; //用来判断是否到达终点

    int dir_1, dir_2; // 控制obstacle往返运动

    int return_value_1, return_value_2;


    void trim_float(float &x);
    void proc_robot_position(float &x, float &y, int &direction);
    void proc_obstacle_position(float &x, int &dir);
    int if_reach_boundary(float &x, float &y, int &robot_before_boundary_0_5, int &direction, int &now_grid, int &next_grid, int &status);
    void updata_helper(float &robot_x, float &robot_y, int &direction, int &robot_before_boundary_0_5, int &now_grid, int &next_grid, int &if_ok_to_cross, int &status, vector<int> &vec, int &arrive_at_destination, int &lock2, int &stop_temp, int &return_value);
    void updata();
    void transmit_1();
    void receive_1();
    void transmit_2();
    void receive_2();


    SC_HAS_PROCESS(PROCESS);
    PROCESS(sc_module_name process, int (*coeffs)[8], int obstacle_y_1, int obstacle_y_2, vector<int> path1, vector<int> path2) : sc_module(process), pt(coeffs), obstacle_y_1(obstacle_y_1), obstacle_y_2(obstacle_y_2), vec_1(path1), vec_2(path2) {
        obstacle_x_1 = 0; // 两个obstacle都在x方向移动
        obstacle_x_2 = 0;
        robot_x_1 = 1;
        robot_x_2 = 1;
        robot_y_1 = 1;
        robot_y_2 = 1;
        robot_1_before_boundary_0_5 = 0;
        robot_2_before_boundary_0_5 = 0;
        Flag_1 = 0;
        Flag_2 = 0;
        r1_trigger = 0;
        r2_trigger = 0;
        lock = 0;
        lock2_robot1 = 0;
        lock2_robot2 = 0;
        arrive_at_destination_1 = 0;
        arrive_at_destination_2 = 0;
        dir_1 = 0;
        dir_2 = 0;
        if_ok_to_cross_1 = 0;
        if_ok_to_cross_2 = 0;

        SC_METHOD(updata);
        sensitive << clk.pos();
        
        SC_METHOD(transmit_1);
        sensitive << Flag_1;
        SC_METHOD(receive_1);
        sensitive << Receive_trigger_1;
        SC_METHOD(transmit_2);
        sensitive << Flag_2;
        SC_METHOD(receive_2);
        sensitive << Receive_trigger_2;

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 8; j++) {
                map[i][j] = pt[i][j];
            }
        }
    }
};

#endif