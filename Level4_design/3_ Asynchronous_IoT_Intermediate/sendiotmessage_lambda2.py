#Don't forget to add both 'ExecuteAPI' and 'System' (AWS Systems Manger) to your permissions as inline policys for this lambda function

import json
import boto3
Websocket_HTTPS_URL = "<Insert-Https-Websocket-Endpoint-Here-With-Prefix>"

client = boto3.client("apigatewaymanagementapi", endpoint_url = Websocket_HTTPS_URL)
ssm_Client = boto3.client('ssm')

response_ssm = ssm_Client.get_parameter(Name='<Insert_Your_Parameter-Store-string_Here>')
connectionId =  response_ssm['Parameter']['Value']   #dig into the response blob to get our string value

Test_Message = json.dumps({ "message": "Hello from lambda, hardcoded test message"})


def lambda_handler(event, context):
    print(event) 
    IoT_Message = json.dumps(event)
    #AWS post_to_connection API requires 'key=value' arguments
    response = client.post_to_connection(ConnectionId = connectionId, Data = IoT_Message)
    
