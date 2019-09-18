#include <stdio.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
void display(void);
void keys(unsigned char key, int x, int y);
void reshape(int x, int y);
GLfloat posx = 0, posy = 0;
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Hola, mundo!");
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
    glTranslatef(posx - 3, posy, -7.0);
    glColor3f((GLfloat)0.9, (GLfloat)0.3, (GLfloat)0.2);
    glScalef(1.0, 1.0, 1.0);
    glutSolidSphere(1, 100, 100);
    glFlush();
}
void keys(unsigned char key, int x, int y){
    switch (key) {
    case 'd':
        posx += 0.1f;
        glutPostRedisplay();
        break;
    case 'a':
        posx -= 0.1f;
        glutPostRedisplay();
        break;
    case 'w':
        posy += 0.1f;
        glutPostRedisplay();
        break;
    case 's':
        posy -= 0.1f;
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
