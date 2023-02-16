import zmq
import json
import time

def send_val(zmqSkt, var, val):

    val = {"var": var, "val": val}
    json_val = json.dumps(val)
    zmqSkt.send(json_val.encode())

def receive_val(zmqSkt):

    raw_val = zmqSkt.recv()
    json_val = json.loads(raw_val)

    var = json_val['var']
    val = json_val['val']

    print("Received: var={}, val={}".format(var, val))

def initZeroMq(ip_addr: str, port: int):

    zmqCxt = zmq.Context()
    zmqSkt = zmqCxt.zmqSkt(zmq.REQ)
    zmqSkt.connect("tcp://" + ip_addr + ":" + str(port))

    return zmqSkt

def main():

    zrMqSocket = initZeroMq("localhost", 5555)

    while True:
        send_val(zrMqSocket, "python", 10)
        receive_val(zrMqSocket)
        time.sleep(1)

if __name__ == "__main__":
    main()
