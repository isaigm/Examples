#include <iostream>
class Pagina {
	friend class ArbolB;
public:
	Pagina(int nClaves) {
		clavesUsadas = 0;
		this->nClaves = nClaves;
		claves = new int[nClaves + 1];
		nodos = new Pagina * [nClaves + 1];
		for (int i = 0; i <= nClaves; i++) {
			nodos[i] = nullptr;
		}
	}
private:
	bool estaLleno() {
		return clavesUsadas == nClaves;
	}
	bool esHoja() {
		int cont = 0;
		for (int i = 0; i < nClaves + 1; i++) {
			if (nodos[i] == nullptr) cont++;
		}
		return cont == nClaves + 1;
	}
	int nClaves, clavesUsadas;
	int* claves;
	Pagina** nodos;
};
class ArbolB {
public:
	ArbolB(int orden) {
		this->orden = orden;
		raiz = new Pagina(orden);
	}
	Pagina* insertar(int clave) {
		raiz = insertar(raiz, clave);
	}
private:
	Pagina* insertar(Pagina* raiz, int clave) {

	}
	bool buscarNodo(Pagina* actual, int clave, int& rama) {
		int index;
		bool encontrado;
		if (clave < actual->claves[1]) {
			encontrado = false;
			index = 0;
		}
		else {
			index = actual->clavesUsadas;
			while (clave < actual->claves[index] && (index > 1)) {
				index--;
			}
			encontrado == clave == actual->claves[index];
		}
		rama = index;
		return encontrado;
	}
	Pagina* buscar(Pagina* actual, int clave, int& rama) {
		if (actual == nullptr) {
			return nullptr;
		}
		else {
			bool encontrado = buscarNodo(actual, clave, rama);
			if (encontrado) {
				return actual;
			}
			else {
				return buscar(actual->nodos[rama], clave, rama);
			}
		}
	}
	void insertarEnPagina(Pagina** pag, Pagina *ramDer, int clave, int pos) {
		for (int i = (*pag)->clavesUsadas; i >= pos + 1; i--) {
			(*pag)->claves[i + 1] = (*pag)->claves[i];
			(*pag)->nodos[i + 1] = (*pag)->nodos[i];
		}
		(*pag)->claves[pos + 1] = clave;
		(*pag)->nodos[pos + 1] = ramDer;
		(*pag)->clavesUsadas++;
	}
	void dividirPagina(Pagina** pag, Pagina** nuevo, int& mediana, int pos) {
		int i, posMdna, k;
		Pagina* nueva;
		nueva = new Pagina(orden);
		k = pos;
		posMdna = (k <= orden / 2) ? orden / 2 : orden / 2 + 1;
		for (i = posMdna + 1; i < orden; i++) {
			nueva->claves[i - posMdna] = (*pag)->claves[i];
			nueva->nodos[i - posMdna] = (*pag)->nodos[i];
			
		}
		nueva->clavesUsadas = orden - 1 - posMdna;
		(*pag)->clavesUsadas = posMdna;
		if (k <= orden / 2) insertarEnPagina(pag, *nuevo, mediana, pos);
		else {
			pos = k - posMdna;
			insertarEnPagina(&nueva, *nuevo, mediana, pos);
		}
		mediana = (*pag)->claves[(*pag)->clavesUsadas];
	}
	int orden;
	Pagina* raiz;
};
int main()
{
	ArbolB ar(4);
	ar.insertar(2);
	return 0;
}

