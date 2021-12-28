import json
import boto3


def lambda_handler(event, context):
    print(event) 
    #AWS API Gateway API's require 'key=value' arguments
    Websocket_HTTPS_URL = "<Your-Websockets-Connection-Endpoint-Here>"
    client = boto3.client("apigatewaymanagementapi", endpoint_url = Websocket_HTTPS_URL)
    Test_Message = json.dumps({ "message": "Hello from lambda, hardcoded test message"})
    IoT_Message = json.dumps(event)
    connectionId = "<Your-ConnectionID-From-CloudWatch-Here>" #must manually enter for this beginner version
    response = client.post_to_connection(ConnectionId = connectionId, Data = IoT_Message)
