        var AWS = require('aws-sdk');
        var s3 = new AWS.S3();
    
        exports.handler = (event, context, callback) => {  
            var bucketName = "<Your-Bucket-Name>/<Your-folder-Name>";
            var keyName = JSON.stringify(Date.now());
          //var content = JSON.stringify(event);                        //uncomment this statement for testing in lambda
          //var content = JSON.stringify(event.queryStringParameters);  //uncommnet this statement after integration with API Gateway
                                                                        //keep only one of the above uncommented!
        
        
            var params = { Bucket: bucketName, Key: keyName, Body: content};
        
            s3.putObject(params, function (err, data) {
                if (err)
                    console.log(err);
                else
                    console.log("Successfully saved object to " + bucketName + "/" + keyName 
                                                                + "and data=" + JSON.stringify(content));
            });
        };
