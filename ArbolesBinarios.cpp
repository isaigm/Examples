#include <iostream>
#include <ncurses.h>
#include <string>
#include <stack>
#include <sstream>
#include <vector>
bool esOperador(char e){
    return e == '+' || e == '*' || e == '-' || e == '/';
};
using Tokens = std::vector<std::string>;
Tokens tokenizer(std::string expr){
    Tokens tokens;
    size_t it = 0;
    size_t digitos = 0;
    while(it < expr.size()){
        if(expr[it] == '(' || expr[it] == ')' || esOperador(expr[it])){
            tokens.push_back(expr.substr(it, 1));
        }
       else if(std::isdigit(expr[it]) != 0){
            size_t aux = it;
            while(std::isdigit(expr[it]) != 0 && it < expr.size()){
                digitos++;
                it++;
            }
            tokens.push_back(expr.substr(aux, digitos));
            digitos = 0;
            it--;
       }
       it++;
    }
    return tokens;
}
Tokens postfija(std::vector<std::string> infija){
    std::vector<std::string> salida;
    std::stack<char> pila;
    auto procedencia = [](char e){
        if(e == '*' || e == '/') return 1;
        return 0;
    };
    while(infija.size() > 0){
        std::string token = infija[0];
        char e = token[0];
        infija.erase(infija.begin());
        if(std::isdigit(e) != 0) salida.push_back(token);
        if(e == '(') pila.push(e);
        if(e == ')'){
            while(!pila.empty() && pila.top() != '('){
                salida.push_back(std::string(1, pila.top()));
                pila.pop();
            }
            if(pila.top() == '(') pila.pop();
            else{
                //error
            }
        }
        if(esOperador(e)){
            while(!pila.empty() && esOperador(pila.top()) && procedencia(pila.top()) >= procedencia(e)){
                salida.push_back(std::string(1, pila.top()));
                pila.pop();
            }
            pila.push(e);
        }
    }
    while(!pila.empty()){
        salida.push_back(std::string(1, pila.top()));
        pila.pop();
    }
    return salida;
}

class Nodo{
public:
    friend class Arbol;
    Nodo(int dato){
        this->dato = dato;
        fe = 0;
        izq = der = nullptr;
        esOperador = false;
    }
private:
    int dato;
    int fe;
    bool esOperador;
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
    Arbol(Tokens postfija){
        std::stack<Nodo *> pila;
        while(!postfija.empty()){
            std::string token = postfija[0];
            char e = token[0];
            postfija.erase(postfija.begin());
            if(std::isdigit(e) != 0) {
                std::stringstream ss(token);
                int dato;
                ss >> dato;
                pila.push(new Nodo(dato));
            }
            if(esOperador(e)){
                if(pila.size() < 2) {} //error
                else{
                    Nodo *nuevo = new Nodo(e);
                    nuevo->esOperador = true;
                    Nodo *n1 = pila.top();
                    pila.pop();
                    Nodo *n2 = pila.top();
                    pila.pop();
                    nuevo->der = n1;
                    nuevo->izq = n2;
                    pila.push(nuevo);
                }
            }
        }
        raiz = pila.top();
    }
    void postorden(){
        postorden(raiz);
        std::cout << std::endl;
    }
    void preorden(){
        preorden(raiz);
        std::cout << std::endl;
    }
    void eliminarDato(int dato){
        eliminarDato(&raiz, dato);
    }
    void insertar(int dato){
        insertar(&raiz, dato);
    }
    void inorden(){
        inorden(raiz);
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
    float evaluar(){
        return evaluar(raiz);
    }
private:
    Nodo *raiz = nullptr;
    void insertar(Nodo ** raiz_, int dato){
        if(*raiz_ == nullptr){
            *raiz_ = new Nodo(dato);
        }else{
            if((*raiz_)->dato > dato) insertar(&(*raiz_)->izq, dato);
            else if((*raiz_)->dato < dato) insertar(&(*raiz_)->der, dato);
            balancear(raiz_);
            (*raiz_)->fe = factorDeEquilibrio(raiz);
        }
    }
    void inorden(Nodo *raiz){
        if(raiz != nullptr){
            std::cout << "(";
            inorden(raiz->izq);
            if(raiz->esOperador) std::cout << char(raiz->dato);
            else std::cout << raiz->dato;
            inorden(raiz->der);
            std::cout << ")";
        }
    }
    float evaluar(Nodo *raiz){
       float res = 0;
        if(raiz != nullptr){
            if(raiz->der == nullptr && raiz->izq == nullptr) return raiz->dato;
            if(raiz->esOperador){
                switch(raiz->dato){
                    case '+':
                        res = evaluar(raiz->der) + evaluar(raiz->izq);
                        break;
                    case '-':
                        res = evaluar(raiz->izq) - evaluar(raiz->der);
                        break;
                    case '*':
                        res = evaluar(raiz->der) * evaluar(raiz->izq);
                        break;
                    case '/':
                        res = evaluar(raiz->izq) / evaluar(raiz->der);
                        break;
                }
            }
        }
        return res;
    }
    void postorden(Nodo *raiz){
        if(raiz != nullptr){
            postorden(raiz->izq);
            postorden(raiz->der);
            if(raiz->esOperador) std::cout << char(raiz->dato);
            else std::cout << raiz->dato;
            std::cout << " ";
        }
    }
    void preorden(Nodo *raiz){
        if(raiz != nullptr){
            if(raiz->esOperador) std::cout << char(raiz->dato);
            else std::cout << raiz->dato;
            std::cout << " ";
            preorden(raiz->izq);
            preorden(raiz->der);
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
    void rotacionII(Nodo **raiz){
        Nodo *izq = (*raiz)->izq;
        (*raiz)->izq = izq->der;
        izq->der = *raiz;
        *raiz = izq;
        (*raiz)->fe = factorDeEquilibrio(*raiz);
    }
    void balancear(Nodo **raiz){
        if(*raiz != nullptr){
            if((*raiz)->fe == -2){
                if(altura((*raiz)->izq->izq) >= altura((*raiz)->izq->der)){
                    rotacionII(raiz);
                }else{
                    rotacionID(raiz);
                }
            }else if((*raiz)->fe == 2){
                if(altura((*raiz)->der->der) >= altura((*raiz)->der->izq)){
                    rotacionDD(raiz);
                }else{
                    rotacionDI(raiz);
                }
            }
        }
    }
    void rotacionDD(Nodo **raiz){
        Nodo *der = (*raiz)->der;
        (*raiz)->der = der->izq;
        der->izq = *raiz;
        *raiz = der;
        (*raiz)->fe = factorDeEquilibrio(*raiz);
    }
    void rotacionID(Nodo **raiz){
        rotacionDD(&(*raiz)->izq);
        rotacionII(raiz);
    }
    void rotacionDI(Nodo **raiz){
        rotacionII(&(*raiz)->der);
        rotacionDD(raiz);
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
                        balancear(raiz_);
                        (*raiz_)->fe = factorDeEquilibrio(raiz);
                    }else if((*raiz_)->der != nullptr && (*raiz_)->izq == nullptr){
                        eliminarNodo(raiz_);
                        *raiz_ = aux->der;
                        balancear(raiz_);
                        (*raiz_)->fe = factorDeEquilibrio(raiz);
                    }else{
                        Nodo *nodo = nodoIzq((*raiz_)->der);
                        int dat = nodo->dato;
                        (*raiz_)->der = aux->der;
                        (*raiz_)->izq = aux->izq;
                        eliminarDato(raiz_, nodo->dato);
                        (*raiz_)->dato = dat;
                        balancear(raiz_);
                        (*raiz_)->fe = factorDeEquilibrio(raiz);
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
    Tokens tokens = tokenizer("(( 543*33)+(62/2)+3)*2 - 10");
    auto t = postfija(tokens);
    Arbol arbol(t);
    arbol.postorden();
    std::cout << std::fixed << arbol.evaluar() << std::endl;
    Arbol ar{1, 2, 3, 4, 5};
    ar.inorden();
    ar.eliminarDato(5);
    ar.inorden();
    std::cout << ar.factorDeEquilibrio() << std::endl;
    return 0;
}
