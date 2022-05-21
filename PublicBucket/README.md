useful tips:

https://aws.amazon.com/premiumsupport/knowledge-center/read-access-objects-s3-bucket/

https://emagalha.es/blog/2020/05/07/securing-s3-static-websites/



Step 1 – Creating a S3 bucket from the AWS Console

Step 2 – Enabling a Static Web Host in the S3 Bucket



![image](https://user-images.githubusercontent.com/16296900/169627735-20a15db4-e568-48af-9cb6-3415f55bedab.png)


Step 1 – Creating a S3 bucket from the AWS Console

•	Warning: Do not store private information in public buckets, all info is publicly readable

Navigate to S3 from your account or just use this link:

https://s3.console.aws.amazon.com/s3

On the upper right hand of the screen select “Create bucket”

![image](https://user-images.githubusercontent.com/16296900/169627857-791fad84-6c72-42d8-a0f9-7354f8c9172a.png)

 Now I am going to name the bucket with a globally unique name.  The name has to be globally unique because you will have a unique URL to access it by.   Next, I will choose a bucket region that IS NOT my home region.  Again, my home region is the region my AWS CLI is configured for (default region), in this class I suggested using ‘us-east-1’ (N. Virginia).    I am doing this to demonstrate how to use the region flag in the CLI.  Also note, whenever you create new AWS services, make sure to keep all those services in the processing chain in the same region, even the S3 bucket.

Next, I am going to select “ACL’s enabled”

 ![image](https://user-images.githubusercontent.com/16296900/169627837-b9bb0784-16e0-43b3-a343-cdfbf3deaa02.png)


Next, unblock your S3 bucket and acknowledge you really want to make it publicly accessible.

 ![image](https://user-images.githubusercontent.com/16296900/169627870-63fbc17c-512c-4362-b062-5b32c4155b23.png)


Keep the rest of the defaults and then “Create bucket”


![image](https://user-images.githubusercontent.com/16296900/169627872-1770bdc3-bb4e-40c0-b34c-70c1481cc26a.png)

 
Going back to the buckets lit you can sort by creation date to easily find your new bucket rather then look for it alphabetically.

 

Open the bucket again and go to the permissions tab and press it.

 
![image](https://user-images.githubusercontent.com/16296900/169627877-c8f826e6-790a-4d33-8419-f6e783bebe32.png)


We need to configure the permissions further by first entering a bucket policy.  Select the “Edit” button.

 
![image](https://user-images.githubusercontent.com/16296900/169627893-bfeeb40c-f488-4681-a5a4-e768daa832b8.png)


Now we have two options.  We can make a general “Read only policy” or a “Read only by IP” The IP policy will limit anyone who doesn’t have your IP from seeing any of your bucket data or code.  This is a great free way to limit access, however it is not foolproof as IP’s can be spoofed.  Also, this restricts you from using your buckets endpoints from being accessible to others outside your IP.  Finally, if you have a ISP provided dynamic IP that can change regularly unless you pay extra to your ISP for a static IP.  If you switch to your phones hotspot or Starbucks WiFI that is also going to be a new IP so you would have to adjust your bucket policy accordingly if not always on the same ISP.

Here is the first example of an open “read-only” (GET) bucket policy you can use.  You only need to copy your ‘ARN” and place it in the designated location and then save it.
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

Paste it in and copy the ARN expression where assigned:

![image](https://user-images.githubusercontent.com/16296900/169627922-b87da55f-8ec4-423e-acea-60523a96d042.png)


Also make sure to leave the training ‘/*’ after the ARN expression.  This allows all the files within the bucket to also share the same bucket policy.

Here is the policy you can use for an IP limited read:
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

To find your IPv4 simply go to website like:  https://whatismyipaddress.com/

Next, we will edit the ACL list we enabled, select “Edit”

![image](https://user-images.githubusercontent.com/16296900/169627942-5a5d722f-dd20-4328-b646-6a517a8d8fb5.png)

 
Now check the “List” and “Read” for “Everyone” and finally click the box acknowledging these changes.  AWS really wants to make sure you are OK with everyone readings your info.  However, since we only allowed “GET” access no one can actually write to (PUT) or delete data from your buckets.

 ![image](https://user-images.githubusercontent.com/16296900/169627977-fedb8dc6-210f-4670-ab14-9ae16830e752.png)


Now save your changes to the ACL.

 ![image](https://user-images.githubusercontent.com/16296900/169627992-785cd063-8471-43a8-bd17-141853952b4b.png)


The last part of permissions is configuring the CORS policy.  CORS allows access across domains which some might want to restrict but we will keep open.  Select the “Edit” button:

 ![image](https://user-images.githubusercontent.com/16296900/169628017-864ca045-35ca-4469-9d20-84f19e7c4bd6.png)


Below is a generic policy you can copy and paste into the CORS editor:
```JSON
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
```

This is especially easy because this statement does not require any customization.

Now select to “Save changes”

 ![image](https://user-images.githubusercontent.com/16296900/169628026-a5770955-2f82-4efc-aba1-09769a4ab2f5.png)


You have now finished making you bucket public and accessible from anywhere in the world.  Just make sure you do not put anything in your new bucket you do not want anyway one seeing.

Step 2 – Enabling a Static Web Host in the S3 Bucket

It is now time to add a static web host to our bucket.  The good news is that we can use the same bucket to both store IoT data and host a static website.  We can do one without the other as well, as they are not mutually exclusive.  Now return to the bucket base level and select the “Properties tab.”

 ![image](https://user-images.githubusercontent.com/16296900/169628039-8e3be95e-e14a-4cbf-a469-b66801f78b40.png)


Scroll all the way to the for “Static website hosting” and select “Edit” 

 .![image](https://user-images.githubusercontent.com/16296900/169628049-79cecc5d-8c30-4495-a057-c2af80d9e240.png)


We will keep things super simple, just “Enable” static website hosting, and then name your landing page “index.html”.   We will not worry about error actions or redirects as this book doesn’t cover website development or design.

![image](https://user-images.githubusercontent.com/16296900/169628060-8786cd22-ffb7-4e4c-a24f-adbcd93bc29e.png)

 

Now press “Save changes”

 ![image](https://user-images.githubusercontent.com/16296900/169628069-95f932c2-49c7-4f16-ae08-d44512059d8a.png)


Now let’s upload some a simple “Hello World” to test that our index.html works in the wild as a functional website.  Return to the top level and select the “Objects” tab.  For an index.html you can use the following code and then save it as “index.html” on your local computer.

```html
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
```

The upload options are drag and drop or browse to the index.html file on your computer.  Either method is fine.  

 
![image](https://user-images.githubusercontent.com/16296900/169628134-b416c3dc-dec8-4fd6-80bd-0d0e673e661f.png)


 
Now click on your index.html file:

![image](https://user-images.githubusercontent.com/16296900/169628112-6ab3999b-6faa-4769-b2fc-304b139b303b.png)

Then click on index.html

![image](https://user-images.githubusercontent.com/16296900/169628188-76cdaf59-f101-4ee7-85ea-d2f72d0587a5.png)



After clicking the index document, you will see the “Object URL” to open our index page.  You can now right click it and open this index.html in a new browser window

 ![image](https://user-images.githubusercontent.com/16296900/169628192-1602635b-24ff-4e5a-ab7d-8b47839b5d80.png)


The result should now look like below if you used the same HTML code as I listed.

 ![image](https://user-images.githubusercontent.com/16296900/169628200-25da3aa0-4de2-4dc5-b9ae-72db8ddc2671.png)


Wow super boring visual result but adding CSS and JavaScript will make our visualizations way better then this.  Anyway, Congratulations on your static webhost in a public bucket.  Except for this specific HTML code, you will be following this process in\ throughout this book for new and exciting visualizations of IoT data.  
Next, we move on to accomplishing all the same steps but with the AWS CLI.
