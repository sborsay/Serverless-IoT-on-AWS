

Data Privacy and Security

 we will make public websites for our IoT data and static websites.  Wouldn’t it be better to make everything private?  All things being equal yes, the problem is that true privacy adds extra cost as you would need to employ extra AWS services like CloudFront and Route 53.  If you want to truly privatize your data for more commercial use, there are many free videos and tutorials on making a website private with these AWS services. Also remember, the IoT data we use here isn’t inherently anything we need to keep private, just simple environmental readings and visualization code.
We can achieve pretty good privacy (“PGP”), through a simply bucket policy that limits IP access.  I will explain that later in this chapter.  The best form of privacy is not to release you bucket URL to the world
The second related consideration to data sensitivity and security is costs.  To explain this let’s use an example to make the issue clear.  Let’s say I have an S3 bucket with a static website that has a URL created with API Gateway that calls a Lambda function that Scans an entire DynamoDB database.  This can be a costly operation, so I clearly do not want anyone to get my URL as they can maliciously keep scanning my database and running up my AWS bill.  This is a situation in which I either have to pay for CloudFront to protect my service endpoint from malicious use if I plan to make my website publicly available or I simply keep my endpoint private, possibly using a IP limiting bucket policy so that will explain how to use an API key with your endpoints later which is a good solution.  An API Key restricts anyone using your endpoint without having a secret ‘key.”  

In this prerequisite project we will first create a public S3 bucket with the AW console and then use the express method with the AWS CLI.  At the end I will demonstrate a single CLI command to delete your bucket with all included files.


 


Step 1 – Creating a S3 bucket from the AWS Console
Step 2 – Enabling a Static Web Host in the S3 Bucket
Step 3 – Creating a S3 Bucket from the AWS CLI
Step 4 – Enabling a Static Web Host in the S3 bucket from the AWS CLI
Step 5 – Deleting the S3 Bucket and all files with a single AWS CLI command

Step 1 – Creating a S3 bucket from the AWS Console

•	Warning: Do not store private information in public buckets, all info is publicly readable

Navigate to S3 from your account or just use this link:

https://s3.console.aws.amazon.com/s3

On the upper right hand of the screen select “Create bucket”

 



Now I am going to name the bucket with a globally unique name.  The name has to be globally unique because you will have a unique URL to access it by.   Next, I will choose a bucket region that IS NOT my home region.  Again, my home region is the region my AWS CLI is configured for (default region), in this class I suggested using ‘us-east-1’ (N. Virginia).    I am doing this to demonstrate how to use the region flag in the CLI.  Also note, whenever you create new AWS services, make sure to keep all those services in the processing chain in the same region, even the S3 bucket.

Next, I am going to select “ACL’s enabled”

 

Next, unblock your S3 bucket and acknowledge you really want to make it publicly accessible.

 

Keep the rest of the defaults and then “Create bucket”

 
Going back to the buckets lit you can sort by creation date to easily find your new bucket rather then look for it alphabetically.

 


Open the bucket again and go to the permissions tab and press it.

 


We need to configure the permissions further by first entering a bucket policy.  Select the “Edit” button.

 


Now we have two options.  We can make a general “Read only policy” or a “Read only by IP” The IP policy will limit anyone who doesn’t have your IP from seeing any of your bucket data or code.  This is a great free way to limit access, however it is not foolproof as IP’s can be spoofed.  Also, this restricts you from using your buckets endpoints from being accessible to others outside your IP.  Finally, if you have a ISP provided dynamic IP that can change regularly unless you pay extra to your ISP for a static IP.  If you switch to your phones hotspot or Starbucks WiFI that is also going to be a new IP so you would have to adjust your bucket policy accordingly if not always on the same ISP.

Here is the first example of an open “read-only” (GET) bucket policy you can use.  You only need to copy your ‘ARN” and place it in the designated location and then save it.

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


Paste it in and copy the ARN expression where assigned:

 

Also make sure to leave the training ‘/*’ after the ARN expression.  This allows all the files within the bucket to also share the same bucket policy.

Here is the policy you can use for an IP limited read:

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


To find your IPv4 simply go to website like:  https://whatismyipaddress.com/

Next, we will edit the ACL list we enabled, select “Edit”

 

Now check the “List” and “Read” for “Everyone” and finally click the box acknowledging these changes.  AWS really wants to make sure you are OK with everyone readings your info.  However, since we only allowed “GET” access no one can actually write to (PUT) or delete data from your buckets.

 

Now save your changes to the ACL.

 

The last part of permissions is configuring the CORS policy.  CORS allows access across domains which some might want to restrict but we will keep open.  Select the “Edit” button:

 

Below is a generic policy you can copy and paste into the CORS editor:

[
    {
        "AllowedHeaders": [
            "Authorization"
        ],
        "AllowedMethods": [
            "GET"
        ],
        "AllowedOrigins": [
            "*"
        ],
        "ExposeHeaders": [],
        "MaxAgeSeconds": 4000
    }
]

This is especially easy because this statement does not require any customization.

Now select to “Save changes”

 

You have now finished making you bucket public and accessible from anywhere in the world.  Just make sure you do not put anything in your new bucket you do not want anyway one seeing.

Step 2 – Enabling a Static Web Host in the S3 Bucket

It is now time to add a static web host to our bucket.  The good news is that we can use the same bucket to both store IoT data and host a static website.  We can do one without the other as well, as they are not mutually exclusive.  Now return to the bucket base level and select the “Properties tab.”

 

Scroll all the way to the for “Static website hosting” and select “Edit” 

 .

We will keep things super simple, just “Enable” static website hosting, and then name your landing page “index.html”.   We will not worry about error actions or redirects as this book doesn’t cover website development or design.


 

Now press “Save changes”

 

Now let’s upload some a simple “Hello World” to test that our index.html works in the wild as a functional website.  Return to the top level and select the “Objects” tab.  For an index.html you can use the following code and then save it as “index.html” on your local computer.

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport"
     content="width=device-width, initial-scale=1, user-scalable=yes">
   <title>Hello World!</title>
</head>
<body>
Hello World in body!
</body>
</html>


The upload options are drag and drop or browse to the index.html file on your computer.  Either method is fine.  

 


 



Now click on your index.html file:

 

After clicking the index document, you will see the “Object URL” to open our index page.  You can now right click it and open this index.html in a new browser window

 

The result should now look like below if you used the same HTML code as I listed.

 

Wow super boring visual result but adding CSS and JavaScript will make our visualizations way better then this.  Anyway, Congratulations on your static webhost in a public bucket.  Except for this specific HTML code, you will be following this process in\ throughout this book for new and exciting visualizations of IoT data.  
Next, we move on to accomplishing all the same steps but with the AWS CLI.

Step 3 – Creating a S3 Bucket from the AWS CLI
               

When using the CLI the bucket will default to your “home” region in which your AWS CLI is configured.  If you make a bucket, as we did in the console, outside your home region, then you will have to use different commands with he ‘s3API’ as I demonstrate below for creating another bucket in us-west-2.

Create a new bucket with a public ACL
•	Warning: Do not store private information in public buckets, all info is publicly readable

Create a public bucket with 
aws s3api create-bucket --bucket <Your_Globally_Unique_Bucket_Name> --acl public-read

To create a bucket in another (non-home) region:

aws s3api create-bucket --bucket <Your-Bucket-Name> --region <AWS-Region> --create-bucket-configuration LocationConstraint=<AWS-Region> --acl public-read
Here I create a bucket called “testbucket520cli” in us-west-2

 

Check the result in S3 for my new public bucket:


  



Now we need to create and save the following file in the same local folder on your computer that you are using the AWS CLI.  So in my case ill navigate to a policys folder on my desktop which contains my policys.  

I will have two bucket policy’s, you can choose either one, they are the same two from the console.  One is a public read bucket and the other is a IP limited read bucket policy.  Both should be saved as .JSON files:


File 1 save as: “public_policy.json” after filling out the ARN expression for your bucket name:

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


If you want to use the IP limited policy for extra free security then save as: “ip_limited_policy.json”after filling out the ARN expression for your bucket name:

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



Upload the file to S3 with the following command



aws s3api put-bucket-policy --region <Your-Region>  --policy file://ip_limited_policy.json --bucket <Your-Bucket-Name>


 

The proper response from the AW CLI is no response.  Go to your bucketPermissionsbucket Policy and make sure it was installed.

Now it is time to upload our CORS policy.  However, unlike the bucket policy the CORS policy must be in a different format that what we used in the console.  Create and save the following file in the same local folder on your computer that you are using the AWS CLI from_

File 2 save as:”cors.json”

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

Upload the file to S3 with the following command_

aws s3api put-bucket-cors --cors-configuration file://cors.json --bucket <Your-Bucket-Name> --region <Your-Region>


 



Step 4 – Enabling a Static Web Host in the S3 bucket from the AWS CLI

To enable static web hosting on your public bucket, enter the following command:

aws s3 website s3://<Your-Bucket-Name> --region <Your-Bucket-Region> --index-document index.html

 


Create and save the following file in the same local folder on your computer that you are using the AWS CLI from:

File 3 save as: “index.html”

<!DOCTYPE html>
<html>
   <body>
      <h1>Hello World</h1>
      <p>My S3 static webhost, add HTML and JavaScript files as needed</p>
   </body>
</html>


Upload the index.html file to S3 with the following command

aws s3 cp index.html s3://<Your_Globally_Unique_Bucket_Name_Here>

 


There is no multifile upload other than recursive within a folder which would upload all local files.


To see your new website for your bucket in your AWS CLI home region navigate to:

https://<Bucket-Name>.s3.<AWS-Region>.amazonaws.com/index.html

 

or go to:

 

 Right click for your website

Step 5 – Deleting the S3 Bucket and all files with a single AWS CLI command


Remove your bucket and all files in your bucket

aws s3 rb s3://<Your-Bucket-Name> --region <Your-AWS-Region> --force

This recursively removes all files and then deletes your bucket.

 

Congratulations, 


