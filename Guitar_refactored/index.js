 //new lambda for data enhancemant uptime to ISO8601 transformm for Devan Sabaratnam's Guitar Project
 //https://hackernoon.com/building-an-iot-dashboard-using-the-onion-omega-and-amazon-aws-a3520f850c9
 //Lambda added for course on serverless IoT on AWS at Udemy.com
//call this Lmabda fnction index.js in Node for the aws-sdk
 
 // Load the SDK for JavaScript
const AWS = require('aws-sdk');

const docClient = new AWS.DynamoDB.DocumentClient({region: '<YOUR-REGION-HERE>'});
var myDate = new Date();
var ISO_Date = myDate.toISOString();

exports.handler = function(event, context, callback) {
    
    
    var params = {
        
      Item: {  
     //  PARTITION_KEY: {'S': 'id'},
     // SORT_KEY: {'N': 'timestamp'},
      
     id: "<YOUR-DynamoDB-ID-HERE>", //in our case we just used 'outTopic' from the device publish topic
     timestamp:  Date.now(),
   
   payload:{ 
    
       "datetime":  ISO_Date, 
       "temperature":   event.temperature, 
       "humidity":  event.humidity
    
   },

 },
           
           TableName: '<YOUR-DYNAMODB-TABLE-NAME-HERE>'
        
    };
    
    
docClient.put(params, function(err, data) {
   if (err) callback(err, err.stack);      // an error occurred
   else     callback(null,data);           // successful response
    }
    );
};
