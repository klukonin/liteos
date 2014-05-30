#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
	freopen("sp-rssi.txt", "r", stdin);
	freopen("sprssi.txt", "w", stdout);
	int a, b;
	while(scanf("%d", &a) != EOF) {
		scanf("%d", &b);
		scanf("%d", &a);
		scanf("%d", &b);
		printf("%d	%d\n", a, b);
	}
	return 0;
}

