var AWS = require('aws-sdk');

var iotdata = new AWS.IotData({endpoint: '<YOUR-IOT-REGION-ENDPOINT>' });

exports.handler = async(event) => {
    console.log("The event object is: " + JSON.stringify(event));
    var params = {
        topic: "YOUR_TOPIC", //intopic_thing_7
        payload: JSON.stringify(event),           //event.Temperature, event.Humidity, event.Time, event.Device_Id
        qos: 0
    };

    return iotdata.publish(params, function(err, data) {
        if (err) {
            console.log("If error: " + JSON.stringify(err));
        }
        else {
            console.log("Success");
        }
    }).promise(); //https://aws.amazon.com/blogs/developer/support-for-promises-in-the-sdk/
};
