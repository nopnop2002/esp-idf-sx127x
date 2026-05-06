import socket
import ssl

hostname = 'localhost'
port = 8000

# 1.Configuring SSL context
# Use a secure protocol (TLS) for the client.
context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)

# 2.Loading certificates
# The server.crt files must be generated beforehand.
context.check_hostname = False
context.load_verify_locations('server.crt')

# 3.Create a regular socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:

	# 4.Wrap the socket with SSL and wait for client connections
	with context.wrap_socket(sock, server_hostname=hostname) as ssock:
		print(f"SSL version: {ssock.version()}")
		
		ssock.connect((hostname, port))
		message = "hello world"
		ssock.send(message.encode('utf-8'))
		response = ssock.recv(4096)
		print(response.decode('utf-8', 'ignore'))
