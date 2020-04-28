//call this file 'index.js'
//AWS permission CLI directions at botton of page
//Payloads to test this Lambda with are in this github subdirectory

var weather = require("weather-js");
var cities = require('cities');
var AWS = require('aws-sdk');

exports.handler =  async function(event, context) {

let promiseData = [];

let weatherPromise = function(data){
  return new Promise((resolve,reject)=>{
    weather.find({search: `${data.State} ${data.City}`, degreeType: 'F'}, function(err, result) {
      if(err) console.log('error weather',err);

      let weatherTempArr = [];
      if(result) {
        result.forEach(item=>{
          console.log(item.current.temperature);
          weatherTempArr.push({
            temperature: item.current.temperature,
            humidity:item.current.humidity
          })
        });
      }
      data.weather = weatherTempArr;
      resolve(data);
    });
  })
}

event.forEach(message => {
  // do some stuff to `message`
  //use incoming lattitude nd longitude from event object to get city and state
  const obj= cities.gps_lookup(message.lat, message.long);
  
  //get ISO8601 date formate. ntively supported in browser JS
var d = new Date();
var n = d.toISOString();   //move timestamping function to local scope to prevent warm start, and the memory heap TSR issue

    /******************* get current weather by using city and state fromm cities.js *******************/
    let data = Object.assign({},message,{City:obj.city, State:obj.state, Time:n});
    promiseData.push(weatherPromise(data))
     /******************* end *******************/

}); //end for each
 
const responses = await Promise.all(promiseData);
return responses;

};

//Lambda pernissions from the AWS CLI, 'aws' or 'aws2' contingent on your AWS CLI edition
//example 1
//aws2 lambda add-permission --function-name YOUR-LAMBDA-FUNCTION --statement-id 34545 --principal iotanalytics.amazonaws.com --action lambda:InvokeFunction
//example2
//aws --region us-east-2 lambda add-permission --function-name YOUR-LAMBDA-FUNCTION --statement-id 12345 --principal iotanalytics.amazonaws.com --action lambda:InvokeFunction
