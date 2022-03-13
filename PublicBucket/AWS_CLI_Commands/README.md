Create a new bucket with a public ACL

```BASH
aws s3api create-bucket --bucket <Your_Globally_Unique_Bucket_Name_Here> --acl public-read
```


Save the following two files in a local folder on your computer

File 1 save as:  policy.json

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

File 2 save as: cors.json

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



