
const AWS = require('aws-sdk');

const api = new AWS.ApiGatewayManagementApi({
  endpoint: 'q3ayqgi3l1.execute-api.ap-southeast-1.amazonaws.com/production'
})


//const api =
//new AWS.ApiGatewayManagementApi({endpoint: 'q3ayqgi3l1.execute-api.ap-southeast-1.amazonaws.com/production'});


const iotdata = new AWS.IotData({endpoint: 'a32qaa131oyees-ats.iot.ap-southeast-1.amazonaws.com' }); //added 1216



//console.log("my APII:", api)
//console.log("my eventsdb:",event)  destroys websocket

// const connectionId = event.requestContext.connectionId //destroys websocket
// console.log("my sdbid:", connectionId)


//remove asych and promise to prevent () double publsih

//using asych here and in return/romise function works to establish persistetnt connection
//however it sends topic twice
//making function nonasych to send topic once by removing async  and promise()
//means external websocket connection will never work
    

exports.handler =  async (event,context) => {
    var connectionId = event.requestContext.connectionId;
    console.log("myConnectionvID1: ", connectionId);
     console.log("event obj: ", event);
    
    /*
        const response = {
        statusCode: 200,
        body: JSON.stringify('Hello from Lambda!'),
    };
    return response;
    
    */
    
   // const connectionId = event.requestContext.connectionId;
    //console.log("The connectionId  is: " + JSON.stringify(connectionId));



   var params = {
       topic: 'outTopic',
       payload: JSON.stringify(connectionId),
       qos: 0
  }
  
  return {
    body: JSON.stringify(await publishMessage(params))
  }
}


const publishMessage = async (params) => {
  return new Promise((resolve, reject) => {
    iotdata.publish(params, function(err, data){
      if(err){
        console.log(err);
        reject(err)
      }
      else{
        console.log("success?");
        resolve(params)
      }
    })
  })
   
   /*
    var params = {
        topic: "outTopic", //intopic_thing_7
        payload: JSON.stringify(connectionId),           //event.Temperature, event.Humidity, event.Time, event.Device_Id
        qos: 0
    };


     return  iotdata.publish(params, function(err, data) {
        if (err) {
            console.log("If error: " + JSON.stringify(err));
        }
        else {
            console.log("Success");
        }
    }).promise(); //https://aws.amazon.com/blogs/developer/support-for-promises-in-the-sdk/
    
    //return api.postToConnection(params))//.promise();
   //await iotdata.publish(params);
   

};
 */   


 //const connectionId = event.requestContext.connectionId;
// console.log("myConnectionID1: ", connectionId);
 // Create publish parameters fo sns to sendMessage 
 //https://docs.aws.amazon.com/sdk-for-javascript/v2/developer-guide/sns-examples-publishing-messages.html
 
 /*
var params = {
  Message: 'hello ', 
  TopicArn: 'arn:aws:sns:ap-southeast-1:125831447646:sendConnectID',
//  Endpoint: 'arn:aws:lambda:ap-southeast-1:125831447646:function:dummysnsreceive'
// console.log("my message3: ", Message)
};
console.log("my params:", params);
// Create promise and SNS service object
var publishTextPromise = new AWS.SNS({apiVersion: '2010-03-31'}).publish(params).promise();
console.log("publishTextPromise: ", publishTextPromise);

publishTextPromise.then(
  function(data) {
    console.log(`Message ${params.Message} sent to the topic ${params.TopicArn}`);
    console.log("MessageID is " + data.MessageId);
  }).catch(
    function(err) {
    console.error(err, err.stack);
  });
  
 
 //add in lambda code for to send payload to sendMessage on success
 //  this reults in at _homogeneousError 
 if (event.requestContext.connectionId) {
        console.log("received connection ID2", connectionId);
        context.callbackWaitsForEmptyEventLoop = false;
        callback(connectionId);
 }
//end add in invoke code        
 */
 
 
 //add now dummysend code here
   // var eventText = JSON.stringify(event.requestContext.connectionId, null, 2); 
    //console.log("Received event:", eventText);
    //console.log("Received eventx:", event.requestContext.connectionId);
   
    //console.log("The event object is: " + JSON.stringify(event));
    

}
