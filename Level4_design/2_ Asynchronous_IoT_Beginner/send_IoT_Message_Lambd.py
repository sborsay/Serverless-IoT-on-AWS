#Don't forget to add 'ExecuteAPI' to your permissions as inline policy for this lambda function

import json
import boto3

 Websocket_HTTPS_URL = "<Your-Websockets-Connection-Endpoint-Here>"
 client = boto3.client("apigatewaymanagementapi", endpoint_url = Websocket_HTTPS_URL)
 
 Test_Message = json.dumps({ "message": "Hello from lambda, hardcoded test message"})

def lambda_handler(event, context):
    print(event)
    connectionId = "<Your-ConnectionID-From-CloudWatch-Here>" #must manually enter for this beginner version
    IoT_Message = json.dumps(event)
    #AWS API Gateway API's require 'key=value' arguments
    #https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/apigatewaymanagementapi.html#ApiGatewayManagementApi.Client.post_to_connection
    response = client.post_to_connection(ConnectionId = connectionId, Data = IoT_Message)
