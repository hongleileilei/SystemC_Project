
#include "mobile.h"
#include "fstream"
#include "string"
#include "sstream"
#include "vector"
#include <stdlib.h>
#include "fstream"
using namespace std;


X_Y MOBILE::sensor() {
    if (read_file == 0)
    {
        fstream file, file2;
        stringstream ss;
        string s;
        int a;
        file.open("/home/kema/Documents/501/proj3/data.txt", ios::in);
        while(getline(file, s)) {
            ss << s;
            while(1) {
                ss >> a;
                if (ss.fail()) {
                    break;
                }
                data[i] = a;
                file2 << data[i] << endl;
                i++;
            }
            ss.str("");
            ss.clear();
        }
        read_file = 1;
        i = 0;
        file.close();
    }
    

    x_y.x = data[i];
    x_y.y = data[i+1];

    cout << sc_time_stamp() << " MOBILE" << mobile_number << " sensor:    " << x_y.x << " " << x_y.y << endl;


    if (i == 2400) {
        i = 0;
    }
    else {
        i = i + 2;
    }
    
    return x_y;
}

int MOBILE::conversion() {
    int img_num = 0;
    int find_num = clk_num % 5000;
    int i;

    if (clk_num <= image_2_time) {
        img_num = 1;
    }
    else if (clk_num <= image_3_time) {
        if (img1_lock == 0) {
            image_num--;
            img1_lock = 1;
        }
        img_num = 2;
    }
    else if (clk_num <= image_4_time) {
        if (img2_lock == 0) {
            image_num--;
            img2_lock = 1;
        }
        img_num = 3;
    }
    else if (clk_num <= image_5_time) {
        if (img3_lock == 0) {
            image_num--;
            img3_lock = 1;
        }
        img_num = 4;
    }
    else if (clk_num > image_5_time) {
        if (img4_lock == 0) {
            image_num--;
            img4_lock = 1;
        }
        img_num = 5;
    }

    X_Y x_y = sensor();

    for (i = 0; i < 7; i++) {
        if (img[img_num][i][0] <= x_y.x && img[img_num][i][2] >= x_y.x && img[img_num][i][1] <= x_y.y && img[img_num][i][3] >= x_y.y) {
            break;
        }
    }
    
    cout << sc_time_stamp() << " MOBILE" << mobile_number << " convesion: " << i + 1 << endl;

    return i + 1;
}

int MOBILE::compress() {
    int a = conversion();
    if (a < 8) { // 是有效的ROI， 不是空白区域
        if (vec.size() == 0 || vec.back().ROI != a) { // 需要插入
            tuple.ROI = a;
            tuple.ts = 10 * clk_num;
            tuple.te = 10 * (clk_num + 1);
            vec.push_back(tuple);
            tuple_num++;    
        }
        else if (vec.back().ROI == a) { // 已经存在， 不需要插入
            vec.back().te += 10;
        }

        //tuple_num++;
        
        if (tuple_num == 21) {
            tuple_num = 1;
            packet_num++;
            vec.erase(vec.begin(), vec.end());
        }  
        cout << sc_time_stamp() << " MOBILE" << mobile_number << " compress: " << endl;
        cout << sc_time_stamp() << " MOBILE" << mobile_number << " tuple_num: " << tuple_num << ", packet_num: " << packet_num << endl;  
        cout << sc_time_stamp() << " MOBILE" << mobile_number << ", ROI: " << vec[0].ROI << ", ts: " << vec[0].ts << ", te: " << vec[0].te << ", free: " << Free.read() << endl;
    }
    return packet_num;
}

void MOBILE::packetizer() {
    clk_num++;
    int a;
    if (clk_num >= 20000) {
        a = compress();
        flag = a;
        cout << sc_time_stamp() << " MOBILE" << mobile_number << " packetizer->flag: " << flag << ", a: " << a << endl;
    }
    //cout << Image_packet.read() << endl;
}

void MOBILE::TX() {
    //速度 500000次/秒， 500次/毫秒， 3*20*64 = 3840 bit, 时间3840/500 = 7.68 (ms), 7.68/10 = 1个周期
    int rand1, rand2;
    int lock = 0;
    while(1) {
        wait();
        
        if (flag > 0) {
            if ( lock == 1 && Free.read() == 1) {
                lock = 0;
            }
            while (Free.read() == 0) { // 不空闲
                rand1 = rand()%6;
                cout << sc_time_stamp() << " MOBILE" << mobile_number << ", Network is been used. Waiting for " << rand1 << " ms." << endl;
                wait(rand1, SC_MS);
                lock = 1; // 若不空闲则后面也不运行
                break;
            }
            
            if (lock == 0) {
                while(1) {
                    Request.write(1);
                    wait(e1);
                    cout << sc_time_stamp() <<" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^MOBILE" << mobile_number << ", If_ok: " << If_ok.read() << endl;

                    if (If_ok.read() ==1) { //可以发送
                        cout << endl;
                        cout << "##############################" << endl;
                        cout << sc_time_stamp() << " MOBILE" << mobile_number << " Access to network. 顺利发送" << endl;
                        cout << "##############################" << endl << endl;
                        packet_num--;
                        Begin.write(1);
                        wait();
                        End.write(1);
                        Request.write(0);
                        wait();
                        Begin.write(0);
                        End.write(0);
                        flag = 0;
                        ///// network free 后过了一个周期，才运行到这里！！！！！！！！！！！！！ 有可能需要修改
                        //cout << sc_time_stamp() << "*************************************************************************************************" << endl;
                        break;
                    }
                    else {
                        rand2 = rand()%6;
                        cout << sc_time_stamp() << " MOBILE" << mobile_number << " Request denied. Network is been using. Waiting for " << rand2 << " ms." << endl;
                        wait(rand2, SC_MS);
                        break;
                    } 
                }
            }
        }
    }
}

void MOBILE::TX_HELPER() {
    while(1) {
        wait();
        if (Tx_helper.read() == 1) {
            e1.notify();
            cout << "Received at MOBILE" << mobile_number << ". If_ok to transmit: " << If_ok.read() << " at " << sc_time_stamp() << endl;
            //cout << sc_time_stamp() <<" MOBILE" << mobile_number << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << Tx_helper.read() << " " << If_ok.read() << " " << Request.read() << endl;
        }
    }
}



void MOBILE::RX() {
    while(1) {
        wait();
        wait(e2);
        wait(e3);
        rx_packet_num++;
        if (rx_packet_num == packetnum_becauseof_size) {
            image_num++;
            //cout << image_num << endl;
            //sc_stop();
            cout << endl << endl << endl;
            cout << image_num << endl << endl << endl;
            rx_packet_num = 0;
        }
    }
}

void MOBILE::RX_HELPER() {
    while(1) {
        wait();
        //cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
        //cout << sc_time_stamp() << " " << Image_packet.read() << endl;
        if (Image_packet.read() == 1) {
            e2.notify();
            //cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << endl;
            while(1) {
                wait();
                if (Image_packet.read() == 0) {
                    e3.notify();
                    cout << "Mobile" << mobile_number << " has received one packet at " << sc_time_stamp() << endl;
                    cout << sc_time_stamp() << " ###############################################" << endl;
                    break;
                }
            }
        }
    }
}

void MOBILE::log_file() {
    fstream file1, file2, file3;
    file1.open("./mobile1.txt", ios::out);
    file2.open("./mobile2.txt", ios::out);
    file3.open("./mobile3.txt", ios::out);

    while(1) {
        wait();
        if (mobile_number == 1) {
            file1 << tuple_num <<  " " << packet_num << " " << rx_packet_num << " " << image_num << " " << sc_time_stamp() << endl;
        }
        if (mobile_number == 2) {
            file2 << tuple_num <<  " " << packet_num << " " << rx_packet_num << " " << image_num << " " << sc_time_stamp() << endl;
        }
        if (mobile_number == 3) {
            file3 << tuple_num <<  " " << packet_num << " " << rx_packet_num << " " << image_num << " " << sc_time_stamp() << endl;
        }
        
    }

}