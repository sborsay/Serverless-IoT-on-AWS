import json
import boto3

 Websocket_HTTPS_URL = "<Your-Websockets-Connection-Endpoint-Here>"
 client = boto3.client("apigatewaymanagementapi", endpoint_url = Websocket_HTTPS_URL)
 connectionId = "<Your-ConnectionID-From-CloudWatch-Here>" #must manually enter for this beginner version
 
 Test_Message = json.dumps({ "message": "Hello from lambda, hardcoded test message"})


def lambda_handler(event, context):
    print(event) 
    IoT_Message = json.dumps(event)
    #AWS API Gateway API's require 'key=value' arguments
    response = client.post_to_connection(ConnectionId = connectionId, Data = IoT_Message)
  
    #Don't forget to add 'ExecuteAPI' to your permissions as inline policy for this lambda function
