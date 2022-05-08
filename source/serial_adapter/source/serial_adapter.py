# pip3 install websocket-client
from websocket import create_connection

def onMessage(ws, message):
    print(message)

ws = create_connection("ws://192.168.0.8:9091/", on_message=onMessage)
ws.send("positionUpdate(&**&)2 2")
ws.close()