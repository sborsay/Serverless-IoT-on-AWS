# Create a new bucket with a public ACL

```
aws s3api create-bucket --bucket <Your_Globally_Unique_Bucket_Name_Here> --acl public-read
```

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

```
aws s3api put-bucket-policy --policy file://policy.json --bucket <Your_Globally_Unique_Bucket_Name_Here>
```


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



