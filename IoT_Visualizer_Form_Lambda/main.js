
$(function() {    // all jquery should be in "$(function() { }) document ready function"
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

            loadChart(uptime, temperature, humidity)
            //--------------------------------------//

            for(i=0; i < keys.length; i++) {
                url = keys[i].childNodes[0].nodeValue;
    
                // Find your locate your nested folder string 
                pos = url.search("<Your-Nested-Folder>")  //mine was folder2
                if(pos > -1) {
                    // Detect nested folder with json file
                     if(url.replace("<YOUR-fOLDER>/", "") != "")  //mine was folder 2
                        parseJson(url)
                }
            }
            // loadChart(uptime, temperature, humidity)
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
            async:false,
            success : function(jsonFile) {
                uptime.push(jsonFile.uptime);
                temperature.push(jsonFile.temperature)
                humidity.push(jsonFile.humidity)

                //  $ajax by default asychrounous so, myChart.update() should update after get success valid respose
                myChart.update();
                //---------------------------------------//
            },
            error : function(xhr, status, error) {
                console.error("JSON error: " + status);
            }
        })
    }
})
