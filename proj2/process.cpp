
#include "process.h"
#include "math.h"
using namespace std;



// v_robot = 2m/s, v_obstacle = 4m/s
// clock_cycle = 0.0125 (s), 12.5 (ms) 
// robot moves 0.025 m/cycle, obstacle moves 0.05 m/cycle 

void PROCESS::proc_robot_position(int &x, int &y, int &direction) {
    if (direction == 4) { // 向北
        y -= 0.025;
    } 
    else if(direction == 5) { // 向南
        y += 0.025;
    }
    else if (direction == 6) { // 向西
        x -= 0.025;
    }
    else if (direction == 7) { // 向东
        x += 0.025;
    }
}

void PROCESS::proc_obstacle_position(int &x) { // 两个obstacle都在x方西移动
    int dir = 0;
    if (dir = 0) {      
        x += 0.05;
        if (x == 8) {   /////////// 注意：边界是 8m
            dir = 1;   // 反向
        } 
    }
    else {
        x -= 0.05;
        if (x == 0) {
            dir = 0;
        } 
    }
     
}

int PROCESS::if_reach_boundary(int &x, int &y, int &robot_before_boundary_0_5, int &direction, int &now_grid, int &next_grid) {  // 0:大于或小于0.5m; 1:等于0.5m了; 2:now_grid == next_grid

    for (int i = 0; i < 8; i++) {  // 寻找现在所在grid
        if (map[i][0] <= x <= map[i][2] && map[i][1] <= y <= map[i][3]) {
            now_grid = i;
            break;
        }
    }


    if (now_gird == next_grid) { // 在结束时， 如果现在就在next_grid， 则不会发生边缘检测，需要保证robot能够停在grid的正中心。
        return 2;
    }
    else { // 若此时不在 next_grid 中
        for (int direction = 4; dircetion < 8; direction++) {  // 与 next_grid 比较， 学找出robot的运动方向 direction
            if (map[now_grid][direction] == next_grid) {
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
                return 1;
            }
        }

        else if (direction == 6) { // 向西行使
            if ( x - ceil(x) != 0.5 ) {
                robot_before_boundary_0_5 = 0;   // 标志位
                return 0;                       
            }
            else {  // 到达边界前 0.5 m
                robot_before_boundary_0_5 = 1;
                return 1;
            }
        }

        else if (direction == 7) {  // 向东行使
            if ( floor(x) - x != 0.5 ) {
                robot_before_boundary_0_5 = 0;   // 标志位
                return 0;                       
            }
            else {  // 到达边界前 0.5 m
                robot_before_boundary_0_5 = 1;
                return 1;
            }
        }
    }
}


void PROCESS::updata_helper(int &robot_x, int &robot_y, int &direction, int &robot_before_boundary_0_5, int &now_grid, int &next_grid, int &if_ok_to_cross, int &status) {
    int return_value = if_reach_boundary(robot_x, robot_y, robot_before_boundary_0_5, direction, now_grid, next_grid);

    // robot_1离两个obstacle都距离大于3    
    if ( pow( abs(obstacle_x_1 - robot_x), 2 ) + pow( abs(obstacle_y_1 - robot_y), 2) > 9 && pow( abs(obstacle_x_2 - robot_x), 2 ) + pow( abs( obstacle_y_2 - robot_y ), 2 ) > 9 )  {
        if ( stop_temp == 1) { // 发送stop之后发送moving
            status = 2;
            flag = ~flag;
            Flag.write(flag);
            stop_temp = 0;
        }

        if (robot_before_boundary_0_5 == 0) { // 还没有到达0.5, 不用等待server信号
            if ( 2 == return_value ) { // 说明此时robot就在目标
                if (direction == 4) { // 北方向
                    proc_robot_position(robot_x, robot_y, direction);
                    if ( floor(y) == y + 1 ) { // robot到达grid中间
                        ;//传送下一个grid
                    }
                }
                else if (direction == 5) { // 南方向
                    proc_robot_position(robot_x, robot_y, direction);
                    if ( floor(y) == y + 1 ) { // robot到达grid中间
                        ;//传送下一个grid
                    }
                }

                else if (direction == 6) { // 西方向
                    proc_robot_position(robot_x, robot_y, direction);
                    if (floor(x) == x + 1) { // robot到达grid中间
                        ;//传送下一个grid
                    }
                }
                else if (direction == 7) { // 东方向
                    proc_robot_position(robot_x, robot_y, direction);
                    if (floor(x) == x + 1) { // robot到达grid中间
                        ;//传送下一个grid
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
                flag = ~flag;
                Flag.write(flag); // transmit对flag敏感
                /////
            }
        }
        else if (robot_before_boundary_0_5 == 1) { // 越过了边界0.5, 等待server确认信号， 否则停在boundary
            if (if_ok_to_cross == 1) { // 可以通过
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
                    if ( robot_y != ceil(robot_y) ) {
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                }
                else if (direction == 5) { // 南方向
                    if ( robot_y != floor(robot_y) ) {
                        proc_robot_position(robot_x, robot_y, direction);
                    } 
                }
                else if (direction == 6) { // 西方向
                    if ( robot_x != ceil(robot_x) ) {
                        proc_robot_position(robot_x, robot_y, direction);
                    }
                }
                else if (direction == 7) { // 东方向
                    if ( robot_x != floor(robot_x) ) {
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
        flag = ~flag;
        Flag.write(flag);
        stop_temp = 1; // 记录现在发送了stop， 准备发送moving
        //////
    }
}

void PROCESS::updata() {
    proc_obstacle_position(obstacle_x_1);
    proc_obstacle_position(obstacle_x_2);

    next_grid_1 = Next_grid_1.read();
    next_grid_2 = Next_grid_2.read();

    updata_helper(robot_x_1, robot_y_1, direction_1, robot_1_before_boundary_0_5, now_grid_1, next_grid_1, if_ok_to_cross_1, r1_status);
    updata_helper(robot_x_2, robot_y_2, direction_2, robot_2_before_boundary_0_5, now_grid_2, next_grid_2, if_ok_to_cross_2, r2_status);

}



void PROCESS::transmit() {
    R1_trigger.write(r1_trigger);   //////////可能有问题， 在刚开始时候trigge信号值是什么不知道
    R2_trigger.write(r2_trigger);   //////////可能有问题

    R1_status.write(r1_status);
    R2_status.write(r1_status);
    r1_trigger = ~r1_trigger;
    r2_trigger = ~r2_trigger;
    R1_trigger.write(r1_trigger);
    R2_trigger.write(r2_trigger);
}

void PROCESS::receive() {
    if_ok_to_cross_1 = If_ok_to_cross_1.read(); // 1:可以通过， 0:不可以通过
    if_ok_to_cross_2 = If_ok_to_cross_2.read();
}