//document.getElementById("datetime").innerHTML = "WebSocket is not connected";

var websocket = new WebSocket('ws://'+location.hostname+'/');
var maxLine = 10;
var currentLine = 0;

function getTextValueByName(name) {
	var textbox = document.getElementsByName(name)
	//console.log('name=', name);
	//console.log('textbox=', textbox);
	//console.log('textbox.length=', textbox.length);
	ret = new Array();
	for (var i=0;i<textbox.length;i++) {
		//console.log('textbox[%d].value=%s', i, textbox[i].value);
		ret[i] = textbox[i].value;
	}
	//console.log('typeof(ret)=', typeof(ret));
	//console.log('ret=', ret);
	return ret;
}

function changeLine() {
	console.log('changeLine');
	//var text = getTextValueByName("line");
	//console.log('text=', text);
	var text = document.getElementById("line");
	console.log('text.value=', text.value);
	console.log('typeof(text.value)=', typeof(text.value));
	maxLine = parseInt(text.value, 10);
	
}

function copyData() {
	console.log('copyData');
	var object = document.getElementById('article');
	console.log('object=', object);
	console.log('typeof(object)=', typeof(object));

	var str="";

	var object_children = object.children;
	console.log('object.children=', object_children);
	console.log('object.children.length=', object_children.length);
	for(var i = 0; i < object_children.length; i++) {
		//console.log(object_children[i]);
		//console.log(typeof(object_children[i]));
		console.log(object_children[i].innerText);
		str += object_children[i].innerText;
		str += "\r\n";
	}

	if(navigator.clipboard){
		console.log("clipboard.writeText");
		navigator.clipboard.writeText(str);
	} else {
		// https://qiita.com/yamayamasou/items/a53bd71894a6058c5f30
		console.log("clipboard not found");
		const textarea = document.createElement('textarea');
		textarea.value = str;
		document.body.appendChild(textarea);
		textarea.select();
		const result = document.execCommand('copy');
		document.body.removeChild(textarea);
		//alert("This browser is not supported");
	}
}

function sendText(name) {
	console.log('sendText');
/*
	var array = ["11", "22", "33"];
	var data = {};
	//data["foo"] = "abc";
	data["foo"] = array;
	var array = ["aa"];
	data["bar"] = array;
	data["hoge"] = 100;
	json_data = JSON.stringify(data);
	console.log(json_data);
*/

	var data = {};
	data["id"] = name;
	var payload_object = getTextValueByName("payload");
	console.log('payload_object=' + payload_object);
	console.log('type payload=' + (typeof payload_object));
	var payload_str = payload_object.toString();
	console.log('type payload_str=' + (typeof payload_str));
	data["payload"] = payload_str;
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

websocket.onopen = function(evt) {
	console.log('WebSocket connection opened');
	var data = {};
	data["id"] = "init";
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
	//document.getElementById("datetime").innerHTML = "WebSocket is connected!";
}

websocket.onmessage = function(evt) {
	var msg = evt.data;
	console.log("msg=" + msg);
	var values = msg.split('\4'); // \4 is EOT
	console.log("values=" + values);
	switch(values[0]) {
		case 'RECEIVE':
			console.log("RECEIVE currentLine=%d maxLine=%d" , currentLine, maxLine);
			if (currentLine >= maxLine) {
				while(1) {
					var object = document.getElementById('article');
					object.removeChild(object.childNodes.item(1));
					//console.log("removeChild");
					currentLine = currentLine - 1;
					if (currentLine == (maxLine -1)) break;
				}
			}
			console.log("RECEIVE values[1]=" + values[1]);
			var msg = document.createElement('div')
			//msg.className = 'message-body';
			//msg.className = 'message-body-frame';
			msg.className = 'message-body-no-frame';
			//msg.style.cssText = "color: red;" + "display:inline-block;" + "_display: inline;"
			msg.innerText = values[1];
			document.getElementById('article').appendChild(msg);

			currentLine = currentLine + 1;
			console.log("RECEIVE currentLine =", currentLine);
			break;

		default:
			break;
	}
}

websocket.onclose = function(evt) {
	console.log('Websocket connection closed');
	//document.getElementById("datetime").innerHTML = "WebSocket closed";
}

websocket.onerror = function(evt) {
	console.log('Websocket error: ' + evt);
	//document.getElementById("datetime").innerHTML = "WebSocket error????!!!1!!";
}
