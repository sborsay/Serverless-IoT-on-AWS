//Designd by Stephen Borsay https://github.com/sborsay

const socket = new WebSocket('wss://<YOUR-WS-ENDPOINT>.execute-api.us-east-1.amazonaws.com/production')

socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch and the Parameter Store on AWS')
})


socket.addEventListener('message', event => {
	
   console.log('Your iot payload is:', event.data);

	
   var IoT_Payload = JSON.parse(event.data);
   console.log("ourr IoT_Payload temp: ", IoT_Payload.temperature);


   var cc = IoT_Payload.timestamps + (new Date().getTimezoneOffset() * -1); //for timeZone        
    cc = new Date(cc);
    console.log(cc)

   var data = [
    {
      domain: { x: [0, 1], y: [0, 1] },
      value: IoT_Payload.temperature,
      title: { text: "Temperature" },
      type: "indicator",
      mode: "gauge+number",
      delta: { reference: 400 },
      gauge: { axis: { range: [null, 130] },   bar: { color: "orange" }}
    }
  ];

  var data2 = [
    {
      domain: { x: [1, 0], y: [0, 1] },
      value: IoT_Payload.humidity,
      title: { text: "Humidity" },
      type: "indicator",
      mode: "gauge+number",
      delta: { reference: 400 },
      gauge: { axis: { range: [null, 100] },   bar: { color: "darkblue" } }
    }
  ];


  var data3 = [
    {
      type: "indicator",
      //mode: "number+delta",
      mode: "delta",
      value: IoT_Payload.timestamps,
      title: { text: "Timestamp" },
      delta : {position: "top",reference:100000, valueformat:'f'},
      font: {
        size: [20]
      },
      //number: { font: { size: 30 }},
      domain: { x: [0, 1], y: [0, 1] }
    }
  ];


var layout = { width: 600, height: 300};
var layout2 = { width: 600, height: 300 };
//var layout3 = { width: 600, height: 300 };
var layout3 = {
  paper_bgcolor: "white",
  width: 600,
  height: 300,
  margin: { t: 0, b: 0, l: 0, r: 0 }
};


Plotly.newPlot('myDiv', data, layout);
Plotly.newPlot('myDiv2', data2, layout2);
Plotly.newPlot('myDiv3', data3, layout3);

  //begin graph section

var time = new Date();

var dataa = [{
  x: [time], 
  y: [IoT_Payload.temperature],
  mode: 'lines',
  line: {color: '#ff0033'},
  name: '<i>Temperature °</i>'
}] 

var layout4 = {
  title: {
    text:'<b>Temperature</b>',
    size: 36,
    color: '#7f7f7f'
  },

      xaxis: {
        title: {
        text: "time",
        font: {
        family: "Courier New, monospace",
        size: 18,
        color: "#7f7f7f"
      }
    }
  },

        yaxis: {
        title: {
        text: "temperature °",
        font: {
        family: "Courier New, monospace",
        size: 18,
        color: "#7f7f7f"
        }
      }
    }
  }


var dataa2 = [{
  x: [time], 
  y: [IoT_Payload.humidity],
  mode: 'lines',
  line: {color: '#80CAF6'},
  name: '<i>Humidity %</i>'
}] 

var layout5 = {
  title: {
    text:'<b>Humidity<b>',
    size: 36,
    color: '#7f7f7f'
  },

  xaxis:{ 
    title: {
    text: "time",
    font: {
    family: "Courier New, monospace",
    size: 18,
    color: "#7f7f7f"
      }
    }
  },

    yaxis: {
    title: {
    text: "humidity %",
    font: {
    family: "Courier New, monospace",
    size: 18,
    color: "#7f7f7f"
        }
      }
    }
  }

Plotly.plot('myDiv4', dataa, layout4);
Plotly.plot('myDiv5', dataa2, layout5); 
  
  var update = {
  x:  [[time]],
  y: [[IoT_Payload.temperature]]
  }
  
   var update2 = {
  x:  [[time]],
  y: [[IoT_Payload.humidity]]
  }
  
  Plotly.extendTraces('myDiv4', update,  [0])
	Plotly.extendTraces('myDiv5', update2, [1])
  
  //end graph section

}) //end websocket function
