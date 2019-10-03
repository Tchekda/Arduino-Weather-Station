from influxdb import InfluxDBClient
import numpy as np
import pandas as pd
import joblib
from sklearn.tree import DecisionTreeRegressor

def make_date(date):
    new_number = int(date
    .replace('-', '')
    .replace('T', '')
    .replace(':', '')
    .replace('Z', '')
    )
    return new_number

def get_data():
    client = InfluxDBClient(host='localhost', port=8086)
    client.switch_database('telegraf')

    result = client.query("SELECT * FROM weather")

    data = result.get_points(tags={'location':'inside'})

    return data

def train(data):
    np_array = []
    for point in data:
        point_array = [make_date(point['time']), point['temperature'], point['light']]
        np_array.append(point_array)

    np_data = np.array(np_array)

    dataset = pd.DataFrame({'time': np_data[:, 0], 'temperature': np_data[:, 1], 'light': np_data[:, 2]})
    
    print(dataset)

    tree_model = DecisionTreeRegressor()

    tree_model.fit(dataset.drop('temperature', axis=1), dataset['temperature'])

    joblib.dump(tree_model, 'weather_predictor.pkl')

    print("-" * 48)
    print("\nDone training\n")
    print("-" * 48)

def predict_weather():
    tree_model = joblib.load('weather_predictor.pkl')

    print("-" * 48)
    print("Enter the details of the date you would like to predict")
    print("\n")
    year = input("Year: ")
    month = input("Month number (00): ")
    theday = input("Day number (00): ")
    hour = input("Hour (00): ")
    minute = input("Minute (00): ")
    light = int(input("Luminosity : "))

    day = int(str(year) + str(month) + str(theday) + str(hour) + str(minute) + '00')

    predict_data = [
        day, 
        light
    ]

    temp = tree_model.predict([predict_data])[0]
    print("-" * 48)
    print("\nThe temperature is estimated to be: " + str(temp) + "\n")
    print("-" * 48)


def run_menu():
    print("*" *48)
    print("-" *10 + " What would you like to do? " + "-" * 10)
    print("\n")
    print("1. Predict the weather on a specific time")
    print("3. Train data")
    print("9. Quit")
    print("\n")

    option = input("Enter option: ")

    while True:
        if option != '':
            option = int(option)
            break
        option = input("Enter option: ")
    return option

if __name__== "__main__":
    # X is our input variables that will be used to predict y which is our output so temperature
    # the data in X needs to be converted to numeric values to simplify our process
    data = get_data()
    while True:
        option = run_menu()
        if option == 9:
            break
        elif option == 3:
            train(data)
        elif option == 1:
            predict_weather()
        else:
            print("*" *48)
            print("-" *10 + " Command Unknown " + "-" * 10)
            print("*" *48)
