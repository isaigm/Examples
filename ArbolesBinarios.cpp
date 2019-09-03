#include <iostream>
#include <ncurses.h>
class Nodo{
public:
    friend class Arbol;
    Nodo(int dato){
        this->dato = dato;
        izq = der = nullptr;
    }
private:
    int dato;
    Nodo *izq;
    Nodo *der;
};
class Arbol{
public:
    Arbol(std::initializer_list <int> args){
        for(auto it = args.begin(); it != args.end(); it++){
            insertar(*it);
        }
    }
    Arbol(){}
    void eliminarDato(int dato){
        eliminarDato(&raiz, dato);
    }
    void insertar(int dato){
        insertar(&raiz, dato);
    }
    void inorden(int x, int y){
        inorden(raiz, x, y);
        std::cout << std::endl;
    }
    int altura(){
        return altura(raiz);
    }
    int nodos(){
        return nodos(raiz);
    }
    int factorDeEquilibrio(){
        return factorDeEquilibrio(raiz);
    }
private:
    Nodo *raiz = nullptr;
    void insertar(Nodo ** raiz_, int dato){
        if(*raiz_ == nullptr){
            *raiz_ = new Nodo(dato);
        }else{
            if((*raiz_)->dato > dato) insertar(&(*raiz_)->izq, dato);
            else if((*raiz_)->dato < dato) insertar(&(*raiz_)->der, dato);
        }
    }
    void inorden(Nodo *raiz, int x, int y){
        if(raiz != nullptr){
            inorden(raiz->izq, x - 3, y + 1);
            mvprintw(y, x, "%d", raiz->dato);
            refresh();
            inorden(raiz->der, x + 3, y + 1);
        }
    }
    int altura(Nodo *raiz_){
        if(raiz_ == nullptr){
            return -1;
        }else{
            int h1 = 1 + altura(raiz_->izq);
            int h2 = 1 + altura(raiz_->der);
            return std::max(h1, h2);

        }
    }
    int nodos(Nodo * raiz_){
        if(raiz_ == nullptr){
            return 0;
        }else{
            int der = nodos(raiz_->der);
            int izq = nodos(raiz_->izq);
            return 1 + izq + der;
        }
    }
    int factorDeEquilibrio(Nodo * raiz){
        if(raiz == nullptr) return 0;
        else{
            return altura(raiz->der) - altura(raiz->izq);
        }
    }
    void eliminarDato(Nodo **raiz_, int dato){
        if(*raiz_ != nullptr){
            if((*raiz_)->dato == dato){
                if((*raiz_)->izq == nullptr && (*raiz_)->der == nullptr){
                    eliminarNodo(raiz_);
                }
                else{
                    Nodo *aux = *raiz_;
                    if((*raiz_)->izq != nullptr && (*raiz_)->der == nullptr){
                        eliminarNodo(raiz_);
                        *raiz_ = aux->izq;
                    }else if((*raiz_)->der != nullptr && (*raiz_)->izq == nullptr){
                        eliminarNodo(raiz_);
                        *raiz_ = aux->der;
                    }else{
                        Nodo *nodo = nodoIzq((*raiz_)->der);
                        int dat = nodo->dato;
                        (*raiz_)->der = aux->der;
                        (*raiz_)->izq = aux->izq;
                        eliminarDato(raiz_, nodo->dato);
                        (*raiz_)->dato = dat;
                   }
                }
            }else if(dato < (*raiz_)->dato) eliminarDato(&(*raiz_)->izq, dato);
            else if(dato > (*raiz_)->dato) eliminarDato(&(*raiz_)->der, dato);
        }
    }
    Nodo * nodoIzq(Nodo *raiz_){
        if(raiz_->izq != nullptr){
            return nodoIzq(raiz_->izq);
        }
        return raiz_;
    }
    void eliminarNodo(Nodo **nodo){
        delete *nodo;
        *nodo = nullptr;
    }
};
int main(){
    int rows, cols;
    initscr();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    noecho();
    getmaxyx(stdscr, rows, cols);
    Arbol arbol {-10, -2, -11, 20, 12, 23, 80, 22, 21, 19};
    arbol.inorden(cols/2 - 3,  rows/2 - 12);
    int f = arbol.factorDeEquilibrio();
    return 0;
}

