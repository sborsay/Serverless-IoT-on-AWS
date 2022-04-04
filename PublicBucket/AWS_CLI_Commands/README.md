# Create a new bucket with a public ACL
### Warning: Do not store private information in public buckets, all info is publically readable

```
aws s3api create-bucket --bucket <Your_Globally_Unique_Bucket_Name_Here> --acl public-read
```
---

_Create and save the following file in the same local folder on your computer that ytou are using the AWS CLI from_

File 1 save as: **policy.json**

```json
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Sid": "Pubdf11145d",
            "Effect": "Allow",
            "Principal": "*",
            "Action": [
                "s3:GetObject",
                "s3:GetObjectVersion"
            ],
            "Resource": "arn:aws:s3:::<Your_Globally_Unique_Bucket_Name_Here>/*"
        }
    ]
}
```

_Upload the file to S3 with the following command_

---

```
aws s3api put-bucket-policy --policy file://policy.json --bucket <Your_Globally_Unique_Bucket_Name_Here>
```

_Create and save the following file in the same local folder on your computer that ytou are using the AWS CLI from_

File 2 save as: **cors.json**

```json
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

_Upload the file to S3 with the following command_

```
aws s3api put-bucket-cors --cors-configuration file://cors.json --bucket <Your_Globally_Unique_Bucket_Name_Here> 
```
---

## To use your Public Bucket as a static website


```
aws s3 website s3://<Your_Globally_Unique_Bucket_Name_Here> --index-document index.html
```

_Create and save the following file in the same local folder on your computer that ytou are using the AWS CLI from_

File 3 save as: **index.html**

```html
<!DOCTYPE html>
<html>
   <body>
      <h1>Hello World</h1>
      <p>My S3 static webshost, add HTML and JavaScript files as needed</p>
   </body>
</html>
```

_Upload the file to S3 with the following command_

```
aws s3 cp index.html s3://<Your_Globally_Unique_Bucket_Name_Here>
```

*There is no multifile upload other than recursive within a folder which would upload all local files[^1].

To see your new website for buckets in your AWS CLI home region naviagete to:

https://<Your_Globally_Unique_Bucket_Name_Here>.s3.amazonaws.com/index.html


![image](https://user-images.githubusercontent.com/16296900/158076079-9774e97b-e167-4af1-9794-676d65fe2e3a.png)

---


```diff
@@ Remove your bucket and all files in your bucket @@
```

```
aws s3 rb s3://<Your_Globally_Unique_Bucket_Name_Here> --force
```


