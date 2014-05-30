import csv

r = csv.reader(open('data/micaz_final.csv', 'rb'), delimiter=',')
w = csv.writer(open('data/micaz_final_good.csv', 'wb'), delimiter=',')

data = []
last_line = []

for line in r:
	if len(last_line) == 0:
		last_line = line
		continue
	if abs(eval(line[2]) - eval(last_line[2])) <= 1:
		data.append(line)
	last_line = line

for line in data:
	w.writerow(line)



