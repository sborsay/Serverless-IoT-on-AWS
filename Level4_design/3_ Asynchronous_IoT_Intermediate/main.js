let humArr = [], tempArr = [], upArr = [];
let myChart = Highcharts.chart('container1', {
    
    title: {
        text: 'Line chart'
    },

    subtitle: {
        text: 'subtitle'
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
        name: 'Humdity',
        data: []
    }, {
        name: 'Temperature',
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


const socket = new WebSocket('<Insert-Your-WSS-Endpoint-With-Prefix-Here>')

socket.addEventListener('open', event => {
  console.log('WebSocket is connected, now check for your new Connection ID in Cloudwatch or the Parameter Store on AWS')
})




socket.addEventListener('message', event => {
	
	console.log('Your iot payload is:', event.data);
	drawChart(event.data);
	})
	


let drawChart = function (data) {

	
var IoT_Payload = JSON.parse(data);
console.log("our json object", IoT_Payload);
console.log(IoT_Payload.temperature);


    let { humidity, temperature, timestamps } = IoT_Payload;
   
	
	humArr.push(Number(IoT_Payload.humidity));
    tempArr.push(Number(IoT_Payload.temperature));
    upArr.push(Number(IoT_Payload.timestamps));

    
    myChart.series[0].setData(humArr , true)
    myChart.series[1].setData(tempArr , true)
}



