//Modified from: https://redstapler.co/javascript-realtime-chart-plotly/

const socket = new WebSocket('wss://<Paste_Tour AWS-WebSocket-URL-Here>.amazonaws.com/production')

socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch and the Parameter Store on AWS')
})

var temp = 0;
var ts= 0;

var A = new Date('October 35, 2021 12:35:32'); 
   var B = A.getTime();

socket.addEventListener('message', event => {
	
  var IoT_Payload = JSON.parse(event.data);
   console.log("ourr json object", IoT_Payload);
temp = IoT_Payload.temperature;
  ts = IoT_Payload.timestamps;
console.log("temp: ", temp)

  }) //end message ws func

  
            function getData() {
               
               return .5  //better for predicatble x-axis lacuna  - moderate gaps
            }  


            Plotly.plot('chart',[{
                y:[getData],
                type:'line'
            }]);
            
            var cnt = 0;

            setInterval(function(){

                Plotly.extendTraces('chart',{ y:[[temp]]}, [0]);
                cnt++;
                if(cnt > 500) {
                    Plotly.relayout('chart',{
                        xaxis: {
                            range: [cnt-500,cnt]
                        }
                    });
                }
            },15);
