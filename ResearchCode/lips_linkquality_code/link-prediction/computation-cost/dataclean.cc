#include <iostream>
#include <cstdio>

using namespace std;

int main() {
	freopen("sp_prr.txt", "r", stdin);
	freopen("spprr.dat", "w", stdout);
	int a, b, c, d;
	while (scanf("%d%d%d%d", &a, &b, &c, &d) != EOF)
		printf("%lf\n", (c*256.0+d)/1000);
	return 0;
}

