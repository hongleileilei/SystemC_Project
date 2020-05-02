
#include "server.h"

void SERVER::proc() {
    while(1) {
        wait();
        cout << endl;
        //cout << sc_time_stamp() << ", SERVER SERVER SERVER SERVER SERVER SERVER SERVER SERVER SERVER SERVER " << endl;
        cout << sc_time_stamp() << ", At server side." << endl;
        cout << sc_time_stamp() << ", Rqeuest1: " << Request1.read() << ", Request2: " << Request2.read() << ", Request3: " << Request3.read() << ", Free: " << Free.read() << endl;
        cout << sc_time_stamp() << ", If_ok1: " << If_ok1.read() << ", If_ok2: " << If_ok2.read() << ", If_ok3: " << If_ok3.read() << endl << endl;
        Tx_helper1.write(0); Tx_helper2.write(0); Tx_helper3.write(0);


        if (is_sending_image_packet == 0) { // 没有发送的时候才进行 phase1 的操作
            Free.write(1);
            if (Request1.read() == 1) {
                if (Free.read() == 1) {
                    If_ok1.write(1);
                    Tx_helper1.write(1);

                    Free.write(0);

                    If_ok2.write(0); // 防止同时来的其他mobile传输
                    Tx_helper2.write(1);
                    If_ok3.write(0);
                    Tx_helper3.write(1);

                    cout << sc_time_stamp() << " MOBILE1 request:" << Request1.read() << ", MOBILE2 request: " << Request2.read() << ", MOBILE3 request: " << Request3.read() << endl;
                    cout << sc_time_stamp() << " If_ok1: " <<  If_ok1.read() << ", If_ok2: " << If_ok2.read() << ", If_ok3: " << If_ok3.read() << endl << endl;


                    wait(1); // 这里没错， 需要等两个周期
                    If_ok1.write(0);
                    wait(1);
                    
                    Free.write(1);
                    cout << endl;
                    cout << "************************" << endl;
                    cout << "Server: MOBILE1 done. Network free at " << sc_time_stamp() << endl;
                    cout << "************************" << endl << endl;
                }
                else {
                    If_ok1.write(0);
                    Tx_helper1.write(1);
                }
            }

            else if (Request2.read() == 1) {
                if (Free.read() == 1) {
                    If_ok2.write(1);
                    Tx_helper2.write(1);

                    Free.write(0);

                    If_ok1.write(0);
                    Tx_helper1.write(1);
                    If_ok3.write(0);
                    Tx_helper3.write(1);
                    
                    cout << sc_time_stamp() << " MOBILE1 request:" << Request1.read() << ", MOBILE2 request: " << Request2.read() << ", MOBILE3 request: " << Request3.read() << endl;
                    cout << sc_time_stamp() << " If_ok1: " <<  If_ok1.read() << ", If_ok2: " << If_ok2.read() << ", If_ok3: " << If_ok3.read() << endl << endl;

                    wait(1);
                    If_ok2.write(0);
                    wait(1);

                    Free.write(1);
                    cout << "************************" << endl;
                    cout << "Server: MOBILE2 done. Network free at " << sc_time_stamp() << endl;
                    cout << "************************" << endl;
                }
                else {
                    If_ok2.write(0);
                    Tx_helper2.write(1);
                }
            }

            else if(Request3.read() == 1) {
                if (Free.read() == 1) {
                    If_ok3.write(1);
                    Tx_helper3.write(1);

                    Free.write(0);

                    If_ok1.write(0);
                    Tx_helper1.write(1);
                    If_ok2.write(0);
                    Tx_helper2.write(1);


                    cout << sc_time_stamp() << " MOBILE1 request:" << Request1.read() << ", MOBILE2 request: " << Request2.read() << ", MOBILE3 request: " << Request3.read() << endl;
                    cout << sc_time_stamp() << " If_ok1: " <<  If_ok1.read() << ", If_ok2: " << If_ok2.read() << ", If_ok3: " << If_ok3.read() << endl << endl;


                    wait(1);
                    If_ok3.write(0);
                    wait(1);

                    Free.write(1);
                    cout << endl;
                    cout << "************************" << endl;
                    cout << "Server: MOBILE3 ddone. Network free at " << sc_time_stamp() << endl;
                    cout << "************************" << endl <<endl;
                }
                else {
                    If_ok3.write(0);
                    Tx_helper3.write(1);
                }
            }
        }
        else if (is_sending_image_packet == 1) {
            If_ok1.write(0);
            If_ok2.write(0);
            If_ok3.write(0);
            Free.write(0);
        }
    }
}


void SERVER::TX() {

    while(1) {
        wait();
        if (Free.read() == 1) {
            if (packet_num < packetnum_becauseof_size) {    
                MOBILE1_PICTURE();
                MOBILE1_PICTURE();
                MOBILE1_PICTURE();
                MOBILE1_PICTURE();
                MOBILE1_PICTURE();
                
                MOBILE2_PICTURE();
                MOBILE2_PICTURE();
                MOBILE2_PICTURE();
                MOBILE2_PICTURE();
                MOBILE2_PICTURE();

                MOBILE3_PICTURE();
                MOBILE3_PICTURE();
                MOBILE3_PICTURE();
                MOBILE3_PICTURE();
                MOBILE3_PICTURE();
                packet_num++;
            }       
        }
    }
}


void SERVER::MOBILE1_PICTURE() {
    is_sending_image_packet = 1;
    Image_packet1.write(1);
    cout << sc_time_stamp() << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    cout << "Begin to send one packet to Mobile1 at " << sc_time_stamp() << endl;
    wait(20, SC_MS); // 1M 网速
    //wait(10, SC_MS); //0.5M 网速
    //wait(10, SC_MS);  // 5M 网速
    //wait(10, SC_MS);  // 10M 网速
    Image_packet1.write(0);
    is_sending_image_packet = 0;
    cout << "End sending to Mobile1 and will wait for 100ms at " << sc_time_stamp() << endl;
    wait(100, SC_MS);  
}

void SERVER::MOBILE2_PICTURE() {
    is_sending_image_packet = 1;
    Image_packet2.write(1);
    cout << sc_time_stamp() << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    cout << "Begin to send one packet to Mobile2 at " << sc_time_stamp() << endl;
    wait(10, SC_MS); // 1M 网速
    //wait(10, SC_MS); //0.5M 网速
    //wait(10, SC_MS);  // 5M 网速
    //wait(10, SC_MS);  // 10M 网速
    Image_packet2.write(0);
    is_sending_image_packet = 0;
    cout << "End sending to Mobile2 and will wait for 100ms at " << sc_time_stamp() << endl;
    wait(100, SC_MS); 
}

void SERVER::MOBILE3_PICTURE() {
    is_sending_image_packet = 1;
    Image_packet3.write(1);
    cout << sc_time_stamp() << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    cout << "Begin to send one packet to Mobile3 at " << sc_time_stamp() << endl;
    wait(10, SC_MS); // 1M 网速
    //wait(10, SC_MS); //0.5M 网速
    //wait(10, SC_MS);  // 5M 网速
    //wait(10, SC_MS);  // 10M 网速
    Image_packet3.write(0);
    is_sending_image_packet = 0;
    cout << "End sending to Mobile3 and will wait for 100ms at " << sc_time_stamp() << endl;
    wait(100, SC_MS);  
}