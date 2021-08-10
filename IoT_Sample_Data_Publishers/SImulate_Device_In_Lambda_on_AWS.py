# Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT-0
    
# Script to generate simulated IoT device parameters data

import json
import random
import datetime
import boto3
import time

## Init AWSIoTMQTTClient


deviceNames = ['SBS01', 'SBS02', 'SBS03', 'SBS04', 'SBS05']

iot = boto3.client('iot-data');  #iot = boto3.client('iot-data');  #'a3vvvvvvvees.iot.us-east-2.amazonaws.com'
#    myAWSIoTMQTTClient.configureEndpoint(host, port)
#iot.configureEndpoint('a32qvvvvvvyees-ats.iot.eu-west-1.amazonaws.com', 8883)


# generate Flow values
def getFlowValues():
    data = {}
    data['deviceValue'] = random.randint(60, 100)
    data['deviceParameter'] = 'Flow'
    data['deviceId'] = random.choice(deviceNames)
    data['dateTime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return data

# generate Temperature values
def getTemperatureValues():
    data = {}
    data['deviceValue'] = random.randint(15, 35)
    data['deviceParameter'] = 'Temperature'
    data['deviceId'] = random.choice(deviceNames)
    data['dateTime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return data

# generate Humidity values
def getHumidityValues():
    data = {}
    data['deviceValue'] = random.randint(50, 90)
    data['deviceParameter'] = 'Humidity'
    data['deviceId'] = random.choice(deviceNames)
    data['dateTime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return data

# generate Sound values
def getSoundValues():
    data = {}
    data['deviceValue'] = random.randint(100, 140)
    data['deviceParameter'] = 'Sound'
    data['deviceId'] = random.choice(deviceNames)
    data['dateTime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return data

# Generate each parameter's data input in varying proportions
while True:
    time.sleep(1)
    rnd = random.random()
    if (0 <= rnd < 0.20):
        data = json.dumps(getFlowValues())
        print(data)
        response = iot.publish(
             topic='xxx',     #var AWS = require('aws-sdk');
                                               #var iotdata = new AWS.IotData({endpoint: 'a32-2.amas.com' }); #return iotdata.publish(params,
             payload=data
        ) 
    elif (0.20<= rnd < 0.55):
        data = json.dumps(getTemperatureValues())
        print(data)
        response = iot.publish(
             topic='xxx',
             payload=data
        )
    elif (0.55<= rnd < 0.70):
        data = json.dumps(getHumidityValues())
        print(data)
        response = iot.publish(
             topic='xxx',
             payload=data
        )
    else:
        data = json.dumps(getSoundValues())
        print(data)
        response = iot.publish(
             topic='xxx',
             payload=data     
)
