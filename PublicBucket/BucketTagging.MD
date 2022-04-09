//do this command per file you want tagged
--------------------------
aws s3api put-bucket-tagging --bucket my-bucket --tagging file://tagging.json

//save file
{
   "TagSet": [
     {
       "Key": "public",
       "Value": "yes"
     }
   ]
}
-----------------------
//https://docs.aws.amazon.com/cli/latest/reference/s3api/put-bucket-tagging.html
