// 1712317-Dictionary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<time.h>
#include<vector>
using namespace std;

struct AVL {
	string key;
	string meaning;
	int bal;
	AVL* pLeft;
	AVL* pRight;
};
AVL* CreateNode(string data,string meaning) {
	AVL* pNode;
	pNode = new AVL;
	if (pNode == NULL) return NULL;
	pNode->key = data;
	pNode->meaning = meaning;
	pNode->pLeft = pNode->pRight = NULL;
	pNode->bal = 0;
	return pNode;
}

void LeftRotate(AVL* &P) {
	AVL* Q;
	Q = P->pRight;
	P->pRight = Q->pLeft;
	Q->pLeft = P;
	P = Q;
}

void RightRotate(AVL* &P) {
	AVL* Q;
	Q = P->pLeft;
	P->pLeft = Q->pRight;
	Q->pRight = P;
	P = Q;
}

void LeftBalance(AVL* &P) {
	switch (P->pLeft->bal) {
	case 1:
		RightRotate(P);
		P->bal = 0;
		P->pRight->bal = 0;
		break;
	case 2:
		LeftRotate(P->pLeft);
		RightRotate(P);
		switch (P->bal) {
		case 0:
			P->pLeft->bal = 0;
			P->pRight->bal = 0;
			break;
		case 1:
			P->pLeft->bal = 0;
			P->pRight->bal = 2;
			break;
		case 2:
			P->pLeft->bal = 1;
			P->pRight->bal = 0;
			break;
		}
		P->bal = 0;
		break;
	}
}

void RightBalance(AVL* &P) {
	switch (P->pRight->bal) {
	case 1:
		RightRotate(P->pRight);
		LeftRotate(P);
		switch (P->bal) {
		case 0:
			P->pLeft->bal = 0;
			P->pRight->bal = 0;
			break;
		case 1:
			P->pLeft->bal = 1;
			P->pRight->bal = 0;
			break;
		case 2:
			P->pLeft->bal = 2;
			P->pRight->bal = 0;
			break;
		}
		P->bal = 0;
		break;
	case 2:
		LeftRotate(P);
		P->bal = 0;
		P->pLeft->bal = 0;
		break;
	}
}

int InsertNode(AVL* &P, string x,string meaning) {
	int res;
	if (P == NULL) {
		P = CreateNode(x,meaning);
		if (P == NULL) return -1;
		return 2;
	}
	else {
		if (P->key == x) {
			return 0;
		}
		else if (P->key > x) {
			res = InsertNode(P->pLeft, x,meaning);
			if (res < 2) return res;
			switch (P->bal) {
			case 0:
				P->bal = 1;
				return 2;
			case 1:
				LeftBalance(P);
				return 1;
			case 2:
				P->bal = 0;
				return 1;
			}
		}
		else {
			res = InsertNode(P->pRight, x,meaning);
			if (res < 2) return res;
			switch (P->bal) {
			case 0:
				P->bal = 2;
				return 2;
			case 1:
				P->bal = 0;
				return 1;
			case 2:
				RightBalance(P);
				return 1;
			}
		}
	}
}
int searchStandFor(AVL* &P, AVL* &Q) {
	int res;
	if (Q->pLeft != NULL) {
		res = searchStandFor(P, Q->pLeft);
		if (res < 2) return res;
		switch (Q->bal) {
		case 0:
			Q->bal = 2;
			return 1;
		case 1:
			Q->bal = 0;
			return 2;
		case 2:
			RightBalance(P);
			return 1;
		}
	} 
	else {
		P->key = Q->key;
		P = Q;
		Q = Q->pRight;
		return 2;
	}
}

int DeleteNode(AVL* &P,string key) {
	int res;
	if (P == NULL) return 0;
	if (P->key > key) {
		res = DeleteNode(P->pLeft, key);
		if (res < 2) return res;
		switch (P->bal) {
		case 1:
			P->bal = 0;
			return 2;
		case 0:
			P->bal = 2;
			return 1;
		case 2:
			RightBalance(P);
			return 1;
		}
	}
	else if (P->key < key) {
		res = DeleteNode(P->pRight,key);
		if (res < 2) return res;
		switch (P->bal) {
		case 2:
			P->bal = 0;
			return 2;
		case 0:
			P->bal = 1;
			return 1;
		case 1:
			LeftBalance(P);
			return 1;
		}
	}
	else {
		AVL* temp = P;
		if (P->pLeft == NULL) {
			P = P->pRight;
			res = 2;
		}
		else if (P->pRight == NULL) {
			P = P->pLeft;
			res = 2;
		}
		else {
			res = searchStandFor(temp, P->pRight);
			if (res < 2) return res;
			switch (P->bal) {
			case 2:
				P->bal = 0;
				return 2;
			case 0:
				P->bal = 1;
				return 1;
			case 1:
				LeftBalance(P);
				return 1;
			}
		}
		delete temp;
		return res;
	}
}

AVL* search(AVL* P,string key) {
	if (P == NULL) return NULL;
	else if (key < P->key) {
		return search(P->pLeft, key);
	}
	else if (P->key < key) {
		return search(P->pRight, key);
	}
	else {
		return P;
	}
}

void Traverse(AVL* P) {
	if (P != NULL) {
		Traverse(P->pLeft);
		cout << P->key <<" : " << P->meaning << endl;
		cout << endl;
		Traverse(P->pRight);
	}
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

AVL* getData() {
	string data;
	AVL* tree = NULL;
	ifstream infile;
	infile.open("dictionary.txt");
	while (getline(infile, data)) {
		vector<string> Tokens = parse(data);
		if (!Tokens.empty()) {
			InsertNode(tree, Tokens[0], Tokens[1]);
		}
	}
	return tree;
}
void saveInfo(AVL* tree, ofstream &outfile) {

	if (tree != NULL) {
		saveInfo(tree->pLeft, outfile);
		outfile << tree->key << "  ";
		outfile << tree->meaning << endl;
		saveInfo(tree->pRight, outfile);

	}

}
void changemean(AVL* tree,string key, string newMean) {
	AVL* word = search(tree, key);
	if (word != NULL) {
		word->meaning = newMean;
	}
	else cout << "khong tim thay" << endl;
}
int findMax(int a, int b) {
	return a > b ? a : b;
}
int height(AVL*tree) {
	if (tree == NULL) return 0;
	else {
		return findMax(height(tree->pLeft), height(tree->pRight)) + 1;
	}
}
int main()
{
	AVL* tree;
	tree = NULL;
	/*string data;
	string meaning;
	do {
		cout << "Nhap du lieu, . de ket thuc: ";
		getline(cin, data);
		cin.ignore();
		cout << "Nhap nghia cua tu: ";
		getline(cin, meaning);
		if (data == ".") break;
		InsertNode(tree, data,meaning);
	} while (data != ".");
	cout << "cay AVL vua tao: " << endl;
	Traverse(tree);
	if (search(tree, "dan")) {
		DeleteNode(tree, "dan");
	}*/
	clock_t start = clock();
	tree = getData();
	cout << "thoi gian doc file: " << (clock() - start) <<"ms" << endl;
	ofstream outfile;
	outfile.open("save.txt");
	start = clock();
	saveInfo(tree, outfile);
	cout << "thoi gian luu file: " << (clock() - start) <<"ms" << endl;
	cout << "save complete" << endl;
	string key = "dan";
	string mean = "a very handsome man :v";
	start = clock();
	InsertNode(tree, key, mean);
	cout << "thoi gian insert: " << (clock() - start) <<"ms" << endl;
	start = clock();
	AVL* searchRessult = search(tree, key);
	cout << "thoi gian search: " << (clock() - start) <<"ms" << endl;
	if (searchRessult == NULL) cout << "khong tim thay" << endl;
	else cout << key << ": " << searchRessult->meaning << endl;
	start = clock();
	DeleteNode(tree, key);
	cout << "thoi gian xoa: " << (clock() - start) <<"ms" << endl;
	changemean(tree, key, "that ra thi khong nhung dep giai ma con khoai to nua :V");
	searchRessult = search(tree,key);
	if (searchRessult == NULL) cout << "khong tim thay" << endl;
	else cout << key << ": " << searchRessult->meaning << endl;
	cout << "height: " << height(tree) << endl;
	cout << sizeof(InsertNode(tree, key, mean));
	delete tree;
	delete searchRessult;
	return 0;
}


