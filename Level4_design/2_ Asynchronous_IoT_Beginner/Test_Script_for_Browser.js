const socket = new WebSocket('wss://<Insert-Your-Websockets-Endpoint-Here>')  // Example: 'wss://3143544j.execute-api.us-east-1.amazonaws.com/production'

socket.addEventListener('open', event => {
  console.log('WebSocket is connected')
})

socket.addEventListener('message', event => {

  console.log('Incoming IoT Payload:', event.data) // for JSON test event
})
