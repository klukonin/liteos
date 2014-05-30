#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
	freopen("data2.txt", "r", stdin);
	freopen("input2.txt", "w", stdout);
	int a, b, c, d;
	double prr = 1.0;
	int cur_id = 0;
	while(scanf("%d", &a) != EOF) {
		scanf("%d", &b);
		scanf("%d", &c);
		scanf("%d", &d);
		int id = a + b*256;
		int rssi = c;
		int lqi = d;
		prr = (prr*cur_id + 1.0) / id;
		cur_id = id;
		printf("%d	%d	%lf\n", rssi, lqi, prr);
	}
	return 0;
}

