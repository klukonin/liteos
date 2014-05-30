import csv

r = open('data/micaz_final.txt', 'r')
w = csv.writer(open('data/micaz_final.csv', 'wb'), delimiter=',')

line = []
while True:
	s = r.read(2)
	if not s: break
	line.append(int(s, 16))
	if len(line) == 6:
		w.writerow(line)
		line = []


