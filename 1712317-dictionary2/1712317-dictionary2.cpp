// 1712317-dictionary2.cpp : This file contains the 'main' function. Program epNodeecution begins and ends there.
//

#include "pch.h"
#include <iostream> 
#include <queue> 
#include<string>
#include<sstream>
#include<vector>
#include<time.h>
#include<fstream>
using namespace std;

enum COLOR { RED, BLACK };

class Node {
public:
	string val;
	string meaning;
	COLOR color;
	Node *left, *right, *parent;

	Node(string val, string meaning) {
		this->val = val;
		this->meaning = meaning;
		parent = left = right = NULL;
		color = RED;
	}
	Node *uncle() {
		if (parent == NULL or parent->parent == NULL)
			return NULL;

		if (parent->isOnLeft())
			return parent->parent->right;
		else
			return parent->parent->left;
	}
	bool isOnLeft() { return this == parent->left; }
	Node *sibling() {
		if (parent == NULL)
			return NULL;

		if (isOnLeft())
			return parent->right;

		return parent->left;
	}

	void moveDown(Node *nParent) {
		if (parent != NULL) {
			if (isOnLeft()) {
				parent->left = nParent;
			}
			else {
				parent->right = nParent;
			}
		}
		nParent->parent = parent;
		parent = nParent;
	}

	bool hasRedChild() {
		return (left != NULL and left->color == RED) or
			(right != NULL and right->color == RED);
	}
	
};

class RBTree {
	Node *root;

	

	void leftRotate(Node *x) {
		Node *nParent = x->right;

		if (x == root)
			root = nParent;

		x->moveDown(nParent);

		x->right = nParent->left;
		if (nParent->left != NULL)
			nParent->left->parent = x;

		nParent->left = x;
	}

	void rightRotate(Node *x) {
		Node *nParent = x->left;

		if (x == root)
			root = nParent;

		x->moveDown(nParent);

		x->left = nParent->right;
		if (nParent->right != NULL)
			nParent->right->parent = x;

		nParent->right = x;
	}

	void swapColors(Node *x1, Node *x2) {
		COLOR temp;
		temp = x1->color;
		x1->color = x2->color;
		x2->color = temp;
	}

	void swapValues(Node *u, Node *v) {
		string tempval;
		string tempmean;
		tempval = u->val;
		tempmean = u->meaning;
		u->val = v->val;
		u->meaning = v->meaning;
		v->val = tempval;
		v->meaning = tempmean;
	}

	void fixRedRed(Node *x) {
		if (x == root) {
			x->color = BLACK;
			return;
		}

		Node *parent = x->parent, *grandparent = parent->parent,
			*uncle = x->uncle();

		if (parent->color != BLACK) {
			if (uncle != NULL && uncle->color == RED) {
				parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				fixRedRed(grandparent);
			}
			else {
				if (parent->isOnLeft()) {
					if (x->isOnLeft()) {
						swapColors(parent, grandparent);
					}
					else {
						leftRotate(parent);
						swapColors(x, grandparent);
					}
					rightRotate(grandparent);
				}
				else {
					if (x->isOnLeft()) {
						rightRotate(parent);
						swapColors(x, grandparent);
					}
					else {
						swapColors(parent, grandparent);
					}

					leftRotate(grandparent);
				}
			}
		}
	}

	Node *successor(Node *x) {
		Node *temp = x;

		while (temp->left != NULL)
			temp = temp->left;

		return temp;
	}

	Node *BSTreplace(Node *x) {
		if (x->left != NULL and x->right != NULL)
			return successor(x->right);

		if (x->left == NULL and x->right == NULL)
			return NULL;

		if (x->left != NULL)
			return x->left;
		else
			return x->right;
	}

	void deleteNode(Node *v) {
		Node *u = BSTreplace(v);

		bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
		Node *parent = v->parent;

		if (u == NULL) {
			if (v == root) {
				root = NULL;
			}
			else {
				if (uvBlack) {
					fixDoubleBlack(v);
				}
				else {
					if (v->sibling() != NULL)
						v->sibling()->color = RED;
				}

				if (v->isOnLeft()) {
					parent->left = NULL;
				}
				else {
					parent->right = NULL;
				}
			}
			delete v;
			return;
		}

		if (v->left == NULL or v->right == NULL) {
			if (v == root) {
				v->val = u->val;
				v->left = v->right = NULL;
				delete u;
			}
			else {
				if (v->isOnLeft()) {
					parent->left = u;
				}
				else {
					parent->right = u;
				}
				delete v;
				u->parent = parent;
				if (uvBlack) {
					fixDoubleBlack(u);
				}
				else {
					u->color = BLACK;
				}
			}
			return;
		}

		swapValues(u, v);
		deleteNode(u);
	}

	void fixDoubleBlack(Node *x) {
		if (x == root)
			return;

		Node *sibling = x->sibling(), *parent = x->parent;
		if (sibling == NULL) {
			fixDoubleBlack(parent);
		}
		else {
			if (sibling->color == RED) {
				parent->color = RED;
				sibling->color = BLACK;
				if (sibling->isOnLeft()) {
					rightRotate(parent);
				}
				else {
					leftRotate(parent);
				}
				fixDoubleBlack(x);
			}
			else {
				if (sibling->hasRedChild()) {
					if (sibling->left != NULL and sibling->left->color == RED) {
						if (sibling->isOnLeft()) {
							sibling->left->color = sibling->color;
							sibling->color = parent->color;
							rightRotate(parent);
						}
						else {
							sibling->left->color = parent->color;
							rightRotate(sibling);
							leftRotate(parent);
						}
					}
					else {
						if (sibling->isOnLeft()) {
							sibling->right->color = parent->color;
							leftRotate(sibling);
							rightRotate(parent);
						}
						else {
							sibling->right->color = sibling->color;
							sibling->color = parent->color;
							leftRotate(parent);
						}
					}
					parent->color = BLACK;
				}
				else {
					sibling->color = RED;
					if (parent->color == BLACK)
						fixDoubleBlack(parent);
					else
						parent->color = BLACK;
				}
			}
		}
	}

	void inorder(Node *x) {
		if (x == NULL)
			return;
		inorder(x->left);
		cout << x->val << ": " << x->meaning << endl;
		inorder(x->right);
	}

public:
	RBTree() { root = NULL; }

	Node *getRoot() { return root; }

	Node *search(string n) {
		Node *temp = root;
		while (temp != NULL) {
			if (n < temp->val) {
				if (temp->left == NULL)
					break;
				else
					temp = temp->left;
			}
			else if (n == temp->val) {
				break;
			}
			else {
				if (temp->right == NULL)
					break;
				else
					temp = temp->right;
			}
		}

		return temp;
	}

	void insert(string n, string meaning) {
		Node *newNode = new Node(n, meaning);
		if (root == NULL) {

			newNode->color = BLACK;
			root = newNode;
		}
		else {
			Node *temp = search(n);

			if (temp->val == n) {
				return;
			}

			newNode->parent = temp;

			if (n < temp->val)
				temp->left = newNode;
			else
				temp->right = newNode;

			fixRedRed(newNode);
		}
	}

	void deleteByVal(string n) {
		if (root == NULL)
			return;

		Node *v = search(n), *u;

		if (v->val != n) {
			cout << "No node found to delete with value:" << n << endl;
			return;
		}

		deleteNode(v);
	}

	void changeMeaning(string key, string newMean) {
		Node* result = search(key);
		result->val = newMean;
	}
	void printInOrder() {
		cout << "Inorder: " << endl;
		if (root == NULL)
			cout << "Tree is empty" << endl;
		else
			inorder(root);
		cout << endl;
	}
	vector<string> parse(string data) {
		vector<string> Tokens;
		int seperatorLen = 0;
		int startPos = 0;
		int foundPos;
		if (data.find("  ") != -1) {
			foundPos = data.find("  ");
			seperatorLen = 2;
			string key = data.substr(startPos, foundPos);
			startPos = foundPos + seperatorLen;
			string meaning = data.substr(startPos);
			Tokens.push_back(key);
			Tokens.push_back(meaning);
		}
		return Tokens;
	}	
	void getData() {
		string data;
		RBTree tree;
		ifstream infile;
		infile.open("dictionary.txt");
		while (getline(infile, data)) {
			vector<string> Tokens = parse(data);
			if (!Tokens.empty()) {
				insert(Tokens[0], Tokens[1]);
			}
		}
	}
	
};
void saveInfo(Node* tree, ofstream &outfile) {

	if (tree != NULL) {
		saveInfo(tree->left, outfile);
		outfile << tree->val << "  ";
		outfile << tree->meaning << endl;
		saveInfo(tree->right, outfile);

	}

}
int findMax(int a, int b) {
	return a > b ? a : b;
}
int height(Node* root) {
	if (root == NULL) return 0;
	else {
		return findMax(height(root->left), height(root->right)) + 1;
	}
}

int main()
{
	string data;
	string meaning;
	RBTree tree;
	/*do {
		cout << "nhap tu khoa, . de ket thuc: ";
		getline(cin, data);
		if (data == ".") break;
		cout << "Nhap nghia cua tu: ";
		getline(cin, meaning);
		tree.insert(data, meaning);
	} while (data != ".");
	string key = "dan";
	tree.printInOrder();
	Node* result = tree.search(key);
	tree.deleteByVal(result->val);
	tree.printInOrder();*/
	clock_t start = clock();
	tree.getData();
	cout << "thoi gian doc file: " << (clock() - start) << "ms" << endl;
	ofstream outfile;
	outfile.open("save.txt");
	start = clock();
	saveInfo(tree.getRoot(),outfile);
	cout << "thoi gian luu file: " << (clock() - start) << "ms" << endl;
	cout << "save complete" << endl;
	string key = "dan";
	string mean = "a very handsome man :v";
	start = clock();
	tree.insert(key, mean);
	cout << "thoi gian insert: " << (clock() - start) << "ms" << endl;
	start = clock();
	Node* searchRessult = tree.search(key);
	cout << "thoi gian search: " << (clock() - start) << "ms" << endl;
	if (searchRessult == NULL) cout << "khong tim thay" << endl;
	else cout << "search result: "<<key << ": " << searchRessult->meaning << endl;
	start = clock();
	tree.deleteByVal(key);
	cout << "thoi gian xoa: " << (clock() - start) << "ms" << endl;
	tree.changeMeaning(key, "that ra thi khong nhung dep giai ma con khoai to nua :V");
	searchRessult = tree.search(key);
	if (searchRessult == NULL) cout << "khong tim thay" << endl;
	else cout << key << ": " << searchRessult->meaning << endl;
	cout << "height: " << height(tree.getRoot()) << endl;
	delete searchRessult;
	return 0;

}

