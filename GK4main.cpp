/*************************************************************************************/
 
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzędnych dla rzutowania perspektywicznego
 
/*************************************************************************************/
 
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
 
 
typedef float point3[3];
 
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja położenia obserwatora
 
static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat alfa = 0.0;   // kąt obrotu obiektu
static GLfloat R = 7.0; // promień
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
 
static GLint status2 = 0;
static GLint status = 0;       // stan klawiszy myszy 
    // 0 - nie naciśnięto żadnego klawisza
    // 1 - naciśnięty zostać lewy klawisz
 
static GLfloat k1 = 0.0;
static GLfloat k2 = 0.0;
 
static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
 
static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
   // i poprzednią kursora myszy 
 
static int y_pos_old = 0;       // poprzednia pozycja kursora myszy
 
static int delta_y = 0;        // różnica pomiędzy pozycją bieżącą
   // i poprzednią kursora myszy
 
/*************************************************************************************/
 
// Funkcja rysująca osie układu wspó?rz?dnych
 
 
 
void Axes(void)
{
 
 point3  x_min = { -5.0, 0.0, 0.0 };
 point3  x_max = { 5.0, 0.0, 0.0 };
 // początek i koniec obrazu osi x
 
 point3  y_min = { 0.0, -5.0, 0.0 };
 point3  y_max = { 0.0,  5.0, 0.0 };
 // pocz?tek i koniec obrazu osi y
 
 point3  z_min = { 0.0, 0.0, -5.0 };
 point3  z_max = { 0.0, 0.0,  5.0 };
 //  pocz?tek i koniec obrazu osi y
 glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
 glBegin(GL_LINES); // rysowanie osi x
 glVertex3fv(x_min);
 glVertex3fv(x_max);
 glEnd();
 
 glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
 glBegin(GL_LINES);  // rysowanie osi y
 
 glVertex3fv(y_min);
 glVertex3fv(y_max);
 glEnd();
 
 glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
 glBegin(GL_LINES); // rysowanie osi z
 
 glVertex3fv(z_min);
 glVertex3fv(z_max);
 glEnd();
 
}
 
/*************************************************************************************/
 
// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba 
// przerysować scenę)
 
 
 
/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych
 
void Mouse(int btn, int state, int x, int y)
{
 if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
 {
 y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora 
  // jako pozycji poprzedniej
 status2 = 1;          // wcięnięty został lewy klawisz myszy
 }
 else
 status2 = 0;          // nie został wcięnięty klawisz RMB
 
 if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
 {
 x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
  // jako pozycji poprzedniej
 y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora 
  // jako pozycji poprzedniej
 status = 1;          // wcięnięty został lewy klawisz myszy
 }
 else
 status = 0;          // nie został wcięnięty klawisz LMB
}
 
/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich 
// zmiennych globalnych
 
void Motion(GLsizei x, GLsizei y)
{
 if (status == 1)
 {
 delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
 
 x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie
 
 delta_y = y - y_pos_old;     // obliczenie różnicy położenia kursora myszy
 
 y_pos_old = y; // podstawienie bieżącego położenia jako poprzednie
 }
 
 if (status2 == 1)
 {
 delta_y = y - y_pos_old;     // obliczenie różnicy położenia kursora myszy
 
 y_pos_old = y; // podstawienie bieżącego położenia jako poprzednie
 }
 glutPostRedisplay();     // przerysowanie obrazu sceny
}
 
/*************************************************************************************/
 
 
 
 
void RenderScene(void)
{
 
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 // Czyszczenie okna aktualnym kolorem czyszczącym
 
 glLoadIdentity();
 // Czyszczenie macierzy bie??cej
 
 gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 // Zdefiniowanie położenia obserwatora
 Axes();
 // Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej
 
 if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
 {
 theta += delta_x * pix2angle;
 alfa += delta_y * pix2angle; // modyfikacja kąta obrotu o kat proporcjonalny
 }                                  // do różnicy położeń kursora myszy
 
 
 
 if (status2 == 1)                     // jeśli prawy klawisz myszy wcięnięty
 {
 R += delta_y * 0.05; // spowolnienie zmiany
 if (R < 6.2)
 R = 6.2;
 if (R > 25)
 R = 25;
 
 }
 
 viewer[0] = R * cos(theta*0.05) * cos(alfa*0.05);
 viewer[1] = R * sin(alfa*0.05);
 viewer[2] = R * sin(theta*0.05) * cos(alfa*0.05);
 
 
 
 std::cout <<" v0 " << viewer[0] << " v1 " << viewer[1] << " v2 " << viewer[2] << " R " << R << std::endl;
// glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
// glRotatef(alfa, 1.0, 0.0, 0.0);  //obrót obiektu o nowy kąt
// glScaled(R, R, R); // skalowanie w kazdej osi
 
 glColor3f(1.0f, 1.0f, 1.0f);
 // Ustawienie koloru rysowania na biały
 
 glutWireTeapot(3.0);
 // Narysowanie czajnika
 glFlush();
 // Przekazanie poleceń rysujących do wykonania
 glutSwapBuffers();
 
 
 
}
/*************************************************************************************/
 
// Funkcja ustalająca stan renderowania
 
 
 
void MyInit(void)
{
 
 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 // Kolor czyszczący (wypełnienia okna) ustawiono na czarny
 
}
 
/*************************************************************************************/
 
 
// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.
 
 
 
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
 pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
 
 glMatrixMode(GL_PROJECTION);
 // Przełączenie macierzy bieżącej na macierz projekcji
 
 glLoadIdentity();
 // Czyszcznie macierzy bieżącej 
 
 gluPerspective(70, 1.0, 1.0, 30.0);
 // Ustawienie parametrów dla rzutu perspektywicznego
 
 
 if (horizontal <= vertical)
 glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
 
 else
 glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
 // Ustawienie wielkości okna okna widoku (viewport) w zależności
 // relacji pomiędzy wysokością i szerokością okna
 
 glMatrixMode(GL_MODELVIEW);
 // Przełączenie macierzy bieżącej na macierz widoku modelu  
 
 glLoadIdentity();
 // Czyszczenie macierzy bieżącej 
 
}
 
/*************************************************************************************/
 
// Główny punkt wejścia programu. Program działa w trybie konsoli
 
 
 
void main(void)
{
 
 
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
 glutInitWindowSize(300, 300);
 
 glutCreateWindow("Rzutowanie perspektywiczne");
 
 glutMouseFunc(Mouse);
 // Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy
 
 glutMotionFunc(Motion);
 // Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy
 
 glutDisplayFunc(RenderScene);
 // Określenie, że funkcja RenderScene będzie funkcją zwrotną
 // (callback function).  Będzie ona wywoływana za każdym razem 
 // gdy zajdzie potrzeba przerysowania okna
 
 
 glutReshapeFunc(ChangeSize);
 // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
 // za zmiany rozmiaru okna                       
 
 
 MyInit();
 // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
 // inicjalizacje konieczne  przed przystąpieniem do renderowania
 glEnable(GL_DEPTH_TEST);
 // Włączenie mechanizmu usuwania niewidocznych elementów sceny
 
 glutMainLoop();
 // Funkcja uruchamia szkielet biblioteki GLUT
}
 
/*************************************************************************************/
