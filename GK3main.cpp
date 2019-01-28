/*************************************************************************************/
 
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych
 
/*************************************************************************************/
 
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
 
 
typedef float point3[3];
const int N = 20;
point3 vertex[N][N];
//point3 color[N][N];
 
/*************************************************************************************/
 
// Funkcja rysująca osie układu współrzędnych
 
 
 
void Axes(void)
{
 
 point3  x_min = { -5.0, 0.0, 0.0 };
 point3  x_max = { 5.0, 0.0, 0.0 };
 // początek i koniec obrazu osi x
 
 point3  y_min = { 0.0, -5.0, 0.0 };
 point3  y_max = { 0.0,  5.0, 0.0 };
 // początek i koniec obrazu osi y
 
 point3  z_min = { 0.0, 0.0, -5.0 };
 point3  z_max = { 0.0, 0.0,  5.0 };
 //  początek i koniec obrazu osi y
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
 
 
 
 
void Egg()
{
 
 
// float step = 1 / (float)(N - 1);
 float u = 0;
 float v = 0;
 
 for (int i = 0; i < N; i++)
 {
 u = i / (float)(N - 1);
 for (int j = 0; j < N; j++)
 {
 v = j / (float)(N - 1);
 
 vertex[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(3.14*v);
 vertex[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
 vertex[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(3.14*v);
 
 }
 
 
 }
 
 glColor3f(0.0f, 1.0f, 1.0f);  // kolor rysowania - biały
 glTranslated(0, -6, 0); // przesuniecie do srodka osi wsp
 glScaled(1.5, 1.5, 1.5); // skalowanie w kazdej osi *1.5
 for(int i = 0; i < N; i++) { // rysowanie poziomych obreczy
 glBegin(GL_LINE_LOOP);
 for (int j = 0; j < N; j++) 
 glVertex3fv(vertex[i][j]);
 glEnd();
 }
 for (int i = 0; i < N; i++) {  //rysowanie przekatnych
 glBegin(GL_LINES);
 for (int j = 0; j < N - 1; j++) {
 glVertex3fv(vertex[i][j]);
 glVertex3fv(vertex[i+1][j]);
 }
 glEnd();
 }
/* for (int i = 0; i < N; i++) { // rysowanie poziomych obreczy
 glBegin(GL_LINES);
 for (int j = 0; j < N - 1; j++) {
 glVertex3fv(vertex[i][j]);
 glVertex3fv(vertex[i][j + 1]);
 }
 glEnd();*/
/* 
 glBegin(GL_POINTS); // rysowanie punktow
 for (int i = 0; i < N; i++)
 for (int j = 0; j < N; j++)
 glVertex3fv(vertex[i][j]);
 
 glEnd();*/
}
/*************************************************************************************/
 
// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba 
// przerysować scenę)
 
 
 
void RenderScene(void)
{
 
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 // Czyszczenie okna aktualnym kolorem czyszczącym
 
 glLoadIdentity();
 // Czyszczenie macierzy bieżącej
// glRotated(60.0, 1.0, 1.0, 1.0);
 Axes();
 // Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej
 glRotated(60.0, 1.0, 1.0, 1.0);
 Egg();
 
 glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały
 
// glRotated(60.0, 1.0, 1.0, 1.0);
// glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
 
 glFlush();
 // Przekazanie poleceń rysujących do wykonania
 
 
 glutSwapBuffers();
 //
}
 
/*************************************************************************************/
 
// Funkcja ustalająca stan renderowania
 
 
 
void MyInit(void)
{
 
 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 // Kolor czyszcący (wypełnienia okna) ustawiono na czarny
 
}
 
/*************************************************************************************/
 
// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.
 
 
 
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
 
 GLfloat AspectRatio;
 // Deklaracja zmiennej AspectRatio  określającej proporcję
 // wymiarów okna 
 if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
 vertical = 1;
 glViewport(0, 0, horizontal, vertical);
 // Ustawienie wielkościokna okna widoku (viewport)
 // W tym przypadku od (0,0) do (horizontal, vertical)  
 glMatrixMode(GL_PROJECTION);
 // Przełączenie macierzy bieżącej na macierz projekcji 
 glLoadIdentity();
 // Czyszcznie macierzy bieżącej            
 AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
 // Wyznaczenie współczynnika  proporcji okna
 // Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
 // przestrzeni ograniczającej pozwalającej zachować właściwe
 // proporcje rysowanego obiektu.
 // Do okreslenia przestrzeni ograniczjącej służy funkcja
 // glOrtho(...)            
 if (horizontal <= vertical)
 
 glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
 else
 
 glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
 glMatrixMode(GL_MODELVIEW);
 // Przełączenie macierzy bieżącej na macierz widoku modelu                                   
 
 glLoadIdentity();
 // Czyszcenie macierzy bieżącej
}
 
/*************************************************************************************/
 
// Główny punkt wejścia programu. Program działa w trybie konsoli
 
 
 
void main(void)
{
 
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
 glutInitWindowSize(300, 300);
 
 glutCreateWindow("jajko N=20");
 
 glutDisplayFunc(RenderScene);
 // Określenie, że funkcja RenderScene będzie funkcją zwrotną
 // (callback function).  Bedzie ona wywoływana za każdym razem 
 // gdy zajdzie potrzba przeryswania okna 
 glutReshapeFunc(ChangeSize);
 // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
 // zazmiany rozmiaru okna      
 MyInit();
 // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
 // inicjalizacje konieczne  przed przystąpieniem do renderowania
 glEnable(GL_DEPTH_TEST);
 // Włączenie mechanizmu usuwania powierzchni niewidocznych
 
 glutMainLoop();
 // Funkcja uruchamia szkielet biblioteki GLUT
}
 
/*************************************************************************************/
