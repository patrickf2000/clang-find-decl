#!/usr/bin/python3
import csv
import sys

if len(sys.argv) < 3:
	print("Error: Please specify input file and column start.")
	exit(1)

# Defines
#input_file = "output.csv"
#start = 7
input_file = str(sys.argv[1])
start = int(sys.argv[2])

# We are interested in these columns: 11, 10, 9, 8, 7

row_index = 0

data = list()
header = list()

def row_exists(input_row):
	#start = 7
	end = len(input_row) - 1
	#end = 11
	for row in data:
		unique = True
		for i in range(start, end):
			if row[i] != input_row[i]:
				unique = False
		if unique:
			return True
	return False

with open(input_file, newline="\n") as csvfile:
	reader = csv.reader(csvfile, delimiter=',')
	for row in reader:
		if row_index == 0:
			header = row
			row_index = 1
			continue
		if row_exists(row) == False:
			data.append(row)
			
# Now write
with open("sorted_" + input_file, "w", newline="\n") as csvfile:
	writer = csv.writer(csvfile, delimiter=",")
	writer.writerow(header)
	for row in data:
		writer.writerow(row)

