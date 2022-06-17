#Using 'conda_chainer_p36'  

import boto3
import pandas as pd
from matplotlib import pyplot as plt

# create IoT Analytics client
client = boto3.client('iotanalytics')

#after running the code portion above run the code below with your own dataset


dataset = "YOUR_DATASET_HERE"
dataset_url = client.get_dataset_content(datasetName = dataset)['entries'][0]['dataURI']

# start working with the data

df = pd.read_csv(dataset_url)
df.serversidetimestamp = pd.to_datetime(pd.to_numeric(df.serversidetimestamp), unit='ms')
df.set_index(df.serversidetimestamp, inplace=True)
#df.sort_values('serversidetimestamp', inplace=True)

fig, ax = plt.subplots()
df.temperature.plot(legend=True)
df.humidity.plot(legend=True)
plt.show()

print('Average Humidity: ', df.humidity.mean())
print('Number of samples: ', len(df))

print('Average temperature: ', df.temperature.mean())
print('Number of samples: ', len(df))
