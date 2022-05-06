import csv
import random

dataCSVPath = '../Deamons/TSSData.csv'
ratesCSVPath = '../Deamons/TSSRates.csv'

with open(dataCSVPath, 'r') as infile:
    csv_reader = csv.reader(infile)

    with open(ratesCSVPath, 'w') as outfile:
        csv_writer = csv.writer(outfile)

        for line in csv_reader:
            if (line[0] == 'id'):
                csv_writer.writerow(['id','rate'])
            else:
                csv_writer.writerow([line[0], round(pow(random.random(),2), 5)])
