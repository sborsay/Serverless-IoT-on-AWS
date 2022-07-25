let tempArr = [], humArr = [],  resArr = [], luxArr = [], upArr = [];

const socket = new WebSocket('wss://<Your-WSS-WebSocket-Endpoint-Here>')

socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch and the Parameter Store on AWS')
})


socket.addEventListener('message', event => {
	
	console.log('Your iot payload is:', event.data);
    //guages draw chart
    drawChart(event.data);  //Bar Graph
    drawChart2(event.data); //Temperature F
    drawChart3(event.data); //Humidity index %
    drawChart4(event.data); //resistance
    drawChart5(event.data); //lux
	})


let myChart = Highcharts.chart('container1', {
    
    title: {
        text: ''
    },

    subtitle: {
        text: ''
    },

    yAxis: {
        title: {
            text: 'Value'
        }
    },

    xAxis: {
        categories: upArr
    },

    legend: {
        layout: 'vertical',
        align: 'right',
        verticalAlign: 'middle'
    },

    plotOptions: {
        series: {
            label: {
                connectorAllowed: false
            }
        }
    },
    series: [{
        name: 'Temperature',
        data: []
    }, {
        name: 'Humdidity',
        data: []
    },
    {
        name: 'Resistence',
        data: []
    },
    {
        name: 'Lux',
        data: []
    }],
    responsive: {
        rules: [{
            condition: {
                maxWidth: 500
            },
            chartOptions: {
                legend: {
                    layout: 'horizontal',
                    align: 'center',
                    verticalAlign: 'bottom'
                }
            }
        }]
    }

});


//let myChart2 = Highcharts.chart('container1', {

    let myChart2 = Highcharts.chart('container2', {

    chart: {
        type: 'gauge',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },

    title: {
        text: 'Temperature'
    },

    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },

    // the value axis
    yAxis: {
        min: 0,
        max: 150,

        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',

        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto'
        },
        title: {
            text: 'Degrees °F'
        },
        plotBands: [{
            from: 0,
            to: 90,
            color: '#55BF3B' // green
        }, {
            from: 90,
            to: 105,
            color: '#DDDF0D' // yellow
        }, {
            from: 105,
            to: 150,
            color: '#DF5353' // red
        }]
    },

    series: [{
        name: 'Temp',
        data: [0],
        tooltip: {
            valueSuffix: ' degrees'
        }
    }]

},  //is this comma needed?
// Add some life

//func here
);

//--------------------
let myChart3 = Highcharts.chart('container3', {

    chart: {
        type: 'gauge',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },

    title: {
        text: 'Humidity'
    },

    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },

    // the value axis
    yAxis: {
        min: 0,
        max: 100,

        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',

        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto'
        },
        title: {
            text: 'Humidity %'
        },
        plotBands: [{
            from: 0,
            to: 40,
            color: '#55BF3B' // green
        }, {
            from: 40,
            to: 70,
            color: '#DDDF0D' // yellow
        }, {
            from: 70,
            to: 100,
            color: '#DF5353' // red
        }]
    },

    series: [{
        name: 'humid',
        data: [0],
        tooltip: {
            valueSuffix: ' degrees'
        }
    }]

},  //is this comma needed?
// Add some life

//func here
);
//-----------------

let myChart4 = Highcharts.chart('container4', {

    chart: {
        type: 'gauge',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },

    title: {
        text: 'Resistance'
    },

    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },

    // the value axis
    yAxis: {
        min: 0,
        max: 200,

        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',

        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto'
        },
        title: {
            text: 'Resistance Ω'
        },
        plotBands: [{
            from: 0,
            to: 100,
            color: '#0000FF' // white
        },  {
            from: 100,
            to: 200,
            color: '#DF5353' // red
        }]
    },

    series: [{
        name: 'Resistance',
        data: [0],
        tooltip: {
            valueSuffix: 'Ω'
        }
    }]

},  //is this comma needed?
// Add some life
);

let myChart5 = Highcharts.chart('container5', {

    chart: {
        type: 'gauge',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },

    title: {
        text: 'Lux'
    },

    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },

    // the value axis
    yAxis: {
        min: 0,
        max: 300,

        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',

        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto'
        },
        title: {
            text: 'Lux lx'
        },
        plotBands: [{
            from: 0,
            to: 75,
            color: '#FFFFFF' // yellow
        }, {
            from: 75,
            to: 150,
            color: '#808080' // grey
        }, {
            from: 150,
            to: 300,
            color: '##000000' // black night
        }]
    },

    series: [{
        name: 'Lux',
        data: [0],
        tooltip: {
            valueSuffix: 'Ω'
        }
    }]

},  //is this comma needed?
// Add some life
);
//func here

//-----------------

let drawChart = function (data) {

	
    var IoT_Payload = JSON.parse(data);
    console.log("ourr json object", IoT_Payload);
    //console.log(IoT_Payload.temperature);
    
    
        let { humidity, temperature, resistance, timestamps } = IoT_Payload;
       
        tempArr.push(Number(IoT_Payload.temperature));
        humArr.push(Number(IoT_Payload.humidity));
        resArr.push(Number(IoT_Payload.resistance));
        luxArr.push(Number(IoT_Payload.lux));
        upArr.push(Number(IoT_Payload.timestamps));
    
        myChart.series[0].setData(tempArr, true)
        myChart.series[1].setData(humArr, true)
        myChart.series[2].setData(resArr, true)
        myChart.series[3].setData(luxArr, true)
        
       
    }
    
    //------------------ guages

    let drawChart2 = function (data) {

        var IoT_Payload = JSON.parse(data);
        console.log("our json object2", IoT_Payload);

        var newVal = (IoT_Payload.temperature);
        var point = myChart2.series[0].points[0];
        console.log('Your point:', point);
        console.log('Your newVal:', newVal);
        point.update(newVal);
    }

    let drawChart3 = function (data) {

        var IoT_Payload = JSON.parse(data);
        console.log("our json object3", IoT_Payload);

        var newVal = (IoT_Payload.humidity);
        var point = myChart3.series[0].points[0];
        console.log('Your point:', point);
        console.log('Your newVal:', newVal);
        point.update(newVal);
    }


    let drawChart4 = function (data) {

        var IoT_Payload = JSON.parse(data);
        console.log("our json object4", IoT_Payload);

        var newVal = (IoT_Payload.resistance);
        var point = myChart4.series[0].points[0];
        console.log('Your point:', point);
        console.log('Your newVal:', newVal);
        point.update(newVal);
    }

    let drawChart5 = function (data) {

        var IoT_Payload = JSON.parse(data);
        console.log("our json object4", IoT_Payload);

        var newVal = (IoT_Payload.lux);
        var point = myChart5.series[0].points[0];
        console.log('Your point:', point);
        console.log('Your newVal:', newVal);
        point.update(newVal);
    }
