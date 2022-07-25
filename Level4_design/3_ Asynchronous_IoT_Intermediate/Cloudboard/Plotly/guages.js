 
	  
const socket = new WebSocket('wss://<Your-WSS-Endpoint-Here>')

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


    //begin guages ----------------------------

   var data = [
    {
      domain: {'row': 5, 'column': 1},
      //domain: { x: [0, 1], y: [0, 1] },
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
      //domain: { x: [1, 0], y: [0, 1] },
      domain: {'row': 5, 'column': 1},
      value: IoT_Payload.humidity,
      title: { text: "Humidity" },
      type: "indicator",
      mode: "gauge+number",
      delta: { reference: 400 },
      gauge: { axis: { range: [null, 100] },   bar: { color: "darkblue" } }
    }
  ];

  var data4 = [
    {
      domain: { x: [1, 0], y: [0, 1] }, // domain: { x: [1, 0], y: [0, 1] },
      value: IoT_Payload.resistance,
      title: { text: "Resistance" },
      type: "indicator",
      mode: "gauge+number",
      delta: { reference: 400 },
      gauge: { axis: { range: [null, 100] },   bar: { color: "brown" } }
    }
  ];

  var data5= [
    {
      domain: { x: [1, 0], y: [0, 1] }, // domain: { x: [1, 0], y: [0, 1] },
      value: IoT_Payload.lux,
      title: { text: "Lux" },
      type: "indicator",
      mode: "gauge+number",
      delta: { reference: 400 },
      gauge: { axis: { range: [null, 100] },   bar: { color: "yellow" } }
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
      domain: { x: [0, 1], y: [0, 1] }
    }
  ];

var layout = { width: 500, height: 400};  //temperature
var layout2 = { width: 500, height: 400 }; //humidity
var layout6 = { width: 500, height: 400 }; //resistance
var layout7 = { width: 500, height: 400 }; //lux

var layout3 = {                               //timestamp
  paper_bgcolor: "white",
  width: 350,
  height: 200,
  margin: { t: 0, b: 0, l: 0, r: 0 }
};


Plotly.newPlot('myDiv',   data, layout);
Plotly.newPlot('myDiv2', data2, layout2);
Plotly.newPlot('myDiv6', data4, layout6); //resistance
Plotly.newPlot('myDiv7', data5, layout7); //resistance
Plotly.newPlot('myDiv3', data3, layout3);



  //end guages------------------------------------

//line charts ---------------------------------------------------
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
    font: {
      family: 'Courier New, monospace',
      size: 36,
      color: 'blue'
    }
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
    font: {
      family: 'Courier New, monospace',
      size: 36,
      color: 'purple'
    }
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

  //resistence begin

  var dataa3 = [{
    x: [time], 
    y: [IoT_Payload.resistance],
    mode: 'lines',
    line: {color: '#80CAF6'},
    name: '<i>Resistance Ω</i>'
  }] 
  
  var layout8 = {
    title: {
      text:'<b>Resistance<b>',
      font: {
        family: 'Courier New, monospace',
        size: 36,
        color: 'brown'
      }
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
      text: "ohms",
      font: {
      family: "Courier New, monospace",
      size: 18,
      color: "#7f7f7f"
          }
        }
      }
    }
    //resistance end


    //lux start 

    var dataa4 = [{
      x: [time], 
      y: [IoT_Payload.lux],
      mode: 'lines',
      line: {color: '#80CAF6'},
      name: '<i>Lux lx</i>'
    }] 
    
    var layout9 = {
      title: {
        text:'<b>Lux<b>',
        font: {
          family: 'Courier New, monospace',
          size: 36,
          color: 'orange'
        }
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
        text: "lx",
        font: {
        family: "Courier New, monospace",
        size: 18,
        color: "#7f7f7f"
            }
          }
        }
      }

      //lux end

  //line charts ---------------------------------------------------
Plotly.plot('myDiv4', dataa, layout4);
Plotly.plot('myDiv5', dataa2, layout5); 
Plotly.plot('myDiv8', dataa3, layout8); //resistence
Plotly.plot('myDiv9', dataa4, layout9); //lux

  
  var update = {
  x:  [[time]],
  y: [[IoT_Payload.temperature]]
  }
  
   var update2 = {
  x:  [[time]],
  y: [[IoT_Payload.humidity]]
  }

  var update3 = {
    x:  [[time]],
    y: [[IoT_Payload.resistance]]
    }

    var update4 = {
      x:  [[time]],
      y: [[IoT_Payload.lux]]
      }
  
  Plotly.extendTraces('myDiv4', update,  [0])
	Plotly.extendTraces('myDiv5', update2, [1])
  Plotly.extendTraces('myDiv8', update3, [2])
  Plotly.extendTraces('myDiv9', update4, [3])
  

  
  //end graph section

}) //end websocket function


