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
    }
    int altura(){
        return altura(raiz);
    }
    int nodos(){
        return nodos(raiz);
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
    void eliminarDato(Nodo **raiz_, int dato){
        if(*raiz_ != nullptr){
            if((*raiz_)->dato == dato){
                if((*raiz_)->izq == nullptr && (*raiz_)->der == nullptr) eliminarNodo(*raiz_);
                else{
                    Nodo *aux = *raiz_;
                    if((*raiz_)->izq != nullptr && (*raiz_)->der == nullptr){
                        *raiz_ = (*raiz_)->izq;
                        eliminarNodo(aux);
                    }else if((*raiz_)->der != nullptr && (*raiz_)->izq == nullptr){
                        *raiz_ = (*raiz_)->der;
                        eliminarNodo(aux);
                    }
                }
            }else if(dato < (*raiz_)->dato) eliminarDato(&(*raiz_)->izq, dato);
            else if(dato > (*raiz_)->dato) eliminarDato(&(*raiz_)->der, dato);
        }
    }
    void eliminarNodo(Nodo *nodo){
        delete nodo;
        nodo = nullptr;
    }
};
int main(){
    int rows, cols;
    initscr();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    noecho();
    getmaxyx(stdscr, rows, cols);
    Arbol arbol {10, 11, -10, -12, -13, 12, -15, -11, -9, -8, 13, 20};
    arbol.eliminarDato(13);
    arbol.eliminarDato(12);
    arbol.inorden(cols/2 - 1,  rows/2 - 10);
    addch('\n');
    refresh();
    return 0;
}
