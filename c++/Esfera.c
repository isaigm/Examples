#include <stdio.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
void display(void);
void keys(unsigned char key, int x, int y);
void reshape(int x, int y);
typedef struct Sphere{
  GLfloat posx, posy;
  GLfloat directionx;
  GLfloat directiony;
  GLfloat radius;
}Sphere;
Sphere sphere;
int main(int argc, char **argv)
{
    sphere.posx = 0;
    sphere.posy = 0;
    sphere.radius = 1;
    sphere.directionx = 1;
    sphere.directiony = 1;
    glutInit(&argc, argv);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Esfera!");
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
void display(){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(sphere.posx - 3, sphere.posy, -7.0);
    glColor3f((GLfloat)0.9, (GLfloat)0.3, (GLfloat)0.2);
    glScalef(1.0, 1.0, 1.0);
    glutSolidSphere(sphere.radius, 100, 100);
    sphere.posx += sphere.directionx * 0.006f;
    sphere.posy += sphere.directiony * 0.006f;
    if(sphere.posx > 6.5) sphere.directionx = -1;
    else if(sphere.posx < -1) sphere.directionx = 1;
    if(sphere.posy > 1.9) sphere.directiony = -1;
    else if(sphere.posy < -2) sphere.directiony = 1;
    glutPostRedisplay();
    glFlush();
}
void keys(unsigned char key, int x, int y){
    switch (key) {
    case 'd':
        sphere.posx += 0.1f;
        glutPostRedisplay();
        break;
    case 'a':
        sphere.posx -= 0.1f;
        glutPostRedisplay();
        break;
    case 'w':
        sphere.posy += 0.1f;
        glutPostRedisplay();
        break;
    case 's':
        sphere.posy -= 0.1f;
        glutPostRedisplay();
        break;
    case 'x':
        exit(0);

    }
}
void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(39.0, (GLdouble)x/(GLdouble)y, 0.6, 21.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);
}

