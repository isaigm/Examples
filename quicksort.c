#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SFML/Graphics.h>
static const uint n = 60;
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
            pos.y  = 720 - sfRectangleShape_getSize(rects[i]).y;
            sfRectangleShape_setPosition(rects[i], pos);
            pos = sfRectangleShape_getPosition(rects[j]);
            pos.y  = 720 - sfRectangleShape_getSize(rects[j]).y;
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
    videomode.width = 1280;
    videomode.height = 720;
    for(uint i = 0; i < n; i++){
        rects[i] = sfRectangleShape_create();
        sfVector2f pos;
        sfVector2f size;
        size.x = 15;
        size.y = rand() % 720;
        pos.x = i * (size.x + 0.5f);
        pos.y = 720 - size.y;
        sfRectangleShape_setSize(rects[i], size);
        sfRectangleShape_setPosition(rects[i], pos);
        sfRectangleShape_setFillColor(rects[i], sfBlack);
    }
    sfRenderWindow *window = sfRenderWindow_create(videomode, "Quicksort", sfDefaultStyle, NULL);
    sfEvent event;
    bool run = true;
    sfVector2i pos;
    pos.x = (sfVideoMode_getDesktopMode().width-1280)/2;
    pos.y = (sfVideoMode_getDesktopMode().height-720)/2;
    sfRenderWindow_setPosition(window, pos);
    sfRenderWindow_setFramerateLimit(window, 60);
    clock_t inicio = clock();
    quicksort(window, rects, n);
    clock_t final = clock();
    double t = 0.0;
    t += (double) (final-inicio) / CLOCKS_PER_SEC;
    printf("Tiempo tardado en segundos -> %.6f\n", t);
    while (run) {
        while (sfRenderWindow_pollEvent(window, &event) == sfTrue) {
            switch (event.type) {
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                    sfRenderWindow_destroy(window);
                    run = false;
                    break;
                default:
                    break;
            }
        }
        draw(window, rects);
    }
    for(uint i = 0; i < n; i++){
        free(rects[i]);
    }
    free(rects);
    return 0;
}

