var loadLineChart = function(uptime, temperature, humidity) {
    var lineChart = $('#lineChart');

    var myChart1 = new Chart(lineChart, {
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
        options: {
            responsive: true,
            title: {
                display: true,
                text: 'Uptime',
                position: 'bottom'
            }
        }
    });
}

var loadBarChart = function(uptime, temperature, humidity) {
    var barChart = $('#barChart');

    var myChart2 = new Chart(barChart, {
        type: 'bar',
        data: {
            labels: uptime,
            datasets: [
                {
                    label: "Temperature",
                    data: temperature,
                    backgroundColor: "rgb(255, 0, 0)"
                },
                {
                    label: "Humidity",
                    data: humidity,
                    backgroundColor: "rgb(0, 0, 255)"
                }
            ]
        },
        options: {
            responsive: true,
            barValueSpacing: 20,
            title: {
                display: true,
                text: 'Uptime',
                position: 'bottom'
            }
        }
    });
}


