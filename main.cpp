
#include<GL/freeglut.h>
#include<stdio.h>
#include<iostream>
#include<math.h>


GLfloat escala = 1;
GLint janela, popup;
int jogador = 1;
int pos = 1;
int gameOver    = false;
int estadoClick = false;
int i, ortho = (0,6);
static int val = 0;
static int menuPrincipal;
static int subMenu;
//static int janela;
// Coodenadas globais X, Y e ângulo teta (T)
float X, Y, T = 0.0;


struct No
{
    int valor;
    struct No *proximo;
};
typedef struct No no;

no *lista = (no *) malloc(sizeof(no));
void iniciaLista(no *lista);
void exibeLista(no *lista);
void adicionaNo(no *lista);
int verificaVelha(no *lista);
int retornaValor(no *lista, int pos);
int verificaPos(no *lista, int pos, int jog);


void DesenhaTexto(void *font, char *string)
{
    // EXIBE CARACTERE POR CARACTERE
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*string++);
}

void DesenhaLinha(float iX, float iY, float fX, float fY)
{
    glBegin(GL_LINES);
        glColor3f(0,1,0);
        glLineWidth(1.0);
        glVertex2f(iX, iY);
        glVertex2f(fX, fY);
    glEnd();
}

void tabuleiro(void)
{
    glBegin(GL_LINES);
      glColor3f(0,0,0);
      // Linhas Horizontais
      glVertex2f(-4.5, -1.5);
      glVertex2f(+4.5, -1.5);
      glVertex2f(-4.5, +1.5);
      glVertex2f(+4.5, +1.5);
      // Linhas Verticais
      glVertex2f(-1.5, -4.5);
      glVertex2f(-1.5, +4.5);
      glVertex2f(+1.5, -4.5);
      glVertex2f(+1.5, +4.5);
    glEnd();
}

void escreveX(void)
{
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex2f(X-1,Y-1);
        glVertex2f(X,Y);
        glVertex2f(X+1,Y+1);
        glVertex2f(X,Y);
        glVertex2f(X+1,Y-1);
        glVertex2f(X,Y);
        glVertex2f(X-1,Y+1);
        glVertex2f(X,Y);
    glEnd();

   // estadoClick = false;
}

void escreveO(void)
{
    glBegin(GL_POINTS);
        glColor3f(0,0,1);
        //LOOP DE PONTOS PARA DESENHAR UM CIRCULO DE RAIO 1
        for (int p=0; p<360; p++)
        {
            // Calculando Teta
            T = p * 3.1416/180;
            glVertex2f(X + cos(T), Y + sin(T));
        }
    glEnd();
}

void apagaTexto(void)
{
    glColor3f(1,1,1);
    glRasterPos2f(-5,5);
    if(jogador==1)
        DesenhaTexto(GLUT_BITMAP_9_BY_15,"JOGADOR 2");
    else
        DesenhaTexto(GLUT_BITMAP_9_BY_15,"JOGADOR 1");
    glPopMatrix();
    glFlush();
}

//VERIFICA SE TRES POSICOES SÃO IGUAIS E NÃO-NULAS
int verificaGanhador(int posA, int posB, int posC)
{
    posA = retornaValor(lista, posA);
    posB = retornaValor(lista, posB);
    posC = retornaValor(lista, posC);

    if(posA == 0 || posB == 0 || posC == 0)
        return false;
    if(posA == posC && posB == posC)
      {
        printf("\nposA %d", posA); printf("\nposB %d", posB); printf("\nposC %d", posC);
        exibeLista(lista);
        return true;}
    else
        return false;
}

void desenha(void) {
    //glClear(GL_COLOR_BUFFER_BIT);
    if(gameOver)
        return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-ortho,ortho,-ortho,ortho);

    glScalef(escala, escala, 0);

    glMatrixMode(GL_MODELVIEW);

    //DESENHA TABULEIRO
    tabuleiro();


    printf("\nestadoClick --> %d\n", estadoClick);
    printf("\nJOGADOR %d\n", jogador);

    if (estadoClick)
    {
        if(jogador==1 && verificaPos(lista, pos, jogador))
        {
            escreveX();
            apagaTexto();

            //ESCREVER TEXTO NA TELA
            glColor3f(1,0,0);
            glRasterPos2f(-5,5);
            DesenhaTexto(GLUT_BITMAP_9_BY_15,"JOGADOR 1" );
            glPopMatrix();

            jogador++;
        }

        if(jogador==2 && verificaPos(lista, pos, jogador))
        {
            escreveO();
            apagaTexto();

            //ESCREVER TEXTO NA TELA
            glColor3f(0,0,1);
            glRasterPos2f(-5,5);
            DesenhaTexto(GLUT_BITMAP_9_BY_15,"JOGADOR 2" );
            glPopMatrix();

            jogador--;
        }
        estadoClick = false;

        //VERIFICA VENCEDOR LINHA 1
        if(verificaGanhador(1, 2, 3))
        {
            gameOver = true;
            DesenhaLinha(-5,3,5,3);
        }
        //VERIFICA VENCEDOR LINHA 2
        if(verificaGanhador(4, 5, 6))
        {
            gameOver = true;
            DesenhaLinha(-5,0,5,0);
        }
        //VERIFICA VENCEDOR LINHA 3
        if(verificaGanhador(7, 8, 9))
        {
            gameOver = true;
            DesenhaLinha(-5,-3,5,-3);
        }
        //VERIFICA VENCEDOR COLUNA 1
        if(verificaGanhador(1, 4, 7))
        {
            gameOver = true;
            DesenhaLinha(-3,5,-3,-5);
        }
        //VERIFICA VENCEDOR COLUNA 2
        if(verificaGanhador(2, 5, 8))
        {
            gameOver = true;
            DesenhaLinha(0,-5,0,5);
        }
        //VERIFICA VENCEDOR COLUNA 3
        if(verificaGanhador(3, 6, 9))
        {
            gameOver = true;
            DesenhaLinha(3,-5,3,5);
        }
        //VERIFICA VENCEDOR DIAGONAL 1
        if(verificaGanhador(1, 5, 9))
        {
            gameOver = true;
            DesenhaLinha(-4,4,4,-4);
        }
        //VERIFICA VENCEDOR DIAGONAL 2
        if(verificaGanhador(3, 5, 7))
        {
            gameOver = true;
            DesenhaLinha(4,4,-4,-4);
        }
        if(verificaVelha(lista) && !gameOver)
        {
            gameOver = true;
            //ESCREVER TEXTO NA 5
            glColor3f(1,0,0);
            glRasterPos2f(-1,-5);
            DesenhaTexto(GLUT_BITMAP_9_BY_15,"DEU VELHA !!!");
            glPopMatrix();
        }
    }
    glutSwapBuffers();
    //glFlush();
}

// INTERAÇÃO COM MOUSE
void mouse(int button, int state, int mouseX, int mouseY)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        estadoClick = true;
        printf("\nX --> %d", mouseX);
        printf("\nY --> %d\n", mouseY);

        //VERIFICA LINHA 1
        if(mouseY < 225)
        {
            // VERIFICA COLUNA 1
            if (mouseX < 299)
            {
                pos = 1;
                X   = -3.0;
                Y   = 3.0;

            }
            // VERIFICA COLUNA 2
            if (mouseX >= 299 && mouseX <= 499)
            {
                pos = 2;
                X   = 0.0;
                Y   = 3.0;
            }
            // VERIFICA COLUNA 3
            if(mouseX > 499)
            {
                pos = 3;
                X   = 3.0;
                Y   = 3.0;
            }
        }
        // VERIFICA LINHA 2
        if (mouseY>=225 && mouseY<=375)
        {
             // VERIFICA COLUNA 1
            if (mouseX < 299)
            {
                pos = 4;
                X   = -3.0;
                Y   = 0.0;
            }
            // VERIFICA COLUNA 2
            if (mouseX >= 299 && mouseX <= 499)
            {
                pos = 5;
                X   = 0.0;
                Y   = 0.0;
            }
            // VERIFICA COLUNA 3
            if(mouseX > 499)
            {
                pos = 6;
                X   = 3.0;
                Y   = 0.0;
            }
        }
        //VERIFICA LINHA 3
        if(mouseY > 375)
        {
            // VERIFICA COLUNA 1
            if (mouseX < 299)
            {
                pos = 7;
                X   = -3.0;
                Y   = -3.0;
            }
            // VERIFICA COLUNA 2
            if (mouseX >= 299 && mouseX <= 499)
            {
                pos = 8;
                X   = 0.0;
                Y   = -3.0;
            }
            // VERIFICA COLUNA 3
            if(mouseX > 499)
            {
                pos = 9;
                  X = 3.0;
                  Y = -3.0;
            }
        }

    }
    glutPostRedisplay();
}

void displayAviso(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-ortho,ortho,-ortho,ortho);

    glColor3f(1,0,0);
    glRasterPos2f(-5,2);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"PUC MINAS - CIENCIA DA COMPUTACAO");
    glRasterPos2f(-5,0);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"MODELAGEM 3D - PROF WILL RICARDO");
    glRasterPos2f(-5,-2);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"AUTOR: YURY REGIS");
    glPopMatrix();
    glFlush();
}

void popUp(void)
{
    glutInitWindowSize(400,200);
    glutInitWindowPosition(500,200);
    popup = glutCreateWindow("JOGO DA VELHA v0.1");
    //popup = glutCreateSubWindow(janela, 10, 10, 500,200);
    glClearColor(1,1,1,0);
    glutDisplayFunc(displayAviso);
}

void menu(int value)
{
    switch (value)
    {
        case 0:
            glutDestroyWindow(janela);
            exit(0);
        case 1:
            printf("\n\nAUTOR --> HARRY\n\n");
            popUp();
    }
    val = value;
    glutPostRedisplay();
}

//CRIANDO MENU
void criaMenu(void)
{
    subMenu = glutCreateMenu(menu);

    //ENTRADAS DOS SUBMENUS PRINCIPAL
    glutAddMenuEntry("AUTOR", 1);

    menuPrincipal = glutCreateMenu(menu);

    //ASSOCIA SUBMENU AO PRINCIPAL
    glutAddSubMenu("JOGO", subMenu);
    glutAddMenuEntry("SAIR",0);

    //PERMITIR INTERAÇÃO DO MENU COM MOUSE
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    printf("INICIO DO PROGRAMA\n\n");

    iniciaLista(lista);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(300,100);
    janela = glutCreateWindow("JOGO DA VELHA");
    criaMenu();
    glutDisplayFunc(desenha);
    glutMouseFunc(mouse);
    glClearColor(1,1,1,0);
    glutMainLoop();
    return 0;
}


void iniciaLista(no *lista)
{
    lista->valor = 0;
    lista->proximo = NULL;

    for(int c=1; c<9; c++)
    {
        adicionaNo(lista);
    }
}

void adicionaNo(no *lista)
{
    no *novoNo = (no *) malloc(sizeof(no));
    no *noTemp = lista->proximo;

    novoNo->valor = 0;
    novoNo->proximo = NULL;

    if (lista->proximo == NULL)
    {
        lista->proximo = novoNo;
        return;
    }
    while(noTemp->proximo != NULL)
        noTemp = noTemp->proximo;
    noTemp->proximo = novoNo;
}

//VERIFICA SE TODOS OS VALORES SÃO DIFERENTE DE ZERO
int verificaVelha(no *lista)
{
    if (lista->valor == 0)
        return false;

    no *aux = lista->proximo;
    while(aux!=NULL)
    {
         if (aux->valor == 0)
         {
             free(aux);
             return false;
         }
         aux = aux->proximo;
    }
    free(aux);
    return true;
}

//RETORNA VALOR POR POSIÇÃO
int retornaValor(no *lista, int pos)
{
    if(pos==1)
        return lista->valor;

    no *anterior = lista;
    no *atual    = lista->proximo;
    no *noTemp   = (no *) malloc(sizeof(no));

    for (int cont=1; cont<pos; cont++)
    {
        anterior = atual;
        atual    = atual->proximo;
    }
    return anterior->valor;
}

//VERIFICA SE JA TEMOS REGISTRO DE ALGUM JOGADOR NESSA POSIÇÃO
int verificaPos(no *lista, int pos, int jog)
{
    int cont = 1;
    no *noTemp = lista->proximo;

    //VERIFICA PRIMEIRO NO
    if(pos == cont)
    {
        if(lista->valor != 0)
            return false;
        else
            {
                lista->valor = jog;
                return true;
            }
    }
    //VERIFICA NOS INTERMEDIARIOS
    while(noTemp->proximo != NULL)
    {
        cont++;
        if(pos == cont)
        {
            if (noTemp->valor != 0)
                return false;
            else
            {
                noTemp->valor = jog;
                return true;
            }

        }
        noTemp = noTemp->proximo;
    }
    //VERIFICA ULTIMO NO
    cont++;
    if (cont == pos)
    {
        if(noTemp->valor!=0)
            return false;
        else
            {
                noTemp->valor = jog;
                return true;
            }
    }
}


void exibeLista(no *lista)
{
    no *noTemp;
    int cont = 1;

    noTemp = lista->proximo;
    printf("\n\nTABELA DO JOGO\n");
    printf(" %d", lista->valor);

    while(noTemp!=NULL)
    {
         printf(" %d", noTemp->valor);
         noTemp = noTemp->proximo;
         cont++;
         if(cont%3==0)
            printf("\n");
    }
    printf("\n");
    free(noTemp);
}
