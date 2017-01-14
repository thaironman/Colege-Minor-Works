#include <iostream>

using namespace std;

struct Ptarv {
	int chave;
	Ptarv *left;
	Ptarv *right;
};

void init(Ptarv *&pt, int n){
	pt = new Ptarv();
	pt->chave = n;
	pt->left = NULL;
	pt->right = NULL;
}

void search(Ptarv *&pai, Ptarv *&pt, int n){
	pt = pai;

	while (pt != NULL){
		if (n == pt->chave){
			break;
		}
		if (n < pt->chave){
			pai = pt;
			pt = pt->left;
		} 
		else {
			pai = pt;
			pt = pt->right;
		}
	}
}

void insert(Ptarv *&raiz, int n){
	Ptarv* pt;
	Ptarv* pai = raiz;
	if (raiz != NULL){
		search(pai, pt, n);
		if (n < pai->chave && pai->left == NULL){
			init(pt, n);
			pai->left = pt;
		}
		else if (n > pai->chave && pai->right == NULL){
			init(pt, n);
			pai->right = pt;
		}
	}
	else
		init(raiz, n);
}

void delPt(Ptarv *&raiz, int n){
	Ptarv* pt;
	Ptarv* pai = raiz;
	Ptarv* temp;
	if (raiz != NULL){
		search(pai, pt, n);
		temp = pt;
		if (pt == pai){ // n = raiz
			if (pt->right != NULL){
				raiz = pt->right;
				pt = raiz;
				while (pt->left != NULL)
					pt = pt->left;
				pt->left = pai->left;
				delete pai;
			}
			else if (pt->left != NULL){
				raiz = pt->left;
				delete pai;
			}
			else{
				delete raiz;
				raiz = NULL;
			}
		}
		else if (pt->chave > raiz->chave){ // pt > raiz
			if (pt->chave > pai->chave){ // pt > pai >= raiz
				if (pt->right != NULL){
					pt = pt->right;
					pai->right = pt;
					while (pt->left != NULL)
						pt = pt->left;
					pt->left = temp->left;
					delete temp;
				}
				else if (pt->left != NULL){
					pai->right = pt->left;
					delete pt;
				}
				else{
					pai->right = NULL;
					delete pt;
				}

			}
			else{ // pai > pt > raiz
				if (pt->right != NULL){
					pt = pt->right;
					pai->left = pt;
					while (pt->left != NULL)
						pt = pt->left;
					pt->left = temp->left;
					delete temp;
				}
				else if (pt->left != NULL){
					pai->left = pt->left;
					delete pt;
				}
				else{
					pai->left = NULL;
					delete pt;
				}
			}
		}
		else{ // pt < raiz
			if (pt->chave > pai->chave){ // raiz > pt > pai
				if (pt->right != NULL){
					pt = pt->right;
					pai->right = pt;
					while (pt->left != NULL)
						pt = pt->left;
					pt->left = temp->left;
					delete temp;
				}
				else if (pt->left != NULL){
					pai->right = pt->left;
					delete pt;
				}
				else{
					pai->right = NULL;
					delete pt;
				}

			}
			else{ // raiz >= pai > pt
				if (pt->right != NULL){
					pt = pt->right;
					pai->left = pt;
					while (pt->left != NULL)
						pt = pt->left;
					pt->left = temp->left;
					delete temp;
				}
				else if (pt->left != NULL){
					pai->left = pt->left;
					delete pt;
				}
				else{
					pai->left = NULL;
					delete pt;
				}
			}
		}
	}
}

void print(Ptarv *&raiz){
	if (raiz != NULL)
		cout << raiz->chave << " ";
	if (raiz->left != NULL)
		print(raiz->left);
	if (raiz->right != NULL)
		print(raiz->right);
}

int main(){
	Ptarv* raiz = NULL;
	int n= 0;
	char stance = 'i';

	while (stance != 'x'){
		stance = 'i';
		cin >> stance;
		if (stance != 'p' && stance != 'x')
			cin >> n;
		switch (stance){
			case 'i':
				insert(raiz, n);
				break;
			case 'r':
				delPt(raiz, n);
				break;
			case 'p':
				print(raiz);
				cout << "\n";
				break;
		}
	}
	return 0;
}