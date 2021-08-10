# clone project
git clone \
    --branch master --single-branch --depth 1 --no-tags \
    https://github.com/garystafford/aws-iot-analytics-demo.git


# publish sample messages to test cfn stack (5 messages)
cd sample_data   |   topic defailt is: 'iot-device-data'

python3 ./send_sample_messages.py -f raw_data_small.json -t iot-device-data

