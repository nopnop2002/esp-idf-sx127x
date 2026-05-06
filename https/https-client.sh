#!/bin/bash
URL="192.168.0.40:8080"

while :
do
	payload=`date "+%Y/%m/%d %H:%M:%S"`
	#echo ${payload}
	echo curl -d "${payload}" ${URI}
	curl -X POST -d "${payload}" https://${URL} --cacert ./server.crt
	echo ""
	sleep 1
done
