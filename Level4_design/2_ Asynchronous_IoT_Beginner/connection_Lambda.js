
const AWS = require('aws-sdk')

exports.handler = async (event, context) => {
  
 console.log(event); 
 let connectionId = event.requestContext.connectionId;
 console.log("myConnectionID is: ", connectionId)  //Go To AWS CloudWatch Service for your Connection ID
 
     const response = {
        statusCode: 200,
        body: JSON.stringify('Hello from Lambda!'),
    };
    return response;  //need a response or we get disconnected immediatly
 
};
