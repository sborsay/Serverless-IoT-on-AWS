var myChart;  // it should be global, otherwise it will be create "javascript execution context" issue
var loadChart = function(uptime, temperature, humidity) {
    var ctx = $('#myChart');

    myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: uptime,
            datasets: [
                {
                    label: "Temperature",
                    data: temperature,
                    fill: false,
                    borderColor: "rgb(255, 0, 0)",
                    lineTension: 0.1
                },
                {
                    label: "Humidity",
                    data: humidity,
                    fill: false,
                    borderColor: "rgb(0, 0, 255)",
                    lineTension: 0.1
                }
            ]
        },
        options: {}
    });
}
