var uptime = new Array();
var humidity = new Array();
var temperature = new Array();

// Start by extracting YOUR FOLDER/ url from YOUR s3 bucket
$.ajax({
    type     : "GET",
    url      : "https://<YOUR-IOT-DATA-BUCKET-NAME>.s3.amazonaws.com/",
    dataType : "xml",
    success  : function(xmlData){
        var url = "";
        keys = xmlData.getElementsByTagName("Key");
        for(i=0; i < keys.length; i++) {
            url = keys[i].childNodes[0].nodeValue;

            // Find your locate folder2 string 
            pos = url.search("<YOUR-FOLDER>/")
            if(pos > -1) {
                // Detect folder2 with json file
                if(url.replace("<YOUR-fOLDER>/", "") != "")
                    parseJson(url)
            }
        }
        // console.log(uptime)
        // console.log(temperature)
        // console.log(humidity)
        loadChart(uptime, temperature, humidity)
    },
    error    : function(){
         alert("Could not retrieve XML file.");
    }
});

// Parse JSON files
var parseJson = function(url) {
    $.ajax({
        type : "GET",
        url : "https://<YOUR-IOT-DATA-BUCKET-NAME>.s3.amazonaws.com/" + url,
        dataType : "json",
        success : function(jsonFile) {
            uptime.push(jsonFile.uptime);
            temperature.push(jsonFile.temperature)
            humidity.push(jsonFile.humidity)
        },
        error : function(xhr, status, error) {
            console.error("JSON error: " + status);
        }
    })
}
