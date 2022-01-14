
const socket = new WebSocket('wss://<your-ws-endpoint-here>.amazonaws.com/production')


socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch and the Parameter Store on AWS')
})


socket.addEventListener('message', event => {
	console.log('Your IoTT payload is:', event.data);

            

            var IoT_Payload = JSON.parse(event.data);
            console.log("our json object3", IoT_Payload);
    
            console.log( "humidity is:  ", IoT_Payload.humidity);
            

            var temperature = IoT_Payload.temperature;
            var humidity = IoT_Payload.humidity;
            var timestamps = IoT_Payload.timestamps;
   
      
         google.charts.load('current', {packages: ['corechart', 'line']});
         google.charts.setOnLoadCallback(drawBasic);  
  
         function drawBasic() {

            //-----------see about incoming data event from wss
            //var IoT_Payload = JSON.parse(data);
           // console.log("our json object3", IoT_Payload);
    
            //console.log( "Humiddxxxxy", IoT_Payload.humidity);
    
            //------------------------ end incoming event


  
        var theData  = new google.visualization.DataTable();
        theData.addColumn('number', 'time');
        theData.addColumn('number', 'temperature');
        theData.addColumn('number', 'humidity');


  //the followig three loops convert our strings in into numbers      
  for(var ts=0; ts<timestamps.length;ts++) timestamps[ts] = parseInt(timestamps[ts], 10);
  for(var tem=0; tem<temperature.length;tem++) temperature[tem] = parseInt(temperature[tem], 10);
  for(var hum=0; hum<humidity.length;hum++) humidity[hum] = parseInt(humidity[hum], 10);


   theData.addRow([timestamps,temperature, humidity]);  //problem is no memory, only plots last point
   

        var options = {
          hAxis: {
            title: 'Time'
          },
          vAxis: {
            title: 'Environmental Data'
          }
                    
        };
  
  
        var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
  
        chart.draw(theData, options);

      
      }

    }); //end websocket function on message
  
