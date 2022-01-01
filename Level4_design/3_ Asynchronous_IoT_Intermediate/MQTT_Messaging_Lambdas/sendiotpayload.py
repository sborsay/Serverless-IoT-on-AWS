import json
import boto3

URL = "https://<Insert-Websocket_Https_Endpoint-Here>"
client = boto3.client("apigatewaymanagementapi", endpoint_url = URL)

Global_cId = ""  # global varible as empty string, tested to stay resident through invocations, used as connection_Id

def lambda_handler(event, context):
    
    default_Message = json.dumps({ "message": "Hello from lambda, no payloads yet"})
    test_payload = json.dumps({"temperature": 55, "humidity": 66, "timestamps":1984}) #works
    print(event)

    key = 'humidity'
    # python check if key in dict using "in"
    if key in json.dumps(event):
       print(f"Yes, key: '{key}' exists in dict/json")
       event = json.dumps(event)
    elif type(event) == str:
        print(f'{event} is a string, found connectionID')
        global Global_cId  #set global varible for connection ID, Global space to stay resident
        Global_cId = event
        print(f'the: {Global_cId} is a string also, found connectionID')
        #set global connection ID from function so it is retained
        #set connection ID but not data
        #quit()
    else:
        print(f"I have no idea what: {event} is")
        quit()

    response=client.post_to_connection(Data=event, ConnectionId=Global_cId)
