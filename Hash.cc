#include <iostream>
#include <ctime>

using namespace std;

class Stopwatch  // timeLapse simples
{
public:
	Stopwatch(){
		time = 0;
	}
	double TimeLapsed() {
		return (double)(time) / CLOCKS_PER_SEC;
	}
	void Start() { start = clock(); }
	void Pause() { 
		time = time + (clock() - start); 
	}
	
private:
	clock_t start;
	clock_t time;
};

struct PtHash {
	int chave;
	PtHash *prox;
};

void init(PtHash *&pt, int n){
	pt = new PtHash();
	pt->chave = n;
	pt->prox = NULL;
}

void insereI(PtHash **&hash, int num, int m){
	int i, mod;
	mod = num % m;
	i = mod + 1;
	while (i != num % m){
		if (hash[i] == NULL){
			init(hash[i], num);
			break;
		}
		else{
			i += 1;
			i = i % m;
		}
	}
}

void insereE(PtHash *&pt, int num){
	if(pt->prox == NULL){
		PtHash *temp;
		init(temp, num);
		pt->prox = temp;
	}
	else{
		insereE(pt->prox, num);
	}
}


int main(){
	char c;
	int n, m, i, num, mod;
	Stopwatch sw;

	cin >> n >> m >> c; // c = interno, ou externo; n = quantidade; m = tamanho da hash

	PtHash **hash = new PtHash*[m];
	for (i = 0; i < m; i++){
		hash[i] = NULL;
	}

	if ( n <= m ){
		if (c == 'i'){
			for (i = 0; i < n; i++){
				cin >> num;
				mod = num % m;
				sw.Start();
				if (hash[mod] == NULL){
					init(hash[mod], num);
				}
				else{
					insereI(hash, num, m);
				}
				sw.Pause();
			}

		}
		else if (c == 'e'){
			for (i = 0; i < n; i++){
				cin >> num;
				mod = num % m;
				sw.Start();
				if (hash[mod] == NULL){
					init(hash[mod], num);
				}
				else{
					insereE(hash[mod], num);
				}
				sw.Pause();
			}
		}
		cout << sw.TimeLapsed();
		cin.get();
		cin.get();
	}
}