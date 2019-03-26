#include<iostream>

using namespace std;

int main(){
	int a[] = {5, 10, 15, 18, 4, 3, 0, 9, 2, 11};
	int b[] = {4, 18, 7, 3, 0, 11, 58, 123};
	int n1 = 10, n2 = 8, i, max=a[0];	
	
	for(i=1; i<n1; i++)
		if(a[i]>max)
			max = a[i];
	
	int *hash = new int[max+1];

	for(i=0; i<n1; i++)
		hash[a[i]] = 1;
	
	for(i=0; i<n2; i++)
		if(b[i]<max+1)
			if(hash[b[i]]==1)	
				cout << b[i] << " ";
	cout << endl;
	
	return 0;
}
