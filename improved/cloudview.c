#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "include/cloud.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_FPS 144

// variaveis globais do mal
struct cloud* cloud;
struct vector3* center;
GLdouble aspect;
GLdouble angle;
GLdouble rotation;
GLdouble cut;

// volume de visualizacao
void setViewParameters()
{
	// seta sistema de projecao
    glMatrixMode(GL_PROJECTION);

	// initializa matrizes
    glLoadIdentity();

	// Especifica a projecao perspectiva
    gluPerspective(angle, aspect, 0, 512);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);

	// init sistema de coordenadas do modelo
    glLoadIdentity();

	// Especifica posicao do observador e do alvo
    gluLookAt(center->x, center->y, center->z + 200,
              center->x, center->y, center->z,
              0, 1, 0);

    glRotatef(rotation, 0, 1, 0);
}

// desenha pontos formadores da nuvem
void drawCloud()
{
    // nuvens da superficie
    glColor3f(0.0f, 0.5f, 1.0f);
    glBegin(GL_POINTS);
    struct cloud* aux = cloud;
    while(aux != NULL) {
        if (vector3_distance(aux->point, center) <= cut) {
            glVertex3d(aux->point->x, aux->point->y, aux->point->z);
        }
        aux = aux->next;
    }
    glEnd();

    // centro da nuvem
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex3d(center->x, center->y, center->z);
    glEnd();
}

// Funcao callback chamada para fazer o desenho
void render()
{
    // limpa fila de cores
    glClear(GL_COLOR_BUFFER_BIT);

    // desenha nuvem
    drawCloud();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

// callback para resize da janela
void resize(GLsizei w, GLsizei h)
{
    // Para previnir uma divisao por zero
    if(h == 0)
        h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correcao de aspecto
	aspect = (GLfloat)w / (GLfloat)h;

    // roda as configuracoes
	setViewParameters();
}

void inputKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'z':
            cut -= 1.0f;
            break;
        case 'x':
            cut += 1.0f;
            break;
        default:
            break;
    }
}

void inputSpecialKeyboard(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            angle -= 1;
            break;
        case GLUT_KEY_DOWN:
            angle += 1;
            break;
        case GLUT_KEY_LEFT:
            rotation -= 1;
            break;
        case GLUT_KEY_RIGHT:
            rotation += 1;
            break;
        default:
            break;
    }
}

// chamado todo frame para rodar qualquer coisa que voce quiser
void update(int val)
{
    setViewParameters();
    glutPostRedisplay();

    glutTimerFunc(1000 / SCREEN_FPS, update, val);
}

// inicia parametros de rendering
void initParams()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    angle = 45;
    rotation = 0;
    cut = 70.0f;
}

// funcao principal
int main(int argc, char** argv)
{
    if(argc != 2) {
        printf("ERRO\nEsse utilitario aceita somente um unico parametro\n");
        printf("Uso: pcv <caminho_para_arquivo_nuvem>\n");
        exit(1);
    }

    cloud = cloud_load_from_file(argv[1]);
    center = cloud_get_center(cloud);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("cloudview by Artur Rodrigues V1.3");

	glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutKeyboardFunc(inputKeyboard);
    glutSpecialFunc(inputSpecialKeyboard);
    glutTimerFunc(1000 / SCREEN_FPS, update, 0);

    initParams();
	glutMainLoop();

    cloud_free(cloud);
	return 0;
}
