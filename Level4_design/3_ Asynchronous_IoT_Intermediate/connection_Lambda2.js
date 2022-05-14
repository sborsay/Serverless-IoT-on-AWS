//Extra Permission required:   SSM - Systemems Manager (search 'system' in inline policys)

const AWS = require('aws-sdk') 

var mySSM_Client = new AWS.SSM();   //create new client object of System Manager Class

exports.handler = async (event, context) => {
    
 console.log(event); 
 let connectionId = event.requestContext.connectionId;
 console.log("myConnectionID is: ", connectionId)
 
//-----------------Begin SSM Code

//https://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/SSM.html#putParameter-property
var params = {
  Name: '<Insert-Your-SSM-Parameter-Name-Here>',
  Value: connectionId,
  Overwrite: true  //not required but default is False
};

//await and promise() stub are not documented but neccessary for function to work - UNFORTUNATELY
var mySSM_request = await mySSM_Client.putParameter(params, function(err, data) {
  if (err) console.log(err, err.stack); // an error occurred
  else     console.log("success: ", data);           // successful response
}).promise()
//var mySSM_request = await mySSM_Client.putParameter(params).promise(); //should also work; sans padantic if err else

console.log("My request: ", mySSM_request)

//------------------End SSM Code

     const response = {
        statusCode: 200,
        body: JSON.stringify('Hello from Lambda!'),
    };
    return response;  //need a response or we get disconnected immediatly
 
};
