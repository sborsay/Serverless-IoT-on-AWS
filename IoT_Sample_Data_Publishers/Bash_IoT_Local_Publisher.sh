#  code found at: https://aws.amazon.com/blogs/iot/integrating-iot-data-with-your-data-lake-with-new-aws-iot-analytics-features/
#  changed 1000 iterations to 10


#!/bin/bash

mqtttopic='iota/topic/myhome_raspberrypi'
iterations=10
wait=5
region='us-east-1'
profile='default'

for (( i = 1; i <= $iterations; i++)) {

  CURRENT_TS=`date +%s`
  DEVICE="P0"$((1 + $RANDOM % 5))
  FLOW=$(( 60 + $RANDOM % 40 ))
  TEMP=$(( 15 + $RANDOM % 20 ))
  HUMIDITY=$(( 50 + $RANDOM % 40 ))
  VIBRATION=$(( 100 + $RANDOM % 40 ))

  # 3% chance of throwing an anomalous temperature reading
  if [ $(($RANDOM % 100)) -gt 97 ]
  then
    echo "Temperature out of range"
    TEMP=$(($TEMP*6))
  fi

  echo "Publishing message $i/$ITERATIONS to IoT topic $mqtttopic:"
  echo "current_ts: $CURRENT_TS"
  echo "deviceid: $DEVICE"
  echo "flow: $FLOW"
  echo "temp: $TEMP"
  echo "humidity: $HUMIDITY"
  echo "vibration: $VIBRATION"

  aws iot-data publish --topic "$mqtttopic" --payload "{\"deviceid\":\"$DEVICE\",\"current_ts\":$CURRENT_TS,\"flow\":$FLOW,\"temp\":$TEMP,\"humidity\":$HUMIDITY,\"vibration\":$VIBRATION}" --profile "$profile" --region "$region"

  sleep $wait
}
