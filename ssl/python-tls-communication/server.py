import socket
import ssl

# 1.Configuring SSL context
# Use a secure protocol (TLS) for the server.
context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)

# 2.Loading certificates and private keys
# The server.crt and server.key files must be generated beforehand.
context.load_cert_chain(certfile="server.crt", keyfile="server.key")

# 3.Create a regular socket
bind_address = '0.0.0.0'
bind_port = 8080
with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:
	sock.bind((bind_address, bind_port))
	sock.listen(5)
	print(f"Serving on {bind_address}:{bind_port}...")

	# 4.Wrap the socket with SSL and wait for client connections
	with context.wrap_socket(sock, server_side=True) as ssock:
		while True:
			try:
				client_sock, client_addr = ssock.accept()
				with client_sock:
					print(f"Connection from: {client_addr}")
					data = client_sock.recv(1024)
					print(f"Received: {data.decode()}")
					client_sock.sendall(b"Hello, secure world!\n")
					client_sock.close()
			except Exception as e:
				print(f"Error: {e}")
