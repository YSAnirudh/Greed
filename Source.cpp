#include <iostream>
#include <algorithm>
#include <map>
#include <math.h>

using namespace std;

//O(log(n)) algorithm where it surely takes less than log(n) base 2 steps unless n is a power of 2
int factorsOf2(int n) {
	int count = 0;
	while (n % 2 == 0) {
		n /= 2; //dividing n by until it becomes odd => 2 is not a factor anymore
		count++;//counting no of factors of 2
	}
	return count;
}

//checks if the number is a power of 2
bool isPowOf2(int n) {
	while (n >= 1) {
		if (n == 1) {
			return true;
		}
		if (n % 2 == 0) {
			n /= 2;
		}
		else {
			return false;
		}
	}
	return false;
}

int B(int n, int m, map<int, int> Back, int* a, int k, int count) {
	if (n <= 0 || n > m || count > log2(n) + 1) {
		return 10000;
	}
	if (n == 1) {
		return 0;
	}
	if (isPowOf2(n)) {
		return Back[n];
	}
	int min = 0, temp;
	if (n % 2 == 0) min = B(n / 2, m, Back, a, k, count + 1);
	for (int i = 0; i < k; i++) {
		temp = B(n + a[i], m, Back, a, k, count + 1);
		if (temp < min) {
			min = temp;
		}
	}
	return 1 + min;
}

//optimal
void optimal(int n, int k, int* a) {
	cout << "+++ Optimal" << endl;
	cout << "	 Start      : " << n << endl;
	int m = n;
	for (int i = 0; i < k; i++) {
		if (a[i] < 0)
			m -= a[i];
		else
			m += a[i];
	}
	int* count = new int[m - 2];
	int countMoves = 0;
	map<int, int> Back;
	Back[1] = 0;
	for (int i = 1; i <= n * 2; i *= 2) {
		Back[i] = factorsOf2(i);
		//cout << i << "=" << Back[i] << endl;
	}
	for (int i = 2; i < m; i++) {
		countMoves = 0;
		count[i - 2] = B(i, m, Back, a, k, countMoves);
		cout << i << " = " << count[i] << endl;
	}
}


int main() {
	int n, k, *a;
	cout << "Enter the value of n: " << flush;
	cin >> n;
	cout << "Enter the value of k: " << flush;
	cin >> k;
	a = new  int[k];
	for (int i = 0; i < k; i++) {
		cout << "a" << i << " = " << flush;
		cin >> a[i];
	}
	optimal(n, k, a);


	system("pause");
	return 0;
}