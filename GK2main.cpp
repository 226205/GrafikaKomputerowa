#include "pch.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>



GLfloat deform = 1;





void RenderScene(void)

{
    GLfloat deformation = (rand() % 15)*deform;



        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = -25; i <= 75; i = i + 50)
        {
            for (int j = -25; j <= 75; j = j + 50)
            {

                if (i == 25 && j == 25); else {

                    glBegin(GL_POLYGON);

                    glColor3f(rand() % 10 * 0.1, rand() % 10 * 0.1, rand() % 10 * 0.1);
                    glVertex2f(i - 50 + (rand() % 15)*deform, j - 50 + (rand() % 15)*deform);
                    glColor3f(rand() % 10 * 0.1, rand() % 10 * 0.1, rand() % 10 * 0.1);
                    glVertex2f(i + (rand() % 15)*deform, j - 50 + (rand() % 15)*deform);
                    glColor3f(rand() % 10 * 0.1, rand() % 10 * 0.1, rand() % 10 * 0.1);
                    glVertex2f(i + (rand() % 15)*deform, j + (rand() % 15)*deform);
                    glColor3f(rand() % 10 * 0.1, rand() % 10 * 0.1, rand() % 10 * 0.1);
                    glVertex2f(i - 50 + (rand() % 15)*deform, j + (rand() % 15)*deform);

                    glEnd();

                }
            }
        }

        glFlush();
    
}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania



void MyInit(void)

{

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // Kolor okna wnętrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokość i wysokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna urządzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)

    glMatrixMode(GL_PROJECTION);
    // Określenie układu współrzędnych obserwatora

    glLoadIdentity();
    // Określenie przestrzeni ograniczającej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie współczynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // określenie okna obserwatora.
    // Pozwala to zachować właściwe proporcje rysowanego obiektu
    // Do określenia okna obserwatora służy funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Określenie układu współrzędnych     

    glLoadIdentity();

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



void main(void)

{
    srand(time(NULL));

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    // Ustawienie trybu wyświetlania
    // GLUT_SINGLE - pojedynczy bufor wyświetlania
    // GLUT_RGBA - model kolorów RGB



    glutCreateWindow("Trzeci program w OpenGL");
    // Utworzenie okna i określenie treści napisu w nagłówku okna

    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
    // Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
    // trzeba będzie przerysować okno


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
    // zmiany rozmiaru okna

    MyInit();
    // Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie 
    // inicjalizacje konieczneprzed przystąpieniem do renderowania


    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/
