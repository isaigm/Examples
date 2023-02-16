#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <SFML/Graphics.h>
#define WIDTH 800
#define HEIGHT 600
static const uint n = 145;
void drawRects(sfRenderWindow *window, sfRectangleShape **rects){
    for (uint i = 0; i < n; i++) {
        sfRenderWindow_drawRectangleShape(window, rects[i], NULL);
    }
}
void draw(sfRenderWindow *window, sfRectangleShape **rects){
    sfRenderWindow_clear(window, sfWhite);
    drawRects(window, rects);
    sfRenderWindow_display(window);
}
int particion(sfRenderWindow *window, sfRectangleShape **rects, int inicio, int final){
    int i = inicio;
    int j = final;
    float pivote = sfRectangleShape_getSize(rects[(inicio + final)/2]).y;
    while (1) {
        while (sfRectangleShape_getSize(rects[i]).y < pivote) {
            i++;
        }
        while (sfRectangleShape_getSize(rects[j]).y > pivote) {
            j--;
        }
        if(i >= j){
            return j;
        }
        else{
            sfVector2f tam = sfRectangleShape_getSize(rects[i]);
            sfRectangleShape_setSize(rects[i], sfRectangleShape_getSize(rects[j]));
            sfRectangleShape_setSize(rects[j], tam);
            sfVector2f pos = sfRectangleShape_getPosition(rects[i]);
            pos.y  = HEIGHT - sfRectangleShape_getSize(rects[i]).y;
            sfRectangleShape_setPosition(rects[i], pos);
            pos = sfRectangleShape_getPosition(rects[j]);
            pos.y  = HEIGHT - sfRectangleShape_getSize(rects[j]).y;
            sfRectangleShape_setPosition(rects[j], pos);
            draw(window, rects);
            i++;
            j--;
        }
    }
}
void qsortt(sfRenderWindow *window, sfRectangleShape **rects, int inicio, int final){
    if(inicio < final){
        int p = particion(window, rects, inicio, final);
        qsortt(window, rects, inicio, p);
        qsortt(window, rects, p+1, final);
    }
}
void quicksort(sfRenderWindow *window, sfRectangleShape **rects, int n){
    qsortt(window, rects, 0, n-1);
}
typedef unsigned int uint;
int main()
{
    srand((uint)time(NULL));
    sfRectangleShape **rects;
    rects = malloc(sizeof (sfRectangleShape *) * n);
    sfVideoMode videomode;
    videomode.width = WIDTH;
    videomode.height = HEIGHT;
    struct timeval inicio, final;
    gettimeofday(&inicio, NULL);
    for(uint i = 0; i < n; i++){
        rects[i] = sfRectangleShape_create();
        sfVector2f pos;
        sfVector2f size;
        size.x = 5;
        size.y = rand() % HEIGHT;
        pos.x = i * (size.x + 0.5f);
        pos.y = HEIGHT - size.y;
        sfRectangleShape_setSize(rects[i], size);
        sfRectangleShape_setPosition(rects[i], pos);
        sfRectangleShape_setFillColor(rects[i], sfBlack);
    }
    sfRenderWindow *window = sfRenderWindow_create(videomode, "Quicksort", sfDefaultStyle, NULL);
    sfEvent event;
    sfVector2i pos;
    pos.x = (sfVideoMode_getDesktopMode().width-WIDTH)/2;
    pos.y = (sfVideoMode_getDesktopMode().height-HEIGHT)/2;
    sfRenderWindow_setPosition(window, pos);
    sfRenderWindow_setFramerateLimit(window, 60);
    quicksort(window, rects, n);
    gettimeofday(&final, NULL);
    long t = final.tv_sec - inicio.tv_sec;
    printf("Tiempo tardado en segundos -> %ld\n", t);
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            switch (event.type) {
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                    break;
                default:
                    break;
            }
        }
        draw(window, rects);
    }
    sfRenderWindow_destroy(window);
    for(uint i = 0; i < n; i++){
        sfRectangleShape_destroy(rects[i]);
    }
    free(rects);
    printf("-Completado-\n");
    return 0;
}
