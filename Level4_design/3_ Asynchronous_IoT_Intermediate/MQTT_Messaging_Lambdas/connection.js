const AWS = require('aws-sdk');

const api = new AWS.ApiGatewayManagementApi({
  endpoint: '<Paste-Your-Websocket-Endpoint_Here-Remove WSS://>'
})

const iotdata = new AWS.IotData({endpoint: '<Paste-Your-IoT-Endpoint-Here>' }); 

//using asych here and in return/romise function works to establish persistetnt connection
//however it sends topic twice
//making function nonasych to send topic once by removing async  and promise()
//means external websocket connection will never work
    

exports.handler =  async (event,context) => {
    var connectionId = event.requestContext.connectionId;
    console.log("myConnectionvID1: ", connectionId);
     console.log("event obj: ", event);
    
 
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
}
