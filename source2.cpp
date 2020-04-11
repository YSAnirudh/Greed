#include <iostream>
#include <algorithm>
#include <map>
#include <unordered_set>
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

//greedy1
void greedy1(int n) {
	int count = 0;
	cout << "+++ Greedy1" << endl;
	cout << "	 Start      : " << n << endl;
	while (n != 1) {
		if (n % 2 == 0) {
			n /= 2; // n = n / 2 if it is even
			cout << "	 Divide     : " << n << endl;
			count++;
		}
		else {
			n -= 1; // n = n - 1 if it is odd
			cout << "	 Decrement  : " << n << endl;
			count++;
		}
	}
	cout << "--- Number of steps = " << count << endl;
	cout << endl;
}

//greedy2
void greedy2(int n) {
	int count = 0;
	cout << "+++ Greedy2" << endl;
	cout << "	 Start      : " << n << endl;
	while (n != 1) {
		if (n % 2 == 0) {
			n /= 2;
			cout << "	 Divide     : " << n << endl;
			count++;
		}
		else {
			if (n == 3) {
				n -= 1; //if n = 3 there is no need to go to 4 and come back to 2
				cout << "	 Decrement  : " << n << endl;
				count++;
			}
			//if no of factors of 2 > when 1 is added to n
			//than when 1 is subtracted from n, n = n + 1
			//else n = n - 1
			else if (factorsOf2(n + 1) > factorsOf2(n - 1)) {
				n += 1;
				cout << "	 Increment  : " << n << endl;
				count++;
			}
			else {
				n -= 1;
				cout << "	 Decrement  : " << n << endl;
				count++;
			}
		}
	}
	cout << "--- Number of steps = " << count << endl;
	cout << endl;
}

//greedy3
void greedy3(int n, int k, int* a) {
	int count = 0;
	int indexKMax, fac1;
	cout << "+++ Greedy3" << endl;
	cout << "	 Start      : " << n << endl;
	while (n != 1) {
		//when n is even n is divided by two as usual
		if (n % 2 == 0) {
			if (n % 2 == 0) {
				n /= 2;
				cout << "	 Divide     : " << n << endl;
				count++;
			}
		}
		//when n is odd
		else {
			if (n == 3) {
				if (a[0] == 1 || a[0] == -1) {
					n += a[0];
					cout << "	 Add " << a[0] << "     : " << n << endl;
					count++;
				}
			}
			else {
				int maxFactors = 0; //initialisation of 
				indexKMax = 0; //value of index i in a[i] for which n+a[i] have maximum no of factors of 2
				for (int i = 0; i < k; i++) {
					fac1 = factorsOf2(n + a[i]);
					//if factors of 2 of i th index if greater than that of maxFactors, then maxFactors 
					// becomes the maximum and the current i becomes indexKMax
					if (fac1 > maxFactors) {
						maxFactors = fac1;
						indexKMax = i;
					}
					else if (fac1 == maxFactors) {
						// if a[i] is greater than the n with a[indexKMax] then indexKMax = i
						//as now, a[i] leads to smaller value of n with highest factors of 2
						if (a[indexKMax] > a[i]) {
							indexKMax = i;
						}
						// else
						// if a[i] is greater than the n with a[indexKMax] then no change takes place
						// as a[indexKMax] leads to smaller n value with highest factors of 2
					}
				}
				//as we have found out the index of a[i] that leads to max no of factors we 
				// simply add that value to n
				n += a[indexKMax];
				if (a[indexKMax] < 0) {
					cout << "	 Add " << a[indexKMax] << "     : " << n << endl;
				}
				else {
					cout << "	 Add  " << a[indexKMax] << "     : " << n << endl;
				}
				count++;
			}
		}
	}
	cout << "--- Number of steps = " << count << endl;
	cout << endl;
}

//checks if the number is a power of 2 O(1) algorithm
bool isPowOf2(int n) {
	if (n == 0) {
		return false;
	}
	else {
		return (ceil(log2(n)) == floor(log2(n)));
	}
	return false;
}

//runs at max O(log(n)*k) and length of the path is surely less than O(log(n))
// with log(k)*log(n) calls for finding all B[i] from 2->m and each call takes O(k)
// as we have put a constraint on k, it works for any n and k for {|a[i]| -> (1,9)}
int B(int n, int m, int* &Back, int* a, int k, int* temps, int ex) {
	if (n <= 0 || n > m) {
		return INT_MAX; //returning a very large number if n <= 0 or n >=m as those values cannot 
						//contribute to the minimum value of 1 + min(...)
	}
	if (n == 1) {
		return 0; //no of steps to reach 1 is 0
	}

	//assigning and returning the no of steps required if n is a pow of 2
	//these values act as chain termination agents of the recursion
	if (isPowOf2(n)) {
		Back[n] = (int)log2(n);
		return Back[n];
	}
	int min = INT_MAX, temp;

	//if the program runs more than log(k)*log(n) times 
	// log(k)*log(n) to handle the function going into infinite loop
	// due to not being able to compute all the required values for n to become 0 for a large k
	// nevertheless, as k <= 5 for this algorithm, it is a constant time with max going till 2 * log(n)
	//we return a large number
	//this is not a constraint on no of steps
	if (ex > (int)(log2(n)*(int)log2(k))) {
		return INT_MAX;
	}

	// if n is even then include B(n/2) in the equation B(n) = 1 + min(...)
	// set that as the min until B(n + a[i]) becomes less than min
	if (n % 2 == 0) {
		min = B(n / 2, m, Back, a, k, temps, ex + 1);
	}

	//iterative call for all the a[k] to compute min(B(..),...)
	// contributes to O(k) complexity
	for (int i = 0; i < k; i++) {
		// checking if the value of B(n + a[i]) is already present in Back
		// Back keeps on storing these values on every recursive call
		if (Back[n + a[i]] > 0) { //if Back[n+a[i]] > 0 then that value is already present
			temp = Back[n + a[i]];
		}
		//if it is not there in Back
		else {
			//ensuring it isnt out of bounds
			if (n + a[i] < m && n + a[i] > 0) {
				// we check if it is there in an array which stores the
				// 1 in i th place which already have been called once in a particular path
				// if we encounter that value ,it leads to an infinite loop,
				//therefore we assign a large no to temp to eliminate that path where it occurs again
				if (temps[n + a[i]] > 0) { // O(1) complexity
					temp = INT_MAX;
				}

				// if it doesn't occur, we normally call the recursive function 
				// and keep storing the no of steps of n+a[i] in Back as usual
				// we also update temps so that we can ensure that B(that no.) doesn't repeat
				else {
					temps[n + a[i]] = 1; // 1 indicates it has appeared 1 time in loop
					Back[n + a[i]] = B(n + a[i], m, Back, a, k, temps, ex + 1);
					temp = INT_MAX;
				}
			}
			else temp = INT_MAX;
		}
		if (temp < min) {
			min = temp;
		}
	}
	if (min == INT_MAX) min -= 1; // when we don't find any way to reach solution

	// as back keeps track of no of steps, we assign it to 1 + min(.....) = B(n) and return it
	Back[n] = 1 + min;
	return Back[n];
}

//optimal
//O(k*n*log(n)) => complexity of this algorithm
void optimal(int n, int k, int* a) {
	//O(k) + O(n) + O(k*log(n)) + O(k*n*log(n)) = O(k*n*log(n))
	cout << "+++ Optimal" << endl;
	cout << "	 Start      : " << n << endl;
	int max = 0; //starting from 0 and negative value can make m go below n => only considering positive values of a[i]
	//Assignment of m
	//O(k) to find max
	//we choose this value of m because n can go until a maximum of log(n) steps of max(a[i])
	for (int i = 0; i < k; i++) {
		if (a[i] > max) max = a[i];
	}
	int m = n + (int)(max * log2(n));

	//array to store the best move count for each i from 2 -> m
	int* count = new int[m - 2];

	// an array storing the best move count for each i in 2-> m
	// taken by every i in 1-> m
	int* Back = new int[m];

	//the value of n that could recur may vary from 1->m
	//used to store values that might recur causing infinite loop
	int* temps = new int[m];

	// O(m) ~ O(n) = assigning base conditions to Back and temps
	Back[0] = INT_MAX; // a large no of it doesnt affect the minimum moves when Back [0] is called
	temps[0] = INT_MAX; //similar to back
	for (int i = 1; i < m; i++) {
		Back[i] = INT_MIN;
		temps[i] = 0;
	}

	// O(n*k*log(n)) as B(..) -> O(k*log(n)) and it runs O(n) times
	for (int i = 2; i < m; i++) {
		count[i - 2] = B(i, m, Back, a, k, temps, 0);
	}

	int stepCountOfn = 0;
	// O(k * log(n)) accounting for the for loop running k times
	while (n != 1) {
		//if n is even check for n/2 and also n+a[i]
		if (n % 2 == 0) {
			// as Back has now stored all the best move counts for each i from 2->m
			// therefore if Back[n/2] has atleast 1 step less than Back[n] to reach 1
			// that is the best move to make => n = n / 2
			if (Back[n / 2] < Back[n]) {
				n /= 2;
				cout << "	 Divide     : " << n << endl;
				stepCountOfn++;
			}
			// if Back[n + a[i]] has atleast 1 step less than Back[n] to reach 1
			// that is the best move to make => n = n / 2
			else {
				//checking for every i = k
				for (int i = 0; i < k; i++) {
					if (Back[n + a[i]] < Back[n]) {
						n += a[i];
						stepCountOfn++;
						//if else only for adjusting spaces to make it look neat
						if (a[i] < 0) {
							cout << "	 Add    " << a[i] << "  : " << n << endl;
							break;
						}
						else {
							cout << "	 Add     " << a[i] << "  : " << n << endl;
							break;
						}
					}
				}
			}
		}
		//if n is odd check for n + a[i]
		else {
			// if Back[n + a[i]] has atleast 1 step less than Back[n] to reach 1
			// that is the best move to make => n = n / 2
			//checking for every i = k
			for (int i = 0; i < k; i++) {
				if (Back[n + a[i]] < Back[n]) {
					n += a[i];
					cout << "	 Add    " << a[i] << "  : " << n << endl;
					stepCountOfn++;
					break;
				}
			}
		}
	}
	cout << "--- Number of steps = " << stepCountOfn << endl;
	cout << endl;

	//deallocating the allocated memory
	delete[] Back;
	delete[] count;
	delete[] temps;
}


int main() {
	int n, k, *a;
	cout << "Enter the value of n: " << flush;
	cin >> n;

	greedy1(n);
	greedy2(n);

	cout << "Enter the value of k: " << flush;
	cin >> k;
	a = new  int[k];
	for (int i = 0; i < k; i++) {
		cout << "a" << i << " = " << flush;
		cin >> a[i];
	}
	greedy3(n, k, a);
	optimal(n, k, a);

	system("pause");
	return 0;
}
