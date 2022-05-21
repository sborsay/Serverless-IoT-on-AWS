AWS CLI commands for a Public Bucket

Step 3 – Creating a S3 Bucket from the AWS CLI
Step 4 – Enabling a Static Web Host in the S3 bucket from the AWS CLI
Step 5 – Deleting the S3 Bucket and all files with a single AWS CLI command


Step 3 – Creating a S3 Bucket from the AWS CLI
               

When using the CLI the bucket will default to your “home” region in which your AWS CLI is configured.  If you make a bucket, as we did in the console, outside your home region, then you will have to use different commands with he ‘s3API’ as I demonstrate below for creating another bucket in us-west-2.

Create a new bucket with a public ACL

•	Warning: Do not store private information in public buckets, all info is publicly readable

Create a public bucket with:

```
aws s3api create-bucket --bucket <Your_Globally_Unique_Bucket_Name> --acl public-read
```

To create a bucket in another (non-home) region:

```
aws s3api create-bucket --bucket <Your-Bucket-Name> --region <AWS-Region> --create-bucket-configuration LocationConstraint=<AWS-Region> --acl public-read

```
 
 Here I create a bucket called “testbucket520cli” in us-west-2
 
 ![image](https://user-images.githubusercontent.com/16296900/169628656-03c1c4d1-07ca-49eb-927a-95a97ce1b9f9.png)



Check the result in S3 for my new public bucket:


 ![image](https://user-images.githubusercontent.com/16296900/169628663-ad715a0a-eaf9-4cff-aedd-ac4355ed6d49.png)



Now we need to create and save the following file in the same local folder on your computer that you are using the AWS CLI.  So in my case ill navigate to a policys folder on my desktop which contains my policys.  

I will have two bucket policy’s, you can choose either one, they are the same two from the console.  One is a public read bucket and the other is a IP limited read bucket policy.  Both should be saved as .JSON files:


File 1 save as: “public_policy.json” after filling out the ARN expression for your bucket name:
```JSON
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Sid": "PublicRead",
            "Effect": "Allow",
            "Principal": "*",
            "Action": "s3:GetObject",
            "Resource": "<YOUR-BUCKET-ARN-EXPRESSION>/*"
        }
    ]
}
```

If you want to use the IP limited policy for extra free security then save as: “ip_limited_policy.json”after filling out the ARN expression for your bucket name:

```JSON
{
    "Version": "2012-10-17",
    "Id": "somethingunique",
    "Statement": [
        {
            "Sid": "IPAllow",
            "Effect": "Allow",
            "Principal": "*",
            "Action": "s3:GetObject",
            "Resource": "arn:aws:s3:::<YOUR-BUCKER-NAME-HERE>/*",
            "Condition": {
                "IpAddress": {
                    "aws:SourceIp": "<YOUR-IP-HERE>/24"
                }
            }
        }
    ]
}
```


Upload the file to S3 with the following command


```
aws s3api put-bucket-policy --region <Your-Region>  --policy file://ip_limited_policy.json --bucket <Your-Bucket-Name>
```

 ![image](https://user-images.githubusercontent.com/16296900/169628708-15b174bb-dfc4-4899-87d3-a71b1f245940.png)


The proper response from the AW CLI is no response.  Go to your bucketPermissionsbucket Policy and make sure it was installed.

Now it is time to upload our CORS policy.  However, unlike the bucket policy the CORS policy must be in a different format that what we used in the console.  Create and save the following file in the same local folder on your computer that you are using the AWS CLI from_

File 2 save as:”cors.json”

```JSON
{
    "CORSRules": [
      {
        "AllowedOrigins": ["*"],
        "AllowedHeaders": ["Authorization"],
        "AllowedMethods": ["GET"],
        "MaxAgeSeconds": 4000
      }
    ]
  }
```

Upload the file to S3 with the following command_

```
aws s3api put-bucket-cors --cors-configuration file://cors.json --bucket <Your-Bucket-Name> --region <Your-Region>
```

 
![image](https://user-images.githubusercontent.com/16296900/169628727-18e6550b-128f-4c75-a9b7-3c67783416c1.png)



Step 4 – Enabling a Static Web Host in the S3 bucket from the AWS CLI

To enable static web hosting on your public bucket, enter the following command:

aws s3 website s3://<Your-Bucket-Name> --region <Your-Bucket-Region> --index-document index.html

 ![image](https://user-images.githubusercontent.com/16296900/169628738-8321f964-efae-4e7f-9c72-61f89f68323f.png)



Create and save the following file in the same local folder on your computer that you are using the AWS CLI from:

File 3 save as: “index.html”

 ```html
<!DOCTYPE html>
<html>
   <body>
      <h1>Hello World</h1>
      <p>My S3 static webhost, add HTML and JavaScript files as needed</p>
   </body>
</html>
```

Upload the index.html file to S3 with the following command

 ```
aws s3 cp index.html s3://<Your_Globally_Unique_Bucket_Name_Here> --region <Your-AWS-Region>
```
 
![image](https://user-images.githubusercontent.com/16296900/169628757-c3fc199f-953d-4edc-9d8b-c3be4aaf81e0.png)


There is no multifile upload other than recursive within a folder which would upload all local files.


To see your new website for your bucket in your AWS CLI home region navigate to:

 ```
https://<Bucket-Name>.s3.<AWS-Region>.amazonaws.com/index.html
```
 ![image](https://user-images.githubusercontent.com/16296900/169628784-8c8eac74-4613-40be-a7aa-45b6bb82c1a3.png)


or go to:

 ![image](https://user-images.githubusercontent.com/16296900/169628789-cb53ff72-fd34-4656-876b-27f1f5b7bf55.png)


 Right click on the Object URL for your website

Step 5 – Deleting the S3 Bucket and all files with a single AWS CLI command


Remove your bucket and all files in your bucket:
 
```
aws s3 rb s3://<Your-Bucket-Name> --region <Your-AWS-Region> --force
```
 ![image](https://user-images.githubusercontent.com/16296900/169628797-1aebfdd1-527e-4c78-87d7-b0a403824e89.png)

 
This recursively removes all files and then deletes your bucket.

 


