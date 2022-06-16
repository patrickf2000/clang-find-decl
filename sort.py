#!/usr/bin/python3
import csv

# We are interested in these columns: 10, 9, 8, 7

row_index = 0

data = list()
header = list()

def row_exists(input_row):
	data7 = input_row[7]
	data8 = input_row[8]
	data9 = input_row[9]
	data10 = input_row[10]
	for row in data:
		i7 = row[7]
		i8 = row[8]
		i9 = row[9]
		i10 = row[10]
		if data7 == i7 and data8 == i8 and data9 == i9 and data10 == i10:
			return True
	return False

with open("output.csv", newline="\n") as csvfile:
	reader = csv.reader(csvfile, delimiter=',')
	for row in reader:
		if row_index == 0:
			header = row
			row_index = 1
			continue
		if row_exists(row) == False:
			data.append(row)
			
# Now write
with open("sorted_output.csv", "w", newline="\n") as csvfile:
	writer = csv.writer(csvfile, delimiter=",")
	writer.writerow(header)
	for row in data:
		writer.writerow(row)

