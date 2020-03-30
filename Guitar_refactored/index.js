 //new lambda for data enhancemant uptime to ISO8601 transformm for Devan Sabaratnam's Guitar Project
 //https://hackernoon.com/building-an-iot-dashboard-using-the-onion-omega-and-amazon-aws-a3520f850c9
 //Lambda added for course on serverless IoT on AWS at Udemy.com
 //call this Lmabda fnction index.js in Node for the aws-sdk
 
// Load the SDK for JavaScript
const AWS = require('aws-sdk');
// Set the Region 
const docClient = new AWS.DynamoDB.DocumentClient({region: '<YOUR-AWS-REGION-HERE>'});

exports.handler = function(event, context, callback) {
    
    //let myDate = new Date();
      let ISO_Date = new Date().toISOString();  //converted to a single line for the pedantics
                                               //moved time inside of the function to prevent
                                               //warm starts from reusuing old timestamp
   
    var params = {
        
      Item: {  
     //  PARTITION_KEY: {'S': 'id'},    
     //  SORT_KEY: {'N': 'timestamp'},
      
     id: "outTopic",
     timestamp:  Date.now(),
   
   payload:{ 
    
       "datetime":    ISO_Date, 
       "temperature": event.temperature,
       "humidity":    event.humidity
    
   },

 },
           
           TableName: 'myDDBTable'
        
    };
    
    
docClient.put(params, function(err, data) {
   if (err) callback(err, err.stack);      // an error occurred
   else     callback(null,data);           // successful response
    }
    );
};
