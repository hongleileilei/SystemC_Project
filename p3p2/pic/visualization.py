import matplotlib.pyplot as plt

def main():
    txtpre = "mobile"
    txtpost = ".txt"
    x = 1
    while(x < 4):
        time = []
        mem = []
        tx_packet = []
        tuple_ = []
        rx_packet_counter = []
        image_counter = []

        ele_in_time = 0
        txt = txtpre + str(x) + txtpost
        f = open(txt, "r")
        
        for line in f:
            line = line.split()
            time.append(ele_in_time)
            ele_in_time += 10
            tuple_.append(line[0])
            tx_packet.append(line[1])
            rx_packet_counter.append(line[2])
            image_counter.append(line[3])

        for i in range(0, len(tx_packet) ):
            tx_packet[i] = int(tx_packet[i])
            tuple_[i] = int(tuple_[i])
            rx_packet_counter[i] = int(rx_packet_counter[i])
            image_counter[i] = int(image_counter[i])

        for i in range(0, len(tx_packet) ):
            a = (tx_packet[i] * 20 * 64 * 3 + tuple_[i] * 64 * 3 + rx_packet_counter[i] * 1024 + image_counter[i] * 8 * 1024 * 1024) / (1024*1024)
            mem.append(a)

        #plt.show(x)
        plt.plot(time, mem)
        plt.xlabel("Time (ms)")
        plt.ylabel("Memory (Mb)")
        plt.title("Mobile" + str(x))
        plt.show()
        x = x + 1

if __name__ == "__main__":
    main()