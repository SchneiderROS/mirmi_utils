import socket
import time


def server(ip, port):
    s = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    s.bind((ip, port))

    while (True):
        msg_in = s.recvfrom(2048)
        t_now = time.time()
        #t_client = float(msg_in[0].decode("utf-8"))
        print(msg_in)
        #print(t_now-t_client)


def client(ip, port):
    s = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

    while True:
        msg = str.encode(str(time.time()))
        msg = str.encode("TEST")
        s.sendto(msg, (ip, port))
