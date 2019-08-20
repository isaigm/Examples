#include <iostream>
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
    Arbol(){

    }
    void insertar(int dato){
        insertar(&raiz, dato);
    }
    void inorden(){
        inorden(raiz);
    }
    int altura(){
        return altura(raiz) - 1;
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
    void inorden(Nodo *raiz){
        if(raiz != nullptr){
            inorden(raiz->izq);
            std::cout << raiz->dato << std::endl;
            inorden(raiz->der);
        }
    }
    int altura(Nodo *raiz_){
        if(raiz_ == nullptr){
            return 0;
        }else{
            int h1 = 1 + altura(raiz_->izq);
            int h2 = 1 + altura(raiz_->der);
            return std::max(h1, h2);
        }
    }
};
int main(){
    Arbol arbol{ 10, 11, -10, -12, -13, 12};
    arbol.inorden();
    int h = arbol.altura();
    std::cout << h << std::endl;
    return 0;
}
