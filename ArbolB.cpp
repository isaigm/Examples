
		
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
    void insertar(int clave) {
        try {
            raiz = insertar(raiz, clave);
        } catch (const char *er) {
            std::cout << er;
        }
    }
    void inorden() {
        inorden(raiz);
    }
private:
    Pagina* insertar(Pagina* raiz, int clave) {
        bool subir;
        int mediana;
        Pagina* pag;
        subir = empujar(&raiz, &pag, clave, mediana);
        if (subir) {
            Pagina* p;
            p = new Pagina(orden);
            p->clavesUsadas++;
            p->claves[1] = mediana;
            p->nodos[0] = raiz;
            p->nodos[1] = pag;
            raiz = p;
        }
        return raiz;
    }
    void inorden(Pagina* raiz) {
        if (raiz != nullptr) {
            inorden(raiz->nodos[0]);
            for (int i = 1; i <= raiz->clavesUsadas; i++) {
                std::cout << raiz->claves[i] << std::endl;
                inorden(raiz->nodos[i]);
            }
        }
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
            encontrado = clave == actual->claves[index];
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
    bool empujar(Pagina** pag, Pagina** nuevo, int clave, int &mediana) {
        int k;
        bool subir = false;
        if (*pag == nullptr) {
            subir = true;
            mediana = clave;
            *nuevo = nullptr;
        }
        else {
            bool esta = buscarNodo(*pag, clave, k);
            if (esta) throw "Clave duplicada\n";
            subir = empujar(&(*pag)->nodos[k], nuevo, clave, mediana);
            if (subir) {
                if ((*pag)->estaLleno()) {
                    std::cout << "Lleno\n";
                    dividirPagina(pag, nuevo, mediana, k);
                }
                else {
                    subir = false;
                    insertarEnPagina(pag, *nuevo, mediana, k);
                }
            }
        }
        return subir;
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
        nueva->nodos[0] = (*pag)->nodos[(*pag)->clavesUsadas];
        (*pag)->clavesUsadas--;
        *nuevo = nueva;
    }
    int orden;
    Pagina* raiz;
};
int main()
{
    ArbolB ar(4);
    ar.insertar(2);
    ar.insertar(1);
    ar.insertar(3);
    ar.insertar(5);
    ar.insertar(-1);
    ar.inorden();
    return 0;
}

