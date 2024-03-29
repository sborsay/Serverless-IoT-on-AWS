const socket = new WebSocket('wss://<PASTE-YOUR-WEBSOCKET-ENDPOINT-HERE>')

socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch and the Parameter Store on AWS')
})


socket.addEventListener('message', event => {
	
  	console.log('Your iot payload is:', event.data);

   var IoT_Payload = JSON.parse(event.data);
   console.log("our IoT_Payload temp: ", IoT_Payload.temperature);


var dataa = [{
  x: [IoT_Payload.timestamps], 
  y: [IoT_Payload.temperature],
  mode: 'lines',
  line: {color: '#ff0033'}
}] 


var dataa2 = [{
  x: [IoT_Payload.timestamps], 
  y: [IoT_Payload.humidity],
  mode: 'lines',
  line: {color: '#80CAF6'}
}] 


Plotly.plot('graph', dataa);
Plotly.plot('graph', dataa2); 

  
  var update = {
  x:  [[IoT_Payload.timestamps]],
  y: [[IoT_Payload.temperature]]
  }
  
   var update2 = {
  x:  [[IoT_Payload.timestamps]],
  y: [[IoT_Payload.humidity]]
  }
  
  Plotly.extendTraces('graph', update, [0])
	Plotly.extendTraces('graph', update2, [1])
  
}) //end websocket message function  here
