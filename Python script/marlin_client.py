import socket

class Position:
    x = 0
    y = 0
    z = 0
    r = 0

    def __init__(self):
        pass

    def __init__(self, X, Y, Z, R):
        self.x = float(X)
        self.y = float(Y)
        self.z = float(Z)
        self.r = float(R)
    
    def toString(self):
        return '({0} {1} {2} {3})'.format(self.x, self.y, self.z, self.r)
    
class MarlinClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connectToHost(self):
        self.client.connect((self.host, self.port))

    def disconnectFromHost(self):
        self.client.close()

    def sendCommand(self, command):
        self.client.send(bytes(command, 'utf-8'))

    def marlinGcode(self, gcode):
        self.sendCommand('Gcode,{0};'.format(gcode))
    
    def marlinHoming(self):
        self.marlinGcode('G28')

    def marlinSetValve(self, state):
        self.sendCommand('SetValve,{0};'.format(str(state)))

    def marlinSetPump(self, state):
        self.sendCommand('SetBump,{0};'.format(str(state)))

    def marlinPnp(self, pos_pick, jump_pick, pos_place, jump_place):
        command = 'PnP,{0},{1},{2},{3};'.format(pos_pick.toString(), jump_pick, pos_place.toString(), jump_place)
        self.sendCommand(command)

    def marlinPnp(self, pos_pick, jump_pick, pos_place, jump_place, speed):
        command = 'PnP,{0},{1},{2},{3},{4};'.format(pos_pick.toString(), jump_pick, pos_place.toString(), jump_place, speed)
        self.sendCommand(command)

    def marlinPnp(self, pos_pick, jump_pick, pos_place, jump_place, speed, lift_speed):
        command = 'PnP,{0},{1},{2},{3},{4},{5};'.format(pos_pick.toString(), jump_pick, pos_place.toString(), 
                                                jump_place, speed, lift_speed)
        self.sendCommand(command)

    def marlinPnp(self, pos_pick, jump_pick, pos_place, jump_place, speed, lift_speed, delay):
        command = 'PnP,{0},{1},{2},{3},{4},{5},{6};'.format(pos_pick.toString(), jump_pick, pos_place.toString(), jump_place, speed, lift_speed, delay)
        self.sendCommand(command)

    def marlinMoveL(self, position, speed):
        command = 'MoveL,{0},{1};'.format(position.toString(), str(speed))
        self.sendCommand(command)

    def marlinAddPnpDoneMarker(self):
        self.marlinGcode('M409')

    def marlinWaitForPnpDone(self):
        while True:
            data = self.client.recv(1024)
            recv_str = data.decode('utf-8')
            # break
            if recv_str == 'PnP Done.\n':
                # print()
                break