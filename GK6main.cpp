 //glEnable(NORMALIZE); // funkcja normalizująca wektory do długości '1'
/*************************************************************************************/
 
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi
//  układu współrzędnych dla rzutowania perspektywicznego
 
/*************************************************************************************/
 
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
 
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
 
 
typedef float point3[3];
const int N = 100;
point3 vertex[N][N];
 
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
 
/*************************************************************************************/
 // Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
 // FileName, alokuje pamięć i zwraca wskaźnik (pBits) do bufora w którym
 // umieszczone są dane.
 // Ponadto udostępnia szerokość (ImWidth), wysokość (ImHeight) obrazu
 // tekstury oraz dane opisujące format obrazu według specyfikacji OpenGL
 // (ImComponents) i (ImFormat).
 // Jest to bardzo uproszczona wersja funkcji wczytującej dane z pliku TGA.
 // Działa tylko dla obrazów wykorzystujących 8, 24, or 32 bitowy kolor.
 // Nie obsługuje plików w formacie TGA kodowanych z kompresją RLE.
/*************************************************************************************/
 
 
GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
 
 /*************************************************************************************/
 
 // Struktura dla nagłówka pliku  TGA
 
 
#pragma pack(1)
 typedef struct
 {
 GLbyte    idlength;
 GLbyte    colormaptype;
 GLbyte    datatypecode;
 unsigned short    colormapstart;
 unsigned short    colormaplength;
 unsigned char     colormapdepth;
 unsigned short    x_orgin;
 unsigned short    y_orgin;
 unsigned short    width;
 unsigned short    height;
 GLbyte    bitsperpixel;
 GLbyte    descriptor;
 }TGAHEADER;
#pragma pack(8)
 
 FILE *pFile;
 TGAHEADER tgaHeader;
 unsigned long lImageSize;
 short sDepth;
 GLbyte    *pbitsperpixel = NULL;
 
 
 /*************************************************************************************/
 
 // Wartości domyślne zwracane w przypadku błędu
 
 *ImWidth = 0;
 *ImHeight = 0;
 *ImFormat = GL_BGR_EXT;
 *ImComponents = GL_RGB8;
 
 pFile = fopen(FileName, "rb");
 if (pFile == NULL)
 return NULL;
 
 /*************************************************************************************/
 // Przeczytanie nagłówka pliku
 
 
 fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
 
 
 /*************************************************************************************/
 
 // Odczytanie szerokości, wysokości i głębi obrazu
 
 *ImWidth = tgaHeader.width;
 *ImHeight = tgaHeader.height;
 sDepth = tgaHeader.bitsperpixel / 8;
 
 
 /*************************************************************************************/
 // Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity)
 
 if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
 return NULL;
 
 /*************************************************************************************/
 
 // Obliczenie rozmiaru bufora w pamięci
 
 
 lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
 
 
 /*************************************************************************************/
 
 // Alokacja pamięci dla danych obrazu
 
 
 pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
 
 if (pbitsperpixel == NULL)
 return NULL;
 
 if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
 {
 free(pbitsperpixel);
 return NULL;
 }
 
 
 /*************************************************************************************/
 
 // Ustawienie formatu OpenGL
 
 
 switch (sDepth)
 
 {
 
 case 3:
 
 *ImFormat = GL_BGR_EXT;
 
 *ImComponents = GL_RGB8;
 
 break;
 
 case 4:
 
 *ImFormat = GL_BGRA_EXT;
 
 *ImComponents = GL_RGBA8;
 
 break;
 
 case 1:
 
 *ImFormat = GL_LUMINANCE;
 
 *ImComponents = GL_LUMINANCE8;
 
 break;
 
 };
 
 
 
 fclose(pFile);
 
 
 
 return pbitsperpixel;
 
}
 
/*************************************************************************************/
 
 
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
 
 
 
 std::cout << " v0 " << viewer[0] << " v1 " << viewer[1] << " v2 " << viewer[2] << " R " << R << std::endl;
 // glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
 // glRotatef(alfa, 1.0, 0.0, 0.0);  //obrót obiektu o nowy kąt
 // glScaled(R, R, R); // skalowanie w kazdej osi
 
 glColor3f(1.0f, 1.0f, 1.0f);
 // Ustawienie koloru rysowania na biały
 
 glBegin(GL_TRIANGLES);
 
 glNormal3f(1.0f, 1.0f, 1.0f);       // zmienna dodana do teksturowania
 glTexCoord2f(0.0f, 0.0f);           // zmienna dodana do teksturowania
 glVertex3f(-5.0f, -5.0f, 0.0f);     // wektory wierzcholkow trojkata
 
 glNormal3f(1.0f, 1.0f, 1.0f);
 glTexCoord2f(1.0f, 0.0f);
 glVertex3f(5.0f, -5.0f, 0.0f);
 
 glNormal3f(1.0f, 1.0f, 1.0f);
 glTexCoord2f(0.5f, 1.0f);
 glVertex3f(0.0f, 5.0f, 0.0f);
 
 glEnd();
 
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
 
 /*************************************************************************************/
 
//  Definicja materiału z jakiego zrobiony jest czajnik
//  i definicja źródła światła
 
/*************************************************************************************/
/*************************************************************************************/
 
// Zmienne dla obrazu tekstury
 
 
 
 GLbyte *pBytes;
 GLint ImWidth, ImHeight, ImComponents;
 GLenum ImFormat;
 
 
 // ..................................
 //       Pozostała część funkcji MyInit()
 
 // ..................................
 
/*************************************************************************************/
 
// Teksturowanie będzie prowadzone tyko po jednej stronie ściany
 
 glEnable(GL_CULL_FACE);
 
 
 /*************************************************************************************/
 
 //  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
 
 pBytes = LoadTGAImage("grafikajack.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);
 
 
 /*************************************************************************************/
 
   // Zdefiniowanie tekstury 2-D
 
 glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);
 
 /*************************************************************************************/
 
 // Zwolnienie pamięci
 
 free(pBytes);
 
 
 /*************************************************************************************/
 
 // Włączenie mechanizmu teksturowania
 
 glEnable(GL_TEXTURE_2D);
 
 /*************************************************************************************/
 
 // Ustalenie trybu teksturowania
 
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 
 /*************************************************************************************/
 
 // Określenie sposobu nakładania tekstur
 
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
 /*************************************************************************************/
 // Definicja materiału z jakiego zrobiony jest czajnik
 
 GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
 // współczynniki ka =[kar,kag,kab] dla światła otoczenia
 
 GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
 // współczynniki kd =[kdr,kdg,kdb] światła rozproszonego
 
 GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
 // współczynniki ks =[ksr,ksg,ksb] dla światła odbitego
 
 GLfloat mat_shininess = { 20.0 };
 // współczynnik n opisujący połysk powierzchni
 
/*************************************************************************************/
// Definicja źródła światła
 
 GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
 // położenie źródła
 
 
 GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
 // składowe intensywności świecenia źródła światła otoczenia
 // Ia = [Iar,Iag,Iab]
 
 GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
 // składowe intensywności świecenia źródła światła powodującego
 // odbicie dyfuzyjne Id = [Idr,Idg,Idb]
 
 GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
 // składowe intensywności świecenia źródła światła powodującego
 // odbicie kierunkowe Is = [Isr,Isg,Isb]
 
 GLfloat att_constant = { 1.0 };
 // składowa stała ds dla modelu zmian oświetlenia w funkcji
 // odległości od źródła
 
 GLfloat att_linear = { 0.05 };
 // składowa liniowa dl dla modelu zmian oświetlenia w funkcji
 // odległości od źródła
 
 GLfloat att_quadratic = { 0.001 };
 // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
 // odległości od źródła
 
/*************************************************************************************/
// Ustawienie parametrów materiału i źródła światła
 
/*************************************************************************************/
// Ustawienie patrametrów materiału
 
 
 glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
 glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
 glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
 glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
 
 /*************************************************************************************/
 // Ustawienie parametrów źródła
 
 glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 
 glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
 glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
 glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
 
 
 /*************************************************************************************/
 // Ustawienie opcji systemu oświetlania sceny
 
 glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
 glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
 glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
 glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora
 
/*************************************************************************************/
 
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
