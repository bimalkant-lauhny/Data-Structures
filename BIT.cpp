#include <bits/stdc++.h>
using namespace std;

// class for Fenwick Tree/ Binary Indexed Tree
class BIT
{
	int n; // number of elements in input array
	int *input_arr; // array for storing input values 
	int *bit; // array for storing bit(Binary indexed Tree)

	//returns parent of node 'i'
	int getParent(int i)
	{
		// steps :
		// 1. Get 2's complement of 'i'
		// 2. Bitwise And it with 'i'
		// 3. Subtract result from 'i'
		return (i - (i & (-i)));
	}

	//returns element next to node 'i' which needs to be updated
	int getNext(int i)
	{
		// steps :
		// 1. Get 2's complement of 'i'
		// 2. Bitwise And it with 'i'
		// 3. Add result to 'i'
		return (i + (i & (-i)));
	}

public:

	// constructor for BIT class
	BIT(int n)
	{
		this->n = n;
		input_arr = new int [n];
		//fill(input_arr, input_arr+n, 0); // initialising input_arr with 0
		bit = new int [n+1]; // one extra node as [0] is dummy/root node that stores 0 value
		fill(bit, bit+n+1, 0); // initialising bit with 0
	}

	void fillInputArr()
	{
		for(int i=0; i<n; i++)
			cin >> input_arr[i];
	}

	// this function builds BIT in bit array for finding prefix sum
	void buildTree()
	{

		//cout << "Inside Build" << endl;
		int index; // this variable stores current index of bit being updated

		for(int i=1; i<=n; ++i) // we update for n elements in bit, time complexity=O(n)
		{
			index = i;
			while(index <= n) //if index goes out of range, we break out of this loop, time complexity=O(log n)
			{
				bit[index] += input_arr[i-1];
				index = getNext(index); // getting next index to be updated
			}

			//cout << "at " << i << endl;
			//debug();
		}

		//total time complexity for this function = O(n*(log n))
	}

	// this function accepts as argument, index 'i' at which update should be done in input array with value 'val' 
	void update(int i, int val)
	{
		int diff = val - input_arr[i]; // diff stores difference between the previous and updated value
		input_arr[i] = val; // updating input_arr at index 'i' with value 'val'

		int index = i + 1; // this variable stores current index of bit being updated

		while(index <= n) //if index goes out of range, we break out of this loop, time complexity=O(log n)
		{
			bit[index] += diff;
			index = getNext(index); // getting next index to be updated
		}
	}

	// this function returns prefix sum upto index 'i' of input_arr 
	int query(int i)
	{
		int prefixsum = 0; // // this variable stores prefix sum from index 0 upto index 'i' of input array
		int index = i + 1; // this variable stores current index of bit being added to the prefixsum

		while(index > 0)
		{
			prefixsum += bit[index];
			index = getParent(index);
			//cout <<"inside query " << index <<endl;
			//cout <<"prefixsum " << prefixsum <<endl;
		}

		return prefixsum;
	}

	// this array print current values of input_arr and bit for debugging purposes
	void debug()
	{
		cout << "Input Array format(index : value)" << endl;

		for(int i=0; i<n; i++)
			cout << i << " : " << input_arr[i] << ", ";

		cout << endl;

		cout << "BIT array format(index : value)" << endl;

		for(int i=0; i<=n; i++)
			cout << i << " : " << bit[i] << ", ";

		cout << endl;
	}

	// destructor, releases memory used
	~BIT()
	{
		delete[] input_arr, bit;
	}
};

int main()
{

	cout << "Enter number of element in your array: " << endl;
	int n;
	cin >> n;

	BIT a(n);

	cout << "Enter the elements:" << endl;
	a.fillInputArr();
	a.buildTree();
	a.debug();

	cout << "Prefix sums:" << endl; 
	cout << "Range (0, 3): " << a.query(3) << endl;
	cout << "Range (0, 7): " << a.query(7) << endl;
	cout << "Range (3, 9): " << a.query(9) - a.query(2) << endl;

	a.update(3, 9);
	cout << "After updating input_arr[3] = 9." << endl;
	a.debug();
	cout << "Prefix sums:" << endl; 
	cout << "Range (0, 3): " << a.query(3) << endl;
	cout << "Range (0, 7): " << a.query(7) << endl;
	cout << "Range (3, 9): " << a.query(9) - a.query(2) << endl;


	a.update(3, 6);
	cout << "After updating input_arr[3] = 6." << endl;
	a.debug();
	cout << "Prefix sums:" << endl; 
	cout << "Range (0, 3): " << a.query(3) << endl;
	cout << "Range (0, 7): " << a.query(7) << endl;
	cout << "Range (3, 9): " << a.query(9) - a.query(2) << endl;
	return 0;
}

