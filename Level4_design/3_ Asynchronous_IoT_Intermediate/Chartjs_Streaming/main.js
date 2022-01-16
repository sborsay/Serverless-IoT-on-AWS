// Based on https://medium.com/@james.li/how-to-visualise-real-time-order-book-data-and-host-your-own-dashboard-part-1-2-c77aa0fc5f59
//incoming payload:  { "temperature": 55, "humidity": 66, "timestamps":  1648273645364}
// do not open cntrl+shft+ i while running  can cause console fault 

const canvasDiv = document.querySelector('#canvas-div');
let canvas = document.createElement("canvas");
canvasDiv.appendChild(canvas);

let buf = {};
let subArray = 'placeholder';

let ctx = canvas.getContext('2d');

const socket = new WebSocket('wss://<Insert-Your-AWS-WebSocket-URL-Here>.amazonaws.com/production')

socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch and the Parameter Store on AWS')
})

  socket.addEventListener('message', event => {
	
   console.log('Your iot payload is:', event.data);
   var IoT_Payload = JSON.parse(event.data);
   console.log("ourr IoT_Payload", IoT_Payload);

   let unix_timestamp = 1549312452
// Create a new JavaScript Date object based on the timestamp
// multiplied by 1000 so that the argument is in milliseconds, not seconds.
var date = new Date(IoT_Payload.timestamps);
// Hours part from the timestamp
var hours = date.getHours();
// Minutes part from the timestamp
var minutes = "0" + date.getMinutes();
// Seconds part from the timestamp
var seconds = "0" + date.getSeconds();

// Will display time in 10:30:23 format
var formattedTime = hours + ':' + minutes.substr(-2) + ':' + seconds.substr(-2);

console.log("for time: ", formattedTime);


buf[subArray] = [[], []]; 

    buf[subArray][0].push({
        x: IoT_Payload.timestamps,   //fomerally timestamp
        y: IoT_Payload.temperature
    });

 console.log("buff1: ",   buf[subArray][0] )

    buf[subArray][1].push({
        x: IoT_Payload.timestamps,
        y: IoT_Payload.humidity
    });

      console.log("my buffer2:, ", buf[subArray][1])
      console.log("my buffer3:, ", buf[subArray])

}) //end message ws func


let chart = new Chart(ctx, {
    type: 'line',
    data: {
        datasets: [{
            data: [],
            label: 'Temperature',
            borderColor: 'rgb(0, 255, 0)',
            backgroundColor: 'rgba(0, 255, 0, 0.5)',
            fill: false,
            lineTension: 0
        }, {
            data: [],
            label: 'Humidity',
            borderColor: 'rgb(255, 0, 0)',
            backgroundColor: 'rgba(255, 0, 0, 0.5)',
            fill: false,
            lineTension: 0
        }]
    },
    options: {
        title: {
            text: 'AWS Asyncrounous IoT with Chartjs streaming',
            display: true
        },
        scales: {
            xAxes: [{
                type: 'realtime'
            }]
        },
        plugins: {
            streaming: {
                duration: 300000,
                onRefresh: (chart) => {

                   // console.log("buf -ex1", buf[subArray][0])
                    Array.prototype.push.apply(
                        chart.data.datasets[0].data, buf[subArray][0]
                    );  
                  //    console.log("buf -ex2", buf[subArray][1])
                    Array.prototype.push.apply(
                      
                        chart.data.datasets[1].data,  buf[subArray][1]
                    );
               
                    buf[subArray] = [[], []];  //clear buffer
                }
            }
        }
    }
})
