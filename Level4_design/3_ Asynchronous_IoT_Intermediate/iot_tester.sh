#!/bin/bash

mqtttopic='iot/ggg'
iterations=10
wait=2
region='us-east-1'
profile='default'

for (( i = 1; i <=$iterations; i++)) {

#added these randomizers because old ones didnt generate good numbers
    
    #Temperature in Fehr
    minT=-20
    maxT=120
    numberT=$(expr $minT + $RANDOM % $maxT)

    #humidity % cannot exceed 100
    minH=0
    maxH=100
    numberH=$(expr $minH + $RANDOM % $maxH)

  #CURRENT_TS=`date +%s`
  #DEVICE="P0"$((1 + $RANDOM % 5))
  #FLOW=$(( 60 + $RANDOM % 40 ))
  #TEMP=$(( 15 + $RANDOM % 20 ))
  #HUMIDITY=$(( 50 + $RANDOM % 40 ))
  #VIBRATION=$(( 100 + $RANDOM % 40 ))
  temperature=$(($numberT ))
  humidity=$(($numberH ))

  # 3% chance of throwing an anomalous temperature reading
  #if [ $(($RANDOM % 100)) -gt 97 ]
  #then
  #  echo "Temperature out of range"
   # TEMP=$(($TEMP*6))
  #fi

  echo "Publishing message $i/$ITERATIONS to IoT topic $mqtttopic:"
  #echo "current_ts: $CURRENT_TS"
  #echo "deviceid: $DEVICE"
  #echo "flow: $FLOW"
  echo "temperature: $temperature"
  echo "humidity: $humidity"
  #echo "vibration: $VIBRATION"

 #use below for AWS CLI V1
 #aws iot-data publish --topic "$mqtttopic" --payload "{\"deviceid\":\"$DEVICE\",\"current_ts\":$CURRENT_TS,\"flow\":$FLOW,\"temp\":$TEMP,\"humidity\":$HUMIDITY,\"vibration\":$VIBRATION}" --profile "$profile" --region "$region"
 
 #use below for AWS CLI V2
 aws iot-data publish --topic "$mqtttopic" --cli-binary-format raw-in-base64-out --payload "{\"temperature\":$temperature,\"humidity\":$humidity}" --profile "$profile" --region "$region"

  sleep $wait
}
