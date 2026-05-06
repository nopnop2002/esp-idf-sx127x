# https://qiita.com/masakielastic/items/05cd6a36bb6fb10fccf6
import argparse
from http.server import HTTPServer
from http.server import BaseHTTPRequestHandler
import ssl

class my_handler(BaseHTTPRequestHandler):
	def do_POST(self):
		if (args.print): print(self.headers)
		content_len  = int(self.headers.get("content-length"))
		#print("content_len={}".format(content_len))
		req_body = self.rfile.read(content_len).decode("utf-8")
		#print("req_body={}".format(req_body))
		print("{}".format(req_body))

		body = "OK"
		self.send_response(200)
		self.send_header('Content-type', 'text/html; charset=utf-8')
		self.send_header('Content-length', len(body.encode()))
		self.end_headers()
		self.wfile.write(body.encode())

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('--port', type=int, help='tcp port', default=8080)
	parser.add_argument('--print', action='store_true')
	args = parser.parse_args()
	print("args.port={}".format(args.port))
	print("args.print={}".format(args.print))
	host = '0.0.0.0'

	ctx = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
	ctx.load_cert_chain('server.crt', keyfile='server.key')
	#ctx.options |= ssl.OP_NO_TLSv1 | ssl.OP_NO_TLSv1_1
	ctx.minimum_version = ssl.TLSVersion.TLSv1_2

	server = HTTPServer((host, args.port), my_handler)
	server.socket = ctx.wrap_socket(server.socket)
	print('Server Starts - %s:%s' % (host, args.port))

	try:
		server.serve_forever()
	except KeyboardInterrupt:
		pass
	server.server_close()
	print('Server Stops - %s:%s' % (host, args.port))
