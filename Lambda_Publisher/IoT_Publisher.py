#https://iotespresso.com/publishing-from-lambda-to-an-aws-iot-topic/

import json
import boto3

client = boto3.client('iot-data', region_name='us-east-2')



def lambda_handler(event, context):
    print(event)
    # TODO implement
    # Change topic, qos and payload
    response = client.publish(
        topic='iot/fromLambda',
        qos=1,
        payload=json.dumps({"foot":"ball"}) #from web input via APi Gateway:  payload=json.dumps(event['queryStringParameters'])
    )
    print(response)
    
    return {
        'statusCode': 200,
        'body': json.dumps('Published to topic')
    }
