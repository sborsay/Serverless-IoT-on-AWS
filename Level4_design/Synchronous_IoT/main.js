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

let getWheatherData = function () {
    $.ajax({
        type: "GET",
        url: "<Insert-Your-IoT-Data-Bucket-With-Key-Here>",  //example: https://mydatabucket.s3.amazonaws.com/myKey"
        dataType: "json",
        async: false,
        success: function (data) {
            console.log('data', data);
            drawChart(data);
        },
        error: function (xhr, status, error) {
            console.error("JSON error: " + status);
        }
    });
}

let drawChart = function (data) {

    let { humidity, temperature, timestamps } = data;

    humArr.push(Number(humidity));
    tempArr.push(Number(temperature));
    upArr.push(Number(timestamps));
    
    myChart.series[0].setData(humArr , true)
    myChart.series[1].setData(tempArr , true)
}

let intervalTime = 3 * 1000; // 3 second interval polling, change as you like
setInterval(() => {
    getWheatherData();
}, intervalTime);
