#!/usr/bin/env python
# python3 -m pip install websocket-server

import argparse
from websocket_server import WebsocketServer

# Called for every client connecting (after handshake)
def new_client(client, server):
	print("New client connected and was given id %d" % client['id'])
	server.send_message_to_all("Hey all, a new client has joined us")

# Called for every client disconnecting
def client_left(client, server):
	print("Client(%d) disconnected" % client['id'])

# Called when a client sends a message
def message_received(client, server, message):
	print("Client(%d) said: %s" % (client['id'], message))
	server.send_message_to_all("OK")

if __name__=='__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('--port', type=int, help='tcp port', default=8080)
	args = parser.parse_args()
	print("args.port={}".format(args.port))

	server = WebsocketServer(host = "0.0.0.0", port = args.port)
	server.set_fn_new_client(new_client)
	server.set_fn_client_left(client_left)
	server.set_fn_message_received(message_received)
	server.run_forever()
