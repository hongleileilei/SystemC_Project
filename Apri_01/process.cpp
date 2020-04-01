
#include "process.h"
#include "math.h"
#include "vector"
#include "string"

using namespace std;



// v_robot = 2m/s, v_obstacle = 4m/s
// clock_cycle = 0.0125 (s), 12.5 (ms) 
// robot moves 0.025 m/cycle, obstacle moves 0.05 m/cycle 


void PROCESS::trim_float(float &x) {
    string s;
    s = to_string(x);   
    s = s.substr(0, 5);
    x = stof(s);                                             
}

void PROCESS::proc_robot_position(float &x, float &y, int &direction) {
    if (direction == 4) { // 向北
        y -= 0.025;
        trim_float(y);
    } 
    else if(direction == 5) { // 向南
        y += 0.025;
        trim_float(y);
    }
    else if (direction == 6) { // 向西
        x -= 0.025;
        trim_float(x);
    }
    else if (direction == 7) { // 向东
        x += 0.025;
        trim_float(x);
    }
}

void PROCESS::proc_obstacle_position(float &x, int &dir) { // 两个obstacle都在x方西移动
   // 运动规律是往返
    if (dir == 0) {      
        x += 0.05;
        trim_float(x);
        if (x == 8) {   /////////// 注意：边界是 8m
            dir = 1;   // 反向
        } 
    }
    else {
        x -= 0.05;
        trim_float(x);
        if (x == 0) {
            dir = 0;
        } 
    }


}

int PROCESS::if_reach_boundary(float &x, float &y, int &robot_before_boundary_0_5, int &direction, int &now_grid, int &next_grid, int &status) {  // 0:大于或小于0.5m; 1:等于0.5m了; 2:now_grid == next_grid
    cout << map[7][0] << map[7][1] <<  map[7][2] <<  map[7][3] <<  map[7][4] <<  map[7][5] <<  map[7][6] <<  map[7][5] << endl; 
    for (int i = 0; i < 16; i++) {  // 寻找现在所在grid
        if (map[i][0] < x && x <= map[i][2] && map[i][1] > y && y >= map[i][3]) {
            now_grid = i + 1;
            break;
        }
    }


    if (now_grid == next_grid) { // 在结束时， 如果现在就在next_grid， 则不会发生边缘检测，需要保证robot能够停在grid的正中心。
        return 2;
    }
    else { // 若此时不在 next_grid 中
        for (direction = 4; direction < 8; direction++) {  // 与 next_grid 比较， 学找出robot的运动方向 direction
            if (map[now_grid - 1][direction] == next_grid) {
                break;
            }
        }


        if (direction == 4) { // 向北行使
            if ( y - floor(y) != 0.5 ) {  // 没到达边界前 0.5 m
                robot_before_boundary_0_5 = 0;   // 标志位
                return 0;                       
            }
            else {  // 到达边界前 0.5 m
                robot_before_boundary_0_5 = 1;
                status = 0; // 发送crossing给server
                return 1;
            }
        }

        else if (direction == 5) { // 向南行使
            if ( ceil(y) - y != 0.5 ) {
                robot_before_boundary_0_5 = 0;   // 标志位
                return 0;                       
            }
            else {  // 到达边界前 0.5 m
                robot_before_boundary_0_5 = 1;
                status = 0; // 发送crossing给server
                return 1;
            }
        }

        else if (direction == 6) { // 向西行使
            if ( ceil(x) - x != 0.5 ) {
                robot_before_boundary_0_5 = 0;   // 标志位
                return 0;                       
            }
            else {  // 到达边界前 0.5 m
                robot_before_boundary_0_5 = 1;
                status = 0; // 发送crossing给server
                return 1;
            }
        }

        else if (direction == 7) {  // 向东行使 
            if (x - floor(x) != 0.5 ) {
                robot_before_boundary_0_5 = 0;   // 标志位
                return 0;                       
            }
            else {  // 到达边界前 0.5 m
                robot_before_boundary_0_5 = 1;
                status = 0; // 发送crossing给server
                return 1;
            }
        }
    }
}


void PROCESS::updata_helper(float &robot_x, float &robot_y, int &direction, int &robot_before_boundary_0_5, int &now_grid, int &next_grid, int &if_ok_to_cross, int &status, vector<int> &vec, int &arrive_at_destination, int &lock2, int &stop_temp, int &return_value) {
    return_value = if_reach_boundary(robot_x, robot_y, robot_before_boundary_0_5, direction, now_grid, next_grid, status);

    if (arrive_at_destination == 0) { // 还没有到达path的终点

        // robot_1离两个obstacle都距离大于3    
        if ( pow( abs(obstacle_x_1 - robot_x), 2 ) + pow( abs(obstacle_y_1 - robot_y), 2) > 9 && pow( abs(obstacle_x_2 - robot_x), 2 ) + pow( abs( obstacle_y_2 - robot_y ), 2 ) > 9 )  {

            if ( stop_temp == 1) { // 发送stop之后发送moving
                status = 2;
                //Flag = 1 - Flag;
                stop_temp = 0;
            }

            cout << "asdf-------------------------------------------------------------------------------------- " << robot_before_boundary_0_5 << " " << lock2 << endl; 
            if ( robot_before_boundary_0_5 == 0 && lock2 == 0) { // 距离不等于0.5, 不用等待server信号
                status = 2; // 若是距离boundary不等于0.5m， 距离obstacle大于3m， 则一直发送 2：moving给server
                if ( 2 == return_value ) { // 说明此时robot就在目标
                    if (direction == 4) { // 北方向
                        proc_robot_position(robot_x, robot_y, direction);
                        if ( (map[now_grid-1][1] + map[now_grid-1][3]) / 2 == robot_y ) { // robot到达grid中间
                            if (vec.size() != 0) {
                                vec.erase(vec.begin());//传送下一个grid
                            }
                            else {
                                arrive_at_destination = 1;
                            }
                        }
                    }
                    else if (direction == 5) { // 南方向
                        proc_robot_position(robot_x, robot_y, direction);
                        if ( (map[now_grid-1][1] + map[now_grid-1][3]) / 2 == robot_y ) { // robot到达grid中间
                            if (vec.size() != 0) {
                                vec.erase(vec.begin());//传送下一个grid
                            }
                            else {
                                arrive_at_destination = 1;
                            }
                        }
                    }

                    else if (direction == 6) { // 西方向
                        proc_robot_position(robot_x, robot_y, direction);
                        if ( (map[now_grid-1][0] + map[now_grid-1][2]) / 2 == robot_x ) { // robot到达grid中间
                            if (vec.size() != 0) {
                                vec.erase(vec.begin());//传送下一个grid
                            }
                            else {
                                arrive_at_destination = 1;
                            }
                        }
                    }
                    else if (direction == 7) { // 东方向
                        proc_robot_position(robot_x, robot_y, direction);
                        if ( (map[now_grid-1][0] + map[now_grid-1][2]) / 2 == robot_x ) { // robot到达grid中间
                            if (vec.size() != 1) {
                                vec.erase(vec.begin());//传送下一个grid
                            }
                            else {
                                arrive_at_destination = 1;
                            }
                        }
                    }
                }
                else if( 0 == return_value ) { // 说明此时robot距离boundary不等于0.5m（或者说现在还在now_grid之中）
                    if (direction == 4) { // 北方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 5) { // 南方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 6) { // 西方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 7) { // 东方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                }
                else if ( 1 == return_value ) { // 说明此时robot距离boundary0.5m（或者说现在在now_grid, 距离next_grid边界0.5m）
                    lock2 = 0;
                    if (direction == 4) { // 北方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 5) { // 南方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 6) { // 西方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 7) { // 东方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    /////  控制输出， 发送crossing给server
                    status = 0; // 0:说明此时在0.5m处， 发送crossing， 等待ack信号
                                // 1:说明发送stop
                                // 2:stop之后发送moving
                    //1 - Flag; // transmit对flag敏感
                    /////
                }
            }
            else if (robot_before_boundary_0_5 == 1 || lock2 == 1) { // 越过了边界0.5, 等待server确认信号， 否则停在boundary
                lock2 = 1;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                if (if_ok_to_cross == 1) { // 可以通过
                    cout << "sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" << endl;
                    lock2 = 0;
                    robot_before_boundary_0_5 == 0;
                    if (direction == 4) { // 北方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 5) { // 南方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 6) { // 西方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                    else if (direction == 7) { // 东方向
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                }
                else { //不可以通过， 需要停在边界
                    if (direction == 4) { // 北方向
                        if ( robot_y != floor(robot_y) ) {
                            proc_robot_position(robot_x, robot_y, direction);
                        }
                    }
                    else if (direction == 5) { // 南方向
                        if ( robot_y != ceil(robot_y) ) {
                            proc_robot_position(robot_x, robot_y, direction);
                        } 
                    }
                    else if (direction == 6) { // 西方向
                        if ( robot_x != floor(robot_x) ) {
                            proc_robot_position(robot_x, robot_y, direction);
                        }
                    }
                    else if (direction == 7) { // 东方向
                        if ( robot_x != ceil(robot_x) ) {
                            proc_robot_position(robot_x, robot_y, direction);
                        }
                    }               
                }
            }
        }
        else { // 距离3米有obstacle， robot停止, 发送信息
            //////  控制输出
            status = 1; // 0:说明此时在0.5m处， 发送crossing， 等待ack信号
                        // 1:说明发送stop
                        // 2:stop之后发送moving 
            stop_temp = 1; // 记录现在发送了stop， 准备发送moving
            //////
        }           
    }
    else { // 到达path的destination
        cout << sc_time_stamp() << ", Arrive." << endl;
    }
}

void PROCESS::updata() {
    if (lock == 0) {  // 去处第一个元素（即 now_grid）
        lock = 1;
        vec_1.erase(vec_1.begin());
        vec_2.erase(vec_2.begin());
    }

    proc_obstacle_position(obstacle_x_1, dir_1);
    proc_obstacle_position(obstacle_x_2, dir_2);

    next_grid_1 = vec_1.front();
    next_grid_2 = vec_2.front();



    updata_helper(robot_x_1, robot_y_1, direction_1, robot_1_before_boundary_0_5, now_grid_1, next_grid_1, if_ok_to_cross_1, r1_status, vec_1, arrive_at_destination_1, lock2_robot1, stop_temp_1, return_value_1);
    updata_helper(robot_x_2, robot_y_2, direction_2, robot_2_before_boundary_0_5, now_grid_2, next_grid_2, if_ok_to_cross_2, r2_status, vec_2, arrive_at_destination_2, lock2_robot2, stop_temp_2, return_value_2);


    if (robot_1_before_boundary_0_5 == 1 || r1_status == 2 || r1_status == 1) {
        Flag_1 = 1 - Flag_1;
    }
    if (robot_2_before_boundary_0_5 == 1 || r2_status == 2 || r2_status == 1) {
        Flag_2 = 1 - Flag_2;
    }


    cout << "time: " << sc_time_stamp() << endl;
    cout << "   cg1: " << now_grid_1 <<  ", ng1: " <<next_grid_1 << ", cg2: " << now_grid_2 << ", ng2: " << next_grid_2 << ", robot1 x: " << robot_x_1 << ", robot1 y: " << robot_y_1 << ", robot2 x: " << robot_x_2 << ", robot2 y: " << robot_y_2 << endl; 
    cout << "   obstacle1 x: " << obstacle_x_1 << ", obstacle1 y: " << obstacle_y_1 << ", obstacle2 x: " << obstacle_x_2 << ", obstacle2 y: " << obstacle_y_2 << endl; 
}


void PROCESS::transmit_1() {


    R1_status.write(r1_status);
    Now_grid_1.write(now_grid_1);
    Next_grid_1.write(next_grid_1);
    R1_trigger.write(r1_trigger);

    r1_trigger = 1 - r1_trigger;   //  receive对trigger电平有效
    cout << sc_time_stamp() << ", next grid_1: " << Next_grid_1.read() << endl;
    cout << sc_time_stamp() << ", process 1 transmit to robot " << endl;
}

void PROCESS::receive_1() {
    if_ok_to_cross_1 = If_ok_to_cross_1.read(); // 1:可以通过， 0:不可以通过
    cout << sc_time_stamp() << ", ACK_1 value: " << if_ok_to_cross_1 << ", process 1 receive from robot " << endl;
}







void PROCESS::transmit_2() {


    R2_status.write(r1_status);
    Now_grid_2.write(now_grid_2);
    Next_grid_2.write(next_grid_2);
    R2_trigger.write(r2_trigger);

    r2_trigger = 1 - r2_trigger;   //  receive对trigger电平有效
    cout << sc_time_stamp() << ", next grid_2: " << Next_grid_2.read() << endl;
    cout << sc_time_stamp() << ", process 2 transmit to robot " << endl;
}

void PROCESS::receive_2() {
    if_ok_to_cross_2 = If_ok_to_cross_2.read();  // 1:可以通过， 0:不可以通过
    cout << sc_time_stamp() << ", ACK_2 value: " << if_ok_to_cross_2 << ", process 2 receive from robot " << endl;
}