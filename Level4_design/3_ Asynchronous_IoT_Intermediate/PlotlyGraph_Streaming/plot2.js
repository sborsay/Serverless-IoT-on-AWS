<head>
  <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
</head>
<body>
  <div id="graph"></div>
</body>


function rand() {
  return Math.random();
}

var time = new Date();

var data = [{
  x: [time], 
  y: [rand()],
  mode: 'lines',
  line: {color: '#80CAF6'}
}] 


var data2 = [{
  x: [time], 
  y: [rand()],
  mode: 'lines',
  line: {color: '#80CAF6'}
}] 


Plotly.plot('graph', data);
Plotly.plot('graph', data2); 

var cnt = 0;

var interval = setInterval(function() {
  
  var time = new Date();
  
  var update = {
  x:  [[time]],
  y: [[rand()]],
	//y: [[rand()+1]] 
  }
  
   var update2 = {
  x:  [[time]],
  y: [[rand()]],
	//y: [[rand()+1]] 
  }
  
  Plotly.extendTraces('graph', update, [0])
	Plotly.extendTraces('graph', update2, [1])
  
  if(cnt === 100) clearInterval(interval);
}, 1000);
