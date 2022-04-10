#include <bits/stdc++.h>
using namespace std;
int t, d;

class Node {
	bool IS_LEAF;
	int *key, size;
	Node** ptr;
	friend class BPTree;

public:
	Node(int);
};

class BPTree {
	Node* root;
	void insertInternal(int, Node*, Node*);
	Node* findParent(Node*, Node*);
    int indexcount;
    int datacount;

public:
	BPTree();
	//void search(int);
	void insert(int);
	void display(void);
};

Node::Node(int MAX)
{
    key = new int[MAX];
	ptr = new Node*[MAX + 1];
}

BPTree::BPTree()
{
	root = NULL;
    indexcount = 0;
    datacount = 1;
}

// void BPTree::search(int x)
// {
// 	if (root == NULL) {
// 		cout << "Tree is empty\n";
// 	}
// 	else {
// 		Node* cursor = root;
// 		while (cursor->IS_LEAF == false) {
// 			for (int i = 0; i < cursor->size; i++) {
// 				if (x < cursor->key[i]) {
// 					cursor = cursor->ptr[i];
// 					break;
// 				}
// 				if (i == cursor->size - 1) {
// 					cursor = cursor->ptr[i + 1];
// 					break;
// 				}
// 			}
// 		}
// 		for (int i = 0;
// 			i < cursor->size; i++) {
// 			if (cursor->key[i] == x) {
// 				cout << "Found\n";
// 				return;
// 			}
// 		}
// 		cout << "Not found\n";
// 	}
// }

void BPTree::insert(int x)
{
	if (root == NULL) {
		root = new Node(2*d);
		root->key[0] = x;
		root->IS_LEAF = true;
		root->size = 1;
	}
	else {
		Node* cursor = root;
		Node* parent;
		while (cursor->IS_LEAF == false) {
			parent = cursor;
			for (int i = 0; i < cursor->size; i++) {
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					break;
				}
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		if (cursor->size < 2*d) {
			int i = 0;
			while (x > cursor->key[i]
				&& i < cursor->size) {
				i++;
			}
			for (int j = cursor->size; j > i; j--) {
                cursor->key[j] = cursor->key[j - 1];
			}
			cursor->key[i] = x;
			cursor->size++;
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
			cursor->ptr[cursor->size - 1] = NULL;
		}
		else {
			Node* newLeaf = new Node(2*d);
            datacount++;
			int virtualNode[2*d + 1];

			for (int i = 0; i < 2*d; i++) {
				virtualNode[i] = cursor->key[i];
			}
			int i = 0, j;

			while (x > virtualNode[i] && i < 2*d) {
				i++;
			}

			for (int j = 2*d + 1; j > i; j--) {
				virtualNode[j] = virtualNode[j - 1];
			}

			virtualNode[i] = x;
			newLeaf->IS_LEAF = true;

			cursor->size = d;
			newLeaf->size = d + 1;

			for (i = 0; i < cursor->size; i++) {
				cursor->key[i] = virtualNode[i];
			}

			for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
				newLeaf->key[i] = virtualNode[j];
			}

			if (cursor == root) {
				Node* newRoot = new Node(2*t+1);
                indexcount++;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot;
			}
			else {
				insertInternal(newLeaf->key[0], parent, newLeaf);
			}
		}
	}
}

void BPTree::insertInternal(int x, Node* cursor, Node* child)
{
	if (cursor->size < 2*t+1) {
		int i = 0;
		while (x > cursor->key[i] && i < cursor->size) {
			i++;
		}

		for (int j = cursor->size;  j > i; j--) {
			cursor->key[j]= cursor->key[j - 1];
		}

		for (int j = cursor->size + 1; j > i + 1; j--) {
			cursor->ptr[j] = cursor->ptr[j - 1];
		}
		cursor->key[i] = x;
		cursor->size++;
		cursor->ptr[i + 1] = child;
	}

	else {
		Node* newInternal = new Node(2*t+1);
        indexcount++;
		int virtualKey[2*t+1 + 1];
		Node* virtualPtr[2*t+1 + 2];

		for (int i = 0; i < 2*t+1; i++) {
			virtualKey[i] = cursor->key[i];
		}

		for (int i = 0; i < 2*t+1 + 1; i++) {
			virtualPtr[i] = cursor->ptr[i];
		}

		int i = 0, j;

		while (x > virtualKey[i] && i < 2*t+1) {
			i++;
		}

		for (int j = 2*t+1 + 1; j > i; j--) {
			virtualKey[j] = virtualKey[j - 1];
		}
		virtualKey[i] = x;

		for (int j = 2*t+1 + 2; j > i + 1; j--) {
			virtualPtr[j] = virtualPtr[j - 1];
		}

		virtualPtr[i + 1] = child;
		newInternal->IS_LEAF = false;

		cursor->size = t;
		newInternal->size = t + 1;

		for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
			newInternal->key[i] = virtualKey[j];
		}

		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {

			newInternal->ptr[i] = virtualPtr[j+1];
		}

		if (cursor == root) {

			Node* newRoot = new Node(2*t+1);
            indexcount++;
			newRoot->key[0] = virtualKey[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->IS_LEAF = false;
			newRoot->size = 1;
			root = newRoot;
        }
		else {
			insertInternal(virtualKey[cursor->size],findParent(root, cursor),newInternal);
		}
	}
}

Node* BPTree::findParent(Node* cursor, Node* child)
{
	Node* parent;
	if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
		return NULL;
	}
	for (int i = 0; i < cursor->size + 1; i++) {
		if (cursor->ptr[i] == child) {
			parent = cursor;
			return parent;
		}
		else {
			parent = findParent(cursor->ptr[i], child);
			if (parent != NULL)
				return parent;
		}
	}
	return parent;
}

void BPTree::display(void){
    cout << indexcount << " " << datacount << " ";
    for(int i = 0; i < root->size; i++){
        cout << root->key[i] << " ";
    }
    cout << endl;
}

// Driver Code
int main()
{
	BPTree node;

	// Create B+ Tree
	cin>>d>>t;
    int choice;
	int n;
	while(1)
	{
		cin>>choice;
		switch(choice)
		{
			case 1:
				cin>>n;
				node.insert(n);
				break;
			case 2:
				node.display();
				break;
			case 3:
				exit(0);
			default:
				cout<<"Invalid Input"<<endl;
		}
	}

	return 0;
}
