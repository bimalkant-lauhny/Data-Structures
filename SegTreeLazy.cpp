//Segment Tree Template

#include <bits/stdc++.h>
using namespace std;

// represents structure of a node of Segment Tree 
struct node
{
	int value; // value stored by a node

	// stores range of input array fo which this node stores sum
	int lRange, rRange;

	// stores leftChild and right child of this node
	int lChild, rChild;

	// stores value needed to update the node
	int updateStatus;

	// assign direct value when node is a leaf
	void assignLeaf(int val)
	{
		value = val;
	}

	// sets indexes of its' children. (-1 if it is a leaf)
	void setChild(int l, int r)
	{
		lChild = l;
		rChild = r;
	}

	// sets range of input array represented by this node
	void setRange(int l, int r)
	{
		lRange = l;
		rRange = r;
	}

	void initUpdate()
	{
		updateStatus = 0;
	}

	// it sets upadte status
	void addUpdate(int update)
	{
		updateStatus += update;
	}

	// update value of the node
	void applyUpdate()
	{
		value += ((rRange - lRange + 1) * updateStatus);
	}

	// merge left and right child nodes to get value for this node
	// given that thi node is not a leaf
	void mergeNodes(node &left, node &right)
	{
		value = left.value + right.value;
	}

	// returns value of this node
	int getVal()
	{
		return value;
	}

};

// Segment Tree class designed for range sum queries with lazy propagation where
// a range is to be updated by INCREMENTING each element of the range by any number 'a'

class SegTree
{

	int *inpArr; // this is input array, it holds values on which segment tree is to be condtructed
	int m; // size of input array
	node *segtree; // array that holds nodes for segment tree
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
		// set range represented by node at 'pos' index of segment tree
		segtree[pos].setRange(low, high); 
		segtree[pos].initUpdate();

		if(low == high) //leaf
		{
			segtree[pos].setChild(-1, -1);
			segtree[pos].assignLeaf(inpArr[low]);
			return;
		}

		int mid = (low + high) / 2;
		
		int left = getLeftChild(pos);
		construct(low, mid, left);
		
		int right = getRightChild(pos);
		construct(mid+1, high, right);

		segtree[pos].setChild(left, right);

		segtree[pos].mergeNodes(segtree[left], segtree[right]);
	}

	// returns sum for query in range [qlow, qhigh]
	node sum(int qlow, int qhigh, int pos)
	{
		if (segtree[pos].updateStatus != 0)
			propagatePendingUpdate(pos);

		if(qlow <= segtree[pos].lRange && qhigh >= segtree[pos].rRange) // total overlap
		{
			return segtree[pos];
		}

		if(qlow > segtree[segtree[pos].lChild].rRange) // no left overlap
			return sum(qlow, qhigh, segtree[pos].rChild);
		if(qhigh < segtree[segtree[pos].rChild].lRange) // no right overlap
			return sum(qlow, qhigh, segtree[pos].lChild);

		// partial overlap
		node leftNode = sum(qlow, qhigh, segtree[pos].lChild);
		node rightNode = sum(qlow, qhigh, segtree[pos].rChild);
	
		node res;
		res.mergeNodes(leftNode, rightNode);
		
		return res;
	}

	// 'pos' is the index of node currently being accessed in segment tree
	// 'i' is the index to be updated in input array
	// 'val' is the new value to be assigned at 'i' index of input array
	void eleUpdate(int pos, int i, int val)
	{
		if(segtree[pos].lRange == segtree[pos].rRange) //leaf
		{
			inpArr[i] = val;
			segtree[pos].assignLeaf(val);
			return;
		}

		// partial overlap
		if(i <= segtree[segtree[pos].lChild].lRange)
			eleUpdate(segtree[pos].lChild, i, val);
		else
			eleUpdate(segtree[pos].rChild, i, val);

		// merges result, takes care even if there is total overlap
		segtree[pos].mergeNodes(segtree[segtree[pos].lChild], segtree[segtree[pos].rChild]);
	}

	// it applies pending update to node at 'pos' and propagates update to pos's children
	void propagatePendingUpdate(int pos)
	{
		segtree[pos].applyUpdate(); // applying pending update to pos

		int propVal = segtree[pos].updateStatus; // value to be propagated
		
		segtree[pos].initUpdate(); // setting update status to 0
		
		// propagating update to left and right child
		int left = segtree[pos].lChild;

		if(left == -1) // leaf, hence no need to propagate
			return;
		
		int right = segtree[pos].rChild;

		segtree[left].addUpdate(propVal);
		segtree[right].addUpdate(propVal);
	}

	void rangeUpdate(int pos, int l, int r, int val)
	{
		// if l and r are the range represented by node at pos, 
		// update assing status and propagate pending update
		if(segtree[pos].lRange == l && segtree[pos].rRange == r)
		{
			segtree[pos].addUpdate(val);
			propagatePendingUpdate(pos);
			return;
		}

		int mid = segtree[segtree[pos].lChild].rRange;

		if(l > mid)
			rangeUpdate(segtree[pos].rChild, l, r, val);
		else if( r < mid+1)
			rangeUpdate(segtree[pos].lChild, l, r, val);
		else
		{
			rangeUpdate(segtree[pos].lChild, l, mid, val);
			rangeUpdate(segtree[pos].rChild, mid+1, r, val);
		}

		segtree[pos].mergeNodes(segtree[segtree[pos].lChild], segtree[segtree[pos].rChild]);
	}

public:

	// constructor, assigns input array size and segment tree size
	// also, it assigns memory for input array and segment tree
	SegTree(int i)
	{
		this->m = i;
		inpArr = new int [m+1];

		this->n = calcSize(i);
		segtree = new node [n];
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
		return sum(qlow, qhigh, 1).getVal();
	}

	//interface for element update
	void update(int index, int val)
	{
		eleUpdate(1, index, val);
	}

	void updateRange(int l, int r, int val)
	{
		rangeUpdate(1, l, r, val);
	}

	// prints segment tree for debugging purposes
	void debug()
	{
		cout << "Segment Tree values:" << endl;
		for(int i=0; i<n; i++)
			cout << i << " : " <<segtree[i].getVal() << " Range : {" << segtree[i].lRange << "," << segtree[i].rRange << "}"<< endl;
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

	cout << "Range Update +3 from 2 to 6" << endl;
	t.updateRange(2, 6, 3);
	t.debug();
	cout << "[1,4] : " << t.query(1, 4) << endl;
	cout << "[3,6] : " << t.query(3, 6) << endl;

	t.debug();

	cout << "Range Update +1 from 3 to 5" << endl;
	t.updateRange(3, 5, 1);
	t.debug();
	cout << "[1,4] : " << t.query(1, 4) << endl;
	cout << "[3,6] : " << t.query(3, 6) << endl;

	t.debug();

	return 0;
}