#!/bin/bash
#set -x
IP=`ip addr show | grep -v 127.0.0.1 | grep -oP '(?<=inet\s)\d+(\.\d+){3}' | head -n 1`
echo "Your HTTPS server is ${IP}."
read -p "Is the HTTPS Server correct? (y/n): " yn
if [[ $yn == [nN] ]]; then
	echo "Edit this file manualy."
	exit
fi

echo "Create server.key"
openssl genrsa 2048 > server.key
echo "Create server.crt with ${IP}"
openssl req -x509 -new -nodes -key server.key -subj "/CN=${IP}" -days 10000 -out server.crt
cp server.crt main/

