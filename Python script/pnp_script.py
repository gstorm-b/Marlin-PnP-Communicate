from marlin_client import MarlinClient
from marlin_client import Position

host = '127.0.0.1'
port = 27800

client = MarlinClient(host, port)

pick = Position(200, 80, 8, 0)
place = Position(20, 150, 8, 1)

client.connectToHost()
# client.marlinSetPump(1)
# client.marlinPnp(pick, 20, place, 30, 300, 200,1000)
client.marlinMoveL(pick, 100)
client.marlinAddPnpDoneMarker()
client.marlinWaitForPnpDone()
# client.marlinSetPump(0)
# client.marlinHoming()
client.disconnectFromHost()

print("Done!")

