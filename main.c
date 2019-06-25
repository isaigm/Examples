#include <stdio.h>
#include <stdlib.h>
#define MAX 255
typedef struct Usuario{
    int ID;
    char nombre[MAX];
    char telefono[MAX];
}Usuario;
typedef struct Automovil{
    char modelo[MAX];
    char marca[MAX];
    char placa[MAX];
    int tiempo;
    int ID;
    int ID_usuario;
    int estacionado;
}Automovil;
typedef struct NodoP{
    struct NodoP *sgte;
    Automovil dato;
}NodoP;
typedef struct Pila{
    NodoP *ultimo;
    int tam;
}Pila;
typedef struct NodoC{
    struct NodoC *sgte;
    Pila *autos;
}NodoC;
typedef struct Cola{
    NodoC *inicio;
    NodoC *ultimo;
    int tam;
}Cola;
typedef struct NodoL{
    struct NodoL *sgte;
    Cola *lotes;
}NodoL;
typedef struct Lista{
    NodoL *inicio;
    int tam;
}Lista;
void limpiarSTDIN(void);
Pila * crearPila(void);
Cola * crearCola(void);
Lista * crearLista(void);
void agregarAPila(Pila *, Automovil);
void capturarAuto(int);
void agregarLote(Cola *, Pila *);
int verificarIDUsuario(int, FILE *);
int verificarIDAuto(int, FILE *);
void capturarUsuario(FILE *);
void mostrarUsuario(int, FILE *);
void agregarPiso(Lista *, NodoL *, Cola *);
void sacarAuto(Pila *p);
int main()
{
    int op;
    FILE *fp = NULL;
    do{
        printf("1 -> Agregar usuario\n2 -> Estacionar auto: ");
        scanf("%d", &op);
        limpiarSTDIN();
        switch(op){
        case -1:
            break;
        case 1:
            capturarUsuario(fp);
            break;
        case 2:

            break;
        default:
            printf("\x1b[31mOpcion invalida\x1b[0m\n");
            break;
        }

    }while(op != -1);
    return 0;
}
void limpiarSTDIN(void){
    int c;
    do{
        c = getchar();
    }while(c != '\n' && c != EOF);
}
Pila * crearPila(void){
    Pila *nueva = malloc(sizeof(Pila));
    nueva->tam = 0;
    nueva->ultimo = NULL;
    return nueva;
}
Cola * crearCola(void){
    Cola *nueva = malloc(sizeof(Cola));
    nueva->tam = 0;
    nueva->inicio = nueva->ultimo = NULL;
    return nueva;
}
Lista * crearLista(void){
    Lista *nueva = malloc(sizeof(Lista));
    nueva->tam = 0;
    nueva->inicio = NULL;
    return nueva;
}
void mostrarUsuario(int ID, FILE *fp){
    fp = fopen("usuarios.dat", "r+b");
    if(fp != NULL){
        if(verificarIDUsuario(ID, fp)){
            Usuario aux;
            fread(&aux, sizeof(Usuario), 1, fp);
            while(!feof(fp)){
                if(aux.ID == ID){
                    printf("Nombre del usuario -> %s", aux.nombre);
                    printf("Telefono del usuario -> %s", aux.telefono);
                    printf("ID del usuario -> %d\n", aux.ID);
                    break;
                }
                fread(&aux, sizeof(Usuario), 1, fp);
            }
        }else fprintf(stderr, "\x1b[31mUsuario no encontrado\x1b[0m\n");
    }else fprintf(stderr, "\x1b[31mError al abrir el archivo\x1b[0m\n");
}
void agregarAPila(Pila *p, Automovil a){
    NodoP *nuevo = malloc(sizeof(NodoP));
    nuevo->dato = a;
    nuevo->sgte = NULL;
    if(p->tam < 3){
        if(p->ultimo == NULL){
            p->ultimo = nuevo;
        }else{
            nuevo->sgte = p->ultimo;
            p->ultimo = nuevo;
        }
        p->tam++;
    }else printf("\x1b[31mNo hay cupo en el lote\x1b[0m\n");
}
int verificarIDUsuario(int ID, FILE *fp){
    fp = fopen("usuarios.dat", "r+b");
    Usuario aux;
    fread(&aux, sizeof(Usuario), 1, fp);
    while(!feof(fp)){
        if(aux.ID == ID) return 1;
        fread(&aux, sizeof(Usuario), 1, fp);
    }
    fclose(fp);
    return 0;
}
int verificarIDAuto(int ID, FILE *fp){
    fp = fopen("automoviles.dat", "r+b");
    Automovil aux;
    fread(&aux, sizeof(Automovil), 1, fp);
    while(!feof(fp)){
        if(aux.ID == ID) return 1;
        fread(&aux, sizeof(Automovil), 1, fp);
    }
    fclose(fp);
    return 0;
}
void capturarUsuario(FILE *fp){
    fp = fopen("usuarios.dat", "a+b");
    Usuario nuevo;
    printf("Nombre del usuario: ");
    fgets(nuevo.nombre, MAX, stdin);
    printf("Telefono del usuario: ");
    fgets(nuevo.telefono, MAX, stdin);
    printf("ID del usuario: ");
    scanf("%d", &nuevo.ID);
    limpiarSTDIN();
    while(verificarIDUsuario(nuevo.ID, fp)){
        fprintf(stderr, "\x1b[31mYa existe un usuario con ese ID, ingrese otro: \x1b[0m\n");
        scanf("%d", &nuevo.ID);
        limpiarSTDIN();
    }
    fwrite(&nuevo, sizeof(Usuario), 1, fp);
    fclose(fp);
    int op;
    printf("\x1b[36m---Capturando automoviles---\x1b[0m\n");
    do{
        capturarAuto(nuevo.ID);
        printf("\x1b[36m* -1 para salir *\x1b[0m\n");
        scanf("%d", &op);
        limpiarSTDIN();
    }while(op != -1);
}
void sacarAuto(Pila *p){
    if(p->tam == 0) printf("\x1b[31mNo hay autos estacionados\x1b[0m\n");
    else{
        Automovil aux;
        FILE *fp = NULL;
        aux = p->ultimo->dato;
        if(p->tam == 1){
            free(p->ultimo);
            p->ultimo = NULL;
        }else{
            NodoP *ptr = p->ultimo;
            p->ultimo = p->ultimo->sgte;
            free(ptr);
        }
        printf("\x1b[36m---Automovil a sacar---\x1b[0m\n");
        printf("Marca -> %s", aux.marca);
        printf("Modelo -> %s", aux.modelo);
        printf("Placa -> %s", aux.placa);
        printf("Tiempo de guardado -> %d\n", aux.tiempo);
        printf("\x1b[36m---Informacion sobre el dueño---\x1b[0m\n");
        mostrarUsuario(aux.ID_usuario, fp);
        p->tam--;
    }
}
void agregarLote(Cola *c, Pila *p){
    NodoC *nuevo = malloc(sizeof(NodoC));
    nuevo->sgte = NULL;
    nuevo->autos = p;
    if(c->inicio == NULL){
        c->inicio = c->ultimo = nuevo;
    }else{
        nuevo->sgte = c->ultimo;
        c->ultimo = nuevo;
    }
    c->tam++;
}
void agregarPiso(Lista *l, NodoL *ptr, Cola *c){
    NodoL *nuevo = malloc(sizeof(NodoL));
    if(l->inicio == NULL){
        l->inicio = nuevo;
        l->tam++;
    }
    else{
        if(ptr->sgte != NULL){
            agregarPiso(l, ptr->sgte, c);
        }else{
            ptr->sgte = nuevo;
            l->tam++;
        }
    }
}
void capturarAuto(int ID_usuario){
    Automovil nuevo;
    FILE *fp;
    fp = fopen("automoviles.dat", "a+b");
    nuevo.ID_usuario = ID_usuario;
    nuevo.estacionado = 0;
    printf("Marca del auto: ");
    fgets(nuevo.marca, MAX, stdin);
    printf("Modelo del auto: ");
    fgets(nuevo.modelo, MAX, stdin);
    printf("Placa del auto: ");
    fgets(nuevo.placa, MAX, stdin);
    printf("Tiempo de guardado: ");
    scanf("%d", &nuevo.tiempo);
    limpiarSTDIN();
    printf("ID del auto: ");
    scanf("%d", &nuevo.ID);
    limpiarSTDIN();
    while(verificarIDAuto(nuevo.ID, fp)){
        printf("\x1b[31mYa existe un automovil con tal ID, ingrese uno valido: \x1b[0m\n");
        scanf("%d", &nuevo.ID);
        limpiarSTDIN();
    }
    fwrite(&nuevo, 1, sizeof(Automovil), fp);
    fclose(fp);
}
