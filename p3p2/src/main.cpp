
#include "systemc.h"
#include "mobile.h"
#include "server.h"

using namespace std;

int sc_main(int a, char *b[]) {
    sc_clock clk("clk", 10, SC_MS, 0.5, 10, SC_MS, true);
    sc_signal<int> FREE;
    sc_signal<bool> REQUEST1, REQUEST2, REQUEST3; // request 和 free 分开用
    sc_signal<int> BEGIN1, BEGIN2, BEGIN3;
    sc_signal<int> END1, END2, END3;
    sc_signal<int> IF_OK1, IF_OK2, IF_OK3;
    sc_signal<int> TX_HELPER1, TX_HELPER2, TX_HELPER3;
    sc_signal<int> Image_packet1, Image_packet2, Image_packet3;

    FREE.write(1);

    MOBILE mobile1("mobile1", 1);
    mobile1(clk, FREE, IF_OK1, TX_HELPER1, Image_packet1, BEGIN1, END1, REQUEST1);

    MOBILE mobile2("mobile2", 2);
    mobile2(clk, FREE, IF_OK2, TX_HELPER1, Image_packet2, BEGIN2, END2, REQUEST2);

    MOBILE mobile3("mobile3", 3);
    mobile3(clk, FREE, IF_OK3, TX_HELPER1, Image_packet3, BEGIN3, END3, REQUEST3);

    SERVER server("server");
    server(clk, BEGIN1, BEGIN2, BEGIN3, END1, END2, END3, REQUEST1, REQUEST2, REQUEST3, FREE, IF_OK1, IF_OK2, IF_OK3, TX_HELPER1, TX_HELPER2, TX_HELPER3, Image_packet1, Image_packet2, Image_packet3);

    sc_start(280000, SC_MS);

    return 0;
}