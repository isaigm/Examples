#include <iostream>
class Pagina {
    friend class ArbolB;
public:
    Pagina(int nClaves) {
        clavesUsadas = 0;
        this->nClaves = nClaves;//nClaves es el orden del arbol, las claves empiezan a partir del primer indice
        claves = new int[nClaves];
        nodos = new Pagina * [nClaves];//el numero de ramas es igual al orden del arbol
        for (int i = 0; i < nClaves; i++) {
            nodos[i] = nullptr;
        }
    }
private:
    bool estaLleno() {
        return clavesUsadas == nClaves-1;
    }
    int nClaves, clavesUsadas;
    int* claves;
    Pagina** nodos;
};
class ArbolB {
public:
    //Constructor para cargar el arbol con los valores que hay en args
    ArbolB(int orden, std::initializer_list<int> args) : ArbolB(orden){
        for(auto it = args.begin(); it != args.end(); it++){
            insertar(*it);
        }
    }
    ArbolB(int orden) {
        this->orden = orden;
        raiz = nullptr;
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
            std::cout << "Here\n";
            Pagina* p;
            p = new Pagina(orden);
            p->clavesUsadas = 1;
            p->claves[1] = mediana;
            p->nodos[0] = raiz;//claves menores
            p->nodos[1] = pag;//claves mayores
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
    //La rama guarda el indice del nodo por el que se deberia bajar para encontrar el nodo que tiene la clave, devuelve
    //true si lo encuentra, false si no
    bool buscarNodo(Pagina* actual, int clave, int& rama) {
        int index;
        bool encontrado;
        //si la clave es menor que la primera clave, entonces deberia bajar por el nodo 0, que contiene
        //claves mas pequeña que claves[1]
        if (clave < actual->claves[1]) {
            encontrado = false;
            index = 0;
        }
        else {
            index = actual->clavesUsadas;
            //iterando en claves hasta encontrar el indice de la clave que es mayor a la clave que se va a insertar
            //si la clave existe, simplemente acaba y devuelve true
            while (clave < actual->claves[index] && (index > 1)) {
                index--;
            }
            encontrado = clave == actual->claves[index];
        }
        rama = index;
        return encontrado;
    }
    //devuelve un puntero a la pagina que tenga la clave o nullptr si no hay pagina con tal clave
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
                return buscar(actual->nodos[rama], clave, rama);//baja por la rama
            }
        }
    }
    bool empujar(Pagina** pag, Pagina** nuevo, int clave, int &mediana) {
        int rama;
        bool subir = false;
        if (*pag == nullptr) {
            subir = true;
            mediana = clave;
            *nuevo = nullptr;
        }
        else {
            bool esta = buscarNodo(*pag, clave, rama);
            if (esta) throw "Clave duplicada\n";
            subir = empujar(&(*pag)->nodos[rama], nuevo, clave, mediana);
            if (subir) {
                if ((*pag)->estaLleno()) {
                    dividirPagina(pag, nuevo, mediana, rama);
                }
                else {
                    subir = false;
                    insertarEnPagina(pag, *nuevo, mediana, rama);
                }
            }
        }
        return subir;
    }
    void insertarEnPagina(Pagina** pag, Pagina *ramDer, int clave, int pos) {
        //moviendo los elemento mayores que clave a la derecha
        //pos es el indice donde clave > claves[indice]
        //se inserta en pos + 1
        for (int i = (*pag)->clavesUsadas; i >= pos + 1; i--) {
            (*pag)->claves[i + 1] = (*pag)->claves[i];
            (*pag)->nodos[i + 1] = (*pag)->nodos[i];
        }
        (*pag)->claves[pos + 1] = clave;
        (*pag)->nodos[pos + 1] = ramDer;
        (*pag)->clavesUsadas++;
    }
    void dividirPagina(Pagina** pag, Pagina** nuevo, int& mediana, int pos) {
        std::cout << pos <<"#"<<std::endl;
        int i, posMdna, k;
        Pagina* nueva;
        nueva = new Pagina(orden);
        k = pos;//aqui es donde se insertaria si el nodo no estuviera lleno
        posMdna = (k <= orden / 2) ? orden / 2 : orden / 2 + 1;
        std::cout << posMdna << std::endl;
        //moviendo los valores mayores que la mediana a una nueva pagina
        for (i = posMdna + 1; i < orden; i++) {
            nueva->claves[i - posMdna] = (*pag)->claves[i];
            nueva->nodos[i - posMdna] = (*pag)->nodos[i];
        }
        nueva->clavesUsadas = orden - 1 - posMdna;
        (*pag)->clavesUsadas = posMdna;
        //que k sea <= orden/2 significa que se puede la clave se puede inserta en pag porque ademas de que hay espacio
        //la misma clave no será más grande que claves[clavesUsadas]
        //si no, se inserta en la nueva pagina
        //cuando se pasan los mayores que la mediana a la nueva pagina no se eliminan de la pagina de donde sacaron
        if (k <= orden / 2) insertarEnPagina(pag, *nuevo, mediana, pos);
        else {
            pos = k - posMdna;
            insertarEnPagina(&nueva, *nuevo, mediana, pos);
        }
        mediana = (*pag)->claves[(*pag)->clavesUsadas];//se obtiene la mediana
        nueva->nodos[0] = (*pag)->nodos[(*pag)->clavesUsadas];//se mueve la pagina de la derecha de la mediana a la pagina izquierda de la nueva pagina
        (*pag)->clavesUsadas--;//la mediana tiene que subir
        *nuevo = nueva;
    }
    int orden;
    Pagina* raiz;
};
int main()
{
    ArbolB ar(5, {1, 2, 3, 4, 5, 6, 7});
    ar.insertar(-1);
    ar.inorden();
    return 0;
}

