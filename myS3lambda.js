    var AWS = require('aws-sdk');
     var s3 = new AWS.S3();
   
        exports.handler = (event, context, callbac) => {  
            var bucketName = '<YOUR-BUCKET-NAME>/<YOUR-FOLDER-NAME>';//+ "/" + Date.now();
            var keyName = JSON.stringify(Date.now());
            var content = JSON.stringify(event);
        
            var params = { Bucket: bucketName, Key: keyName, Body: content };
        
            s3.putObject(params, function (err, data) {
                if (err)
                    console.log(err);
                else
                    console.log("Successfully saved object to " + bucketName + "/" + keyName);
            });
        };
