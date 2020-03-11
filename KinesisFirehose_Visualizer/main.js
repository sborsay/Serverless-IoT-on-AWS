var uptime = [];
var humidity = [];
var temperature = [];

// Parse file as text
var parseText = function(url) {
    var values ={ "uptime":"", "temperature":"", "humidity":"" };
    $.ajax({
        type     : "GET",
        url      : "https://ourfirehose212.s3.amazonaws.com/" + url,
        async : false,
        dataType : "text",
        success  : function(txtFile){
            var jsonArray;
            // Convert text file to JSON Array
            jsonArray = parseToJSONArray(txtFile)
            // Get Values from JSONArray
            jsonArray.forEach(getValuesFromJSON)
            
            values.uptime = uptime
            values.temperature = temperature
            values.humidity = humidity
        },
        error    : function(xhr, status, error){
             alert(status + ' ' + error);
        }
    });
}


// Get uptime, temperature and humidity from json
var getValuesFromJSON = function(json, index, JSONArray) {
    json = JSON.parse(json)
    uptime.push(json.uptime);
    temperature.push(json.temperature)
    humidity.push(json.humidity)
}

// This function converts the text file from server to a JSON Array
var parseToJSONArray = function(txtFile) {
    var json = [];
    var temp = "";
    var i = 0
    while(i < txtFile.length) {
        // console.log(txtFile[i])
        if(txtFile[i] != " ") {
            temp += txtFile[i]
            if(txtFile[i] == "}") {
                i = i+1
                json.push(temp)
                temp = ""
            }
        }
        i+=1
    }
    return json;
}

// Start by extracting folder2/ url from s3 bucket
$.ajax({
    type     : "GET",
    url      : "https://ourfirehose212.s3.amazonaws.com/",
    async : false,
    dataType : "xml",
    success  : function(xmlData, values){
        var url = "";
        var values = {};
        keys = xmlData.getElementsByTagName("Key");
        for(i=0; i < keys.length; i++) {
            url = keys[i].childNodes[0].nodeValue;
            parseText(url)
        }
        

        // console.log('Uptime')
        // console.log(uptime)
        // console.log('Humidity')
        // console.log(humidity)
        // console.log('Temperature')
        // console.log(temperature)
        // Load values in Chart
        loadLineChart(uptime,temperature,humidity);
        loadBarChart(uptime,temperature,humidity);
    },
    error    : function(){
         alert("Could not retrieve XML file.");
    }
})
