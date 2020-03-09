var uptime = new Array();
var humidity = new Array();
var temperature = new Array();

// Start by extracting your folder/key name url from s3 bucket
$.ajax({
    type     : "GET",
    url      : "https://<YOUR-BUCKET-NAME-HERE>.s3.<YOUR-AWS-REGION-HERE>.amazonaws.com/",
    dataType : "xml",
    success  : function(xmlData){
        var url = "";
        keys = xmlData.getElementsByTagName("Key");
        for(i=0; i < keys.length; i++) {
            url = keys[i].childNodes[0].nodeValue;

            // Find locate your folder/key string 
            pos = url.search("<YOUR-FOLDER-KEY-NAME-HERE>/")
            if(pos > -1) {
                // Detect your folder/key with json file
                if(url.replace("<YOUR-FOLDER-KEY-NAME-HERE>/", "") != "")
                    parseJson(url)
            }
        }
        loadChart(humidity, temperature, uptime )
    },
    error    : function(){
         alert("Could not retrieve XML file.");
    }
}).always(function() {
    console.log(uptime)
});

// Parse JSON files
var parseJson = function(url) {
    $.ajax({
        type : "GET",
        url : "https://<YOUR-BUCKET-NAME-HERE>.s3.<YOUR-AWS-REGION-HERE>.amazonaws.com/" + url,
        dataType : "json",
        success : function(jsonFile) {
            uptime.push(Number(jsonFile.uptime));
            temperature.push(Number(jsonFile.temperature))
            humidity.push(Number(jsonFile.humidity))
        },
        error : function(xhr, status, error) {
            console.error("JSON error: " + status);
        }
    })
}
