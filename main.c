#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define MAX 128
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
    int piso;
}Automovil;
typedef struct NodoP{
    struct NodoP *sgte;
    Automovil dato;
}NodoP;
typedef struct Pila{
    NodoP *inicio;
    int tam;
}Pila;
typedef struct NodoC{
    struct NodoC *sgte;
    Pila *autos;
}NodoC;
typedef struct Cola{
    NodoC *inicio;
    int tam;
    int flag;
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
void agregarAPila(Pila **, Automovil);
void capturarAuto(int);
void agregarLote(Cola **, Pila **);
int verificarIDUsuario(int);
int verificarIDAuto(int);
void capturarUsuario(void);
void mostrarUsuario(int);
void agregarPiso(Lista *);
void sacarAuto(Lista **);
void modificarAuto(int, int, int);
void sacarDeP(Pila **, int);
void loteAXPiso(Lista **);
void mostrarEstado(Lista *);
void mostrarLotes(Cola *);
void mostrarAutos(Pila *);
Automovil obtenerAuto(int, int);
void estacionarAuto(Lista *);
void actualizar(int);
int main()
{
    int op;
    Lista *estacionamiento = crearLista();
    signal(SIGINT, actualizar);
    do{
        printf("1 -> Agregar usuario\n2 -> Estacionar auto\n3 -> Agregar piso\n4 -> Agregar lote a X piso\n5 -> Sacar auto\n6 -> Mostrar estado del estacionamiento (-1 para salir): ");
        scanf("%d", &op);
        limpiarSTDIN();
        switch(op){
        case -1:
            break;
        case 1:
            capturarUsuario();
            break;
        case 2:
            if(estacionamiento->tam > 0){
                estacionarAuto(estacionamiento);
            }else printf("\x1b[31mNo ha agregado pisos\x1b[0m\n");
            break;
        case 3:
            agregarPiso(estacionamiento);
            printf(" ----------------------------\n");
            printf("|Se ha agregado un nuevo piso|\n");
            printf(" ----------------------------\n");
            break;
        case 4:
            loteAXPiso(&estacionamiento);
            break;
        case 5:
            sacarAuto(&estacionamiento);
            break;
        case 6:
            if(estacionamiento->tam > 0){
                 mostrarEstado(estacionamiento);
            }else printf("\x1b[31mEstacionamiento vacio\x1b[0m\n");
            break;
        default:
            printf("\x1b[31mOpcion invalida\x1b[0m\n");
            break;
        }

    }while(op != -1);
    actualizar(0);
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
    nueva->inicio = NULL;
    return nueva;
}
Cola * crearCola(void){
    Cola *nueva = malloc(sizeof(Cola));
    nueva->tam = 0;
    nueva->inicio = NULL;
    return nueva;
}
Lista * crearLista(void){
    Lista *nueva = malloc(sizeof(Lista));
    nueva->tam = 0;
    nueva->inicio = NULL;
    return nueva;
}
void actualizar(int k){
    FILE *fp = fopen("automoviles.dat", "r+b");
    if(fp != NULL){
        Automovil a;
        fread(&a, sizeof(Automovil), 1, fp);
        while(!feof(fp)){
            a.estacionado = 0;
            long pos = ftell(fp) - (long)sizeof(Automovil);
            fseek(fp, pos, SEEK_SET);
            fwrite(&a, sizeof(Automovil), 1, fp);
            fread(&a, sizeof(Automovil), 1, fp);
        }
        fclose(fp);
    }
    raise(SIGTERM);
}
void mostrarUsuario(int ID){
    FILE *fp = fopen("usuarios.dat", "r+b");
    if(fp != NULL){
        if(verificarIDUsuario(ID)){
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
void agregarAPila(Pila **p, Automovil a){
    NodoP *nuevo = malloc(sizeof(NodoP));
    nuevo->dato = a;
    nuevo->sgte = NULL;
    if((*p)->tam < 3){
        if((*p)->inicio == NULL){
            (*p)->inicio = nuevo;
        }else{
            NodoP *ptr = (*p)->inicio;
            while(ptr->sgte != NULL){
                ptr = ptr->sgte;
            }
            ptr->sgte = nuevo;
        }
        (*p)->tam++;
    }else printf("\x1b[31mNo hay cupo en el lote\x1b[0m\n");
}
int verificarIDUsuario(int ID){
    FILE *fp = fopen("usuarios.dat", "r+b");
    if(fp != NULL){
        Usuario aux;
        fread(&aux, sizeof(Usuario), 1, fp);
        while(!feof(fp)){
            if(aux.ID == ID) return 1;
            fread(&aux, sizeof(Usuario), 1, fp);
        }
        fclose(fp);
    }else printf("\x1b[31mNo existe el archivo\x1b[0m\n");
    return 0;
}
int verificarIDAuto(int ID){
    FILE *fp = fopen("automoviles.dat", "r+b");
    if(fp != NULL){
        Automovil aux;
        fread(&aux, sizeof(Automovil), 1, fp);
        while(!feof(fp)){
            if(aux.ID == ID) return 1;
            fread(&aux, sizeof(Automovil), 1, fp);
        }
        fclose(fp);
    }else printf("\x1b[31mNo existe el archivo\x1b[0m\n");
    return 0;
}
void capturarUsuario(void){
    FILE *fp = fopen("usuarios.dat", "a+b");
    Usuario nuevo;
    printf("Nombre del usuario: ");
    fgets(nuevo.nombre, MAX, stdin);
    printf("Telefono del usuario: ");
    fgets(nuevo.telefono, MAX, stdin);
    printf("ID del usuario: ");
    scanf("%d", &nuevo.ID);
    limpiarSTDIN();
    while(verificarIDUsuario(nuevo.ID)){
        fprintf(stderr, "\x1b[31mYa existe un usuario con ese ID, ingrese otro: \x1b[0m\n");
        scanf("%d", &nuevo.ID);
        limpiarSTDIN();
    }
    fwrite(&nuevo, sizeof(Usuario), 1, fp);
    fclose(fp);
    int op;
    printf("\x1b[36m---Capturando automoviles del cliente con ID %d---\x1b[0m\n", nuevo.ID);
    do{
        capturarAuto(nuevo.ID);
        printf("\x1b[36m* -1 para salir *\x1b[0m\n");
        scanf("%d", &op);
        limpiarSTDIN();
    }while(op != -1);
}
void sacarDeP(Pila **p, int ID_auto){
    if((*p)->tam > 0){
        if((*p)->tam == 1){
            modificarAuto(ID_auto, 0, -1);
            free((*p)->inicio);
            (*p)->inicio = NULL;
            (*p)->tam--;
        }else{
            NodoP *ptr = (*p)->inicio;
            int cont = 0;
            while(ptr != NULL){
                if(ptr->dato.ID == ID_auto) break;
                cont++;
                ptr = ptr->sgte;
            }
            if(cont == (*p)->tam -1){
                modificarAuto(ID_auto, 0, -1);
                ptr = (*p)->inicio;
                while(ptr->sgte->sgte != NULL) ptr = ptr->sgte;
                free(ptr->sgte);
                ptr->sgte = NULL;
                (*p)->tam--;
            }else printf("\x1b[31mSe tienen que sacar los autos de enfrente primero\x1b[0m\n");
        }
    }
}
void sacarAuto(Lista **lista){
    if((*lista)->tam > 0){
        int ID_auto;
        printf("Ingrese el ID del auto a sacar: ");
        scanf("%d", &ID_auto);
        if(verificarIDAuto(ID_auto)){
            FILE *fp = fopen("automoviles.dat", "r+b");
            Automovil aux;
            fread(&aux, sizeof(Automovil), 1, fp);
            while(!feof(fp)){
                if(aux.ID == ID_auto) break;
                fread(&aux, sizeof(Automovil), 1, fp);
            }
            if(aux.estacionado == 1){
                int piso = aux.piso, cont = 0;
                NodoL *ptr = (*lista)->inicio;
                while(ptr != NULL){
                    if(cont == piso) break;
                    cont++;
                    ptr = ptr->sgte;
                }
                if(ptr == NULL) printf("NULL\n");
                NodoC *n = ptr->lotes->inicio;
                int flag = 0;
                while(n != NULL && !flag){
                    NodoP *p = n->autos->inicio;
                    while(p != NULL){
                        if(p->dato.ID == ID_auto){
                            sacarDeP(&n->autos, ID_auto);
                            flag = 1;
                            break;
                        }
                        p = p->sgte;
                    }
                    n = n->sgte;
                }
            }else printf("\x1b[31mTal auto no ha sido estacionado\x1b[0m\n");

        }else printf("\x1b[31mNo existe tal auto\x1b[0m\n");
    }else printf("\x1b[31mEstacionamiento vacio\x1b[0m\n");
}
void agregarLote(Cola **c, Pila **p){
    NodoC *nuevo = malloc(sizeof(NodoC));
    nuevo->sgte = NULL;
    nuevo->autos = *p;
    (*c)->flag = 1;
    if((*c)->inicio == NULL){
        (*c)->inicio = nuevo;
    }else{
        NodoC *ptr = (*c)->inicio;
        while(ptr->sgte != NULL){
            ptr = ptr->sgte;
        }
        ptr->sgte = nuevo;
    }
    (*c)->tam++;
}
void agregarPiso(Lista *l){
    NodoL *nuevo = malloc(sizeof(NodoL));
    nuevo->sgte = NULL;
    nuevo->lotes = crearCola();
    if(l->inicio == NULL) l->inicio = nuevo;
    else{
        NodoL *ptr = l->inicio;
        while(ptr->sgte != NULL) ptr = ptr->sgte;
        ptr->sgte = nuevo;
    }
    l->tam++;
}
void capturarAuto(int ID_usuario){
    Automovil nuevo;
    FILE *fp;
    fp = fopen("automoviles.dat", "a+b");
    nuevo.ID_usuario = ID_usuario;
    nuevo.estacionado = 0;
    nuevo.tiempo = -1;
    nuevo.piso = -1;
    printf("Marca del auto: ");
    fgets(nuevo.marca, MAX, stdin);
    printf("Modelo del auto: ");
    fgets(nuevo.modelo, MAX, stdin);
    printf("Placa del auto: ");
    fgets(nuevo.placa, MAX, stdin);
    printf("ID del auto: ");
    scanf("%d", &nuevo.ID);
    limpiarSTDIN();
    while(verificarIDAuto(nuevo.ID)){
        printf("\x1b[31mYa existe un automovil con tal ID, ingrese uno valido: \x1b[0m\n");
        scanf("%d", &nuevo.ID);
        limpiarSTDIN();
    }
    fwrite(&nuevo, sizeof(Automovil), 1, fp);
    fclose(fp);
}
Automovil obtenerAuto(int ID_usuario, int ID_auto){
    FILE *usuarios = fopen("usuarios.dat", "r+b");
    FILE *autos = fopen("automoviles.dat", "r+b");
    if(usuarios != NULL && autos != NULL){
        Usuario aux;
        int existe = 0, valido = 0;
        fread(&aux, sizeof(Usuario), 1, usuarios);
        while(!feof(usuarios)){
            if(aux.ID == ID_usuario){
                existe = 1;
                break;
            }
            fread(&aux, sizeof(Usuario), 1, usuarios);
        }
        if(existe){
            Automovil a;
            existe = 0;
            fread(&a, sizeof(Automovil), 1, autos);
            while(!feof(autos)){
                if(a.ID == ID_auto){
                    existe = 1;
                    if(ID_usuario == a.ID_usuario) valido = 1;
                    break;
                }
                fread(&a, sizeof(Automovil), 1, autos);
            }
            if(existe){
                if(valido){
                    if(a.estacionado == 0){
                        return a;
                    }else printf("\x1b[31mAutomovil ya estacionado\x1b[0m\n");
                }
                else printf("\x1b[31mTal automovil no pertenece al usuario\x1b[0m\n");
            }else printf("\x1b[31mNo existe el automovil\x1b[0m\n");
        }else printf("\x1b[31mNo existe el usuario\x1b[0m\n");
        fclose(usuarios);
        fclose(autos);
    }else printf("\x1b[31mNo existe alguno de los archivos necesarios\x1b[0m\n");
    Automovil at;
    at.ID = -128;
    return at;
}
void modificarAuto(int ID, int e, int piso){
    FILE *fp = fopen("automoviles.dat", "r+b");
    if(fp != NULL){
       Automovil aux;
       fread(&aux, sizeof(Automovil), 1, fp);
       while(!feof(fp)){
           if(aux.ID == ID){
               long pos = ftell(fp) - (long)sizeof(Automovil);
               fseek(fp, pos, SEEK_SET);
               aux.estacionado = e;
               aux.piso = piso;
               fwrite(&aux, sizeof(Automovil), 1, fp);
               break;
           }
           fread(&aux, sizeof(Automovil), 1, fp);
       }
       fclose(fp);
    }
}
void loteAXPiso(Lista **l){
    int index;
    printf("Ingrese el numero del piso: ");
    scanf("%d", &index);
    limpiarSTDIN();
    if((*l)->tam > 0){
        if(index >= 0 && index < (*l)->tam){
            int cont = 0;
            NodoL *ptr = (*l)->inicio;
            while(ptr != NULL){
                if(cont == index){
                    Pila *p = crearPila();
                    agregarLote(&ptr->lotes, &p);
                    printf(" --------------------------------\n");
                    printf("|Se ha agregado un lote al piso %d|\n", index);
                    printf(" --------------------------------\n");
                    break;
                }
                ptr = ptr->sgte;
                cont++;
            }
        }else printf("\x1b[31mFuera de rango\x1b[0m\n");
    }else printf("\x1b[31mNo hay pisos\x1b[0m\n");
}
void estacionarAuto(Lista *l){
    int ID_usuario, ID_auto;
    printf("ID del usuario: ");
    scanf("%d", &ID_usuario);
    limpiarSTDIN();
    printf("ID del auto: ");
    scanf("%d", &ID_auto);
    limpiarSTDIN();
    Automovil a = obtenerAuto(ID_usuario, ID_auto);
    if(a.ID != -128){
        int tiempo;
        printf("Ingrese numero de dias de estacionado: ");
        scanf("%d", &tiempo);
        limpiarSTDIN();
        int dias[50], ind = -1;
        dias[0] = 1;
        for(int i = 1; i < 50; i++) dias[i] = dias[i-1] + 2;
        if(tiempo >= 0 && tiempo < 1) ind = 0;
        else if(tiempo < 0) printf("\x1b[31mTiempo invalido\x1b[0m\n");
        if(ind != 0 && tiempo > 0){
            for(int i = 1; i < 50; i++){
                if(tiempo >= dias[i-1] && tiempo < dias[i]){
                    ind = i;
                    break;
                }
            }
        }
        if(ind != -1){
            if(ind < l->tam){
                int cont = 0;
                NodoL *ptr = l->inicio;
                while(ptr != NULL){
                    if(cont == ind) break;
                    ptr = ptr->sgte;
                    cont++;
                }
                Cola **c = &ptr->lotes;
                if((*c)->tam > 0){
                    NodoC *ptrC = (*c)->inicio;
                    int e = 0;
                    while(ptrC != NULL){
                        if(ptrC->autos->tam < 3){
                            agregarAPila(&ptrC->autos, a);
                            modificarAuto(a.ID, 1, ind);
                            e = 1;
                            break;
                        }
                        ptrC = ptrC->sgte;
                    }
                    if(!e) printf("\x1b[31mNo hay lotes para ese automovil\x1b[0m\n");
                }else printf("\x1b[31mHay lotes, pero ese no pisos\x1b[0m\n");
            }else printf("\x1b[31mNo hay pisos para ese automovil\x1b[0m\n");
        }
    }
}
void mostrarLotes(Cola *c){
    NodoC *ptr = c->inicio;
    int cont = 0;
    while(ptr != NULL){
        printf("---Lote %d---\n", cont);
        mostrarAutos(ptr->autos);
        ptr = ptr->sgte;
        cont++;
    }
}
void mostrarAutos(Pila *p){
    NodoP *ptr = p->inicio;
    while(ptr != NULL){
        printf("ID del auto -> %d\n", ptr->dato.ID);
        printf("ID del dueño -> %d\n", ptr->dato.ID_usuario);
        printf("Placa del auto -> %s", ptr->dato.placa);
        ptr = ptr->sgte;
    }
}
void mostrarEstado(Lista *l){
    NodoL *ptr = l->inicio;
    int cont = 0;
    while(ptr != NULL){
        printf("---Piso %d---\n", cont);
        mostrarLotes(ptr->lotes);
        ptr = ptr->sgte;
        cont++;
    }
}
