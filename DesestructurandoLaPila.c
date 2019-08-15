#include <stdio.h>
#include <stdlib.h>
typedef struct Nodo{
	struct Nodo *sgte;
	int dato;
}Nodo;
typedef struct Pila{
	Nodo *ultimo;
}Pila;
Nodo * crear_nodo(int dato){
	Nodo *nuevo = (Nodo *) malloc(sizeof(Nodo));
	nuevo->dato = dato;
	nuevo->sgte = NULL;
	return nuevo;
}
Pila * crear_pila(){
	Pila *pila = malloc(sizeof(Pila));
	pila->ultimo = NULL;
	return pila;
}
void push(Pila * pila, Nodo * nuevo){
	if(pila->ultimo == NULL){
		pila->ultimo  = nuevo;
	}else{
		nuevo->sgte = pila->ultimo ;
		pila->ultimo  = nuevo;
	}
}
Nodo * pop(Pila **pila){
	if((*pila)->ultimo != NULL){
		Nodo *aux = (*pila)->ultimo;
		(*pila)->ultimo = aux->sgte;
		return aux;
	}
	return NULL;
}

Nodo * busqueda(Pila **pila, int dato){
	Nodo *aux2;
	if((*pila)->ultimo != NULL){
		Nodo *aux = pop(pila);
		if(aux->dato == dato){
			push(*pila, aux);
			return aux;
		}else{
			aux2 = busqueda(pila, dato);
			push(*pila, aux);
		}
		return aux2;
	}else return NULL;
}
int main(int argc, char *argv[]) {
	Pila *pila = crear_pila();
	push(pila, crear_nodo(53));
	push(pila, crear_nodo(3));
	push(pila, crear_nodo(33));
	
	Nodo *ptr = busqueda(&pila, 53);
	if(ptr == NULL) printf("SDS\n");
	else printf("%d\n\n", ptr->dato);
	Nodo *el = pila->ultimo;
	while(el != NULL){
		printf("%d\n", el->dato);
		el = el->sgte;
	}
	return 0;
}
