//Segment Tree Template

#include <bits/stdc++.h>
using namespace std;

// Segment Tree class designed for range sum queries
class SegTree
{

	int *inpArr; // this is input array, it holds values on which segment tree is to be condtructed
	int m; // size of input array
	int *segtree; // array that holds nodes for segment tree
	int n; // size of tree array

	// returns size of array for segment tree
	int calcSize(int i)
	{
		int size = 1;
		while(size < i)
			size <<= 1;
		return (size << 1);
	}

	// returns parent for ith node
	int getParent(int i)
	{
		return (i)/2;
	}

	// returns left child for ith node
	int getLeftChild(int i)
	{
		return (2*i);
	}

	// returns right child for ith node
	int getRightChild(int i)
	{
		return (2*i + 1);
	}

	//builds segment tree using bottom up approach
	void construct(int low, int high, int pos)
	{
		if(low == high)
		{
			segtree[pos] = inpArr[low];
			return;
		}

		int mid = (low + high) / 2;
		
		int left = getLeftChild(pos);
		construct(low, mid, left);
		
		int right = getRightChild(pos);
		construct(mid+1, high, right);

		segtree[pos] =  segtree[left] + segtree[right];
	}

	// returns sum for query in range [qlow, qhigh]
	int sum(int qlow, int qhigh, int low, int high, int pos)
	{
		if(qlow <= low && qhigh >= high) // total overlap
			return segtree[pos];

		if(qlow > high || qhigh < low) // no overlap
			return 0;

		// partial overlap
		int mid = (low + high) / 2;

		int left = getLeftChild(pos);
		int right = getRightChild(pos);

		return sum(qlow, qhigh, low, mid, left) + sum(qlow, qhigh, mid+1, high, right);

	}

public:

	// constructor, assigns input array size and segment tree size
	// also, it assigns memory for input array and segment tree
	SegTree(int i)
	{
		this->m = i;
		inpArr = new int [m+1];

		this->n = calcSize(i);
		segtree = new int [n];
	}

	// returns size for segment tree

	// takes values for inpArr[] from user
	void input()
	{
		for(int i=1; i<=m; i++)
			cin >> inpArr[i];
	}

	// interface for building segment tree
	void buildTree()
	{
		construct(1, m, 1);
	}

	// interface for range sum query
	int query(int qlow, int qhigh)
	{
		return sum(qlow, qhigh, 1, m, 1);
	}

	// prints segment tree for debugging purposes
	void debug()
	{
		cout << "Segment Tree values:" << endl;
		for(int i=0; i<n; i++)
			cout << i << " : " <<segtree[i] << endl;
		cout << endl;
	}

	// destructor, frees memory assigned to input array and segment tree
	~SegTree()
	{
		delete[] inpArr, segtree;
	}
};

int main()
{
	cout << "Enter size for input array: " << endl;
	int size;
	cin >> size;
	
	SegTree t(size);
	
	cout << "Enter the values for input array: " << endl;
	t.input();

	cout << "Building Segment Tree for range sum...." << endl;	
	t.buildTree();
	cout << "Build Complete." << endl;

	t.debug();

	cout << "Range Sums:" << endl;
	cout << "[1,4] : " << t.query(1, 4) << endl;
	cout << "[3,6] : " << t.query(3, 6) << endl;

	return 0;
}