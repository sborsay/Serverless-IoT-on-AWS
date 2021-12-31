import json
import boto3
#import logging

URL = "https://q3ayqgi3l1.execute-api.ap-southeast-1.amazonaws.com/production"
client = boto3.client("apigatewaymanagementapi", endpoint_url = URL)

ccc = ""  # global varible as empty string, tested to stay resident through invocations 



def lambda_handler(event, context):
    
    #cid = "KkFOxeqhSQ0CEHQ=" # keep global so extract from function once
    
    default_Message = json.dumps({ "message": "Hello from lambda, no payloads yet"})
    test_payload = json.dumps({"temperature": 55, "humidity": 66, "timestamps":1984}) #works
    #can make to topics "connectID_Topic", #"payload_Topic" make two rule/actions for incoming topics
    #how to retian old connection ID
    #if message has connection ID then establosh websocket , else just print event/message/payload
    print(event)
    #event2 = json.dumps(event)
    
    
#-----------------------
    key = 'humidity'
    # python check if key in dict using "in"
    if key in json.dumps(event):
       print(f"Yes, key: '{key}' exists in dict/json")
       event = json.dumps(event)
    elif type(event) == str:
        print(f'{event} is a string, found connectionID')
        global ccc 
        ccc = event
        print(f'the: {ccc} is a string also, found connectionID')
        #set global connection ID from function so it is retained
        #set connection ID but not data
        #quit()
    else:
        print(f"I have no idea what: {event} is")
        quit()
        
    
    #--------------------------
    
    
    #You can try to do json.loads(), which will throw a ValueError if the string you pass can't be decoded as JSON.
    #if key value == temperture then execute event to publish
    #else execute below function
    #event2 = json.dumps(event) # works
    #connectionId = json.dumps(event) #takes object and retuns a string
    
    #print(f"this is my connect idea after dumps {connectionId}")
    
    #xxx = (f"Your WS connection ID for this session is {connectionId}") #print from if else connectionid
    xxx = (f"Your WS connection ID for this session is {ccc}") #print from if else connectionid
    # yy = event # if json objet has temerature key
    #if event["requestContext"] is not None:
     #   msg1 = json.dumps(event["requestContext"].get("connectionId"))
     #response = client.post_to_connection(ConnectionId = event["requestContext"].get("connectionId"), Data = json.dumps(event)) #message was msg
     #   print("connetion ID is set")
     #   print(event)
    #else:
    #    print("in else statement")
    #    response = client.post_to_connection(Data = json.dumps(event)) #message was msg
    #    print("response dumped")
    #    print(event)
    #response = client.post_to_connection(ConnectionId=event, Data=xxx)
    #client.post_to_connection(ConnectionId=ccc, Data= event) #  #response = client.post_to_connection(Data=b'bytes'|file, ConnectionId='string')
    #P2C function only accepts keywork arguments
    response=client.post_to_connection(Data=event, ConnectionId=ccc)
    print(r)
    #connectionId = ["J1ixZeaeyQ0CG1g="] #must manually enter because https not wss internal
    #connectionId = context['requestContext']['connectionId'];
    #/message = json.loads(event['requestContext']['connectionId']);
    #connection_id = event["requestContext"].get("connectionId")
    #connectionID = event["requestContext"].get("connectionId")
    #json.loads(event.get("body", ""))
   
    #message = event['Records'][0]['Sns']['Message']
    #print("From SNS: " + message) #message is just connectionID
    #print(event);
    #ctx = event.get("requestContext", {})
    # get user and connection id
    #conn_id = ctx.get("connectionId")
    #print("Printed after 2.4 seconds.")
    #print("$context.connectionId")
   
    #for connection in connectionId:
    #  response = client.post_to_connection(ConnectionId = connection, Data = json.dumps(msg)) #message was msg
    #response = client.post_to_connection(ConnectionId = message, Data = json.dumps(msg1)) #message was msg
