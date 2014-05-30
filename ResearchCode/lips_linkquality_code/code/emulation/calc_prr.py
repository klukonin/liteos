import csv

r = csv.reader(open('data/iris_final_good.csv', 'rb'), delimiter=';')
w = csv.writer(open('data/iris_input.csv', 'wb'), delimiter=',')

prr = 1.0
cur_id = 0

for line in r:
	id = eval(line[0]) + eval(line[1])*256 + eval(line[2])*256*256
	prr = (cur_id*prr + 1.0)/id
	cur_id = id
	w.writerow([prr, eval(line[4]), eval(line[5])])


