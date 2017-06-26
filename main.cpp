/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG(ma³peczka)enter.net.pl
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

// stale do obslugi menu podrecznego
enum
{
    FULL_WINDOW, // aspekt obrazu - cale okno
    ASPECT_1_1, // aspekt obrazu 1:1
    EXIT // wyjscie
};

// aspekt obrazu
int Aspect = FULL_WINDOW;

// dlugosc kroku
GLdouble step = 0.1;

// odleglosc punktu od obserwatora / zakres
GLdouble range = 10;

// wpolrzedne polozenia obserwatora
GLdouble eyex = 19;
GLdouble eyey = 3;
GLdouble eyez = 21;

// wspolrzedne punktu w ktorego kierunku jest zwrocony obserwator,
GLdouble centerx = eyex;
GLdouble centery = eyey;
GLdouble centerz = eyez-range;

// kierunki patrzenia lwo/prawo gora/dol
float lewoprawo = 0.0f;
float goradol = 0.0f;

// labirynt
int tab_old[8][7] = {
    {1,0,1,1,1,1,1},
    {1,0,1,0,0,0,1},
    {1,0,1,1,1,0,1},
    {1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1},
    {1,0,0,0,1,0,0},
    {1,1,1,1,1,1,1}
};

// tablica z labiryntem
int tab [21][21] = {
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // 1
    {1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1}, // 2
    {1,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1}, // 3
    {1,0,1,1,0,1,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1}, // 4
    {1,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,0,1}, // 5
    {1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1}, // 6
    {1,0,1,0,0,1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,1}, // 7
    {1,0,0,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,1,0,1}, // 8
    {1,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,0,1,0,0,1}, // 9
    {1,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1}, // 10
    {1,0,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,0,1}, // 11
    {1,0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,1}, // 12
    {1,0,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1}, // 13
    {1,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,0,1,0,1,1}, // 14
    {1,1,0,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,1}, // 15
    {1,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,0,1}, // 16
    {1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1}, // 17
    {1,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,1}, // 18
    {1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1}, // 19
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1}, // 20
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1}, // 21
};

// okiekt ktory sluzy za sciane
void obiekt()
{
    //glutWireCube(1.0);
    glutSolidCube(1.0);
}

// funkcja generujaca labirynt z tablicy
void labirynt(int tab[21][21])
{
    for(int y=0; y<21; y++){
        for(int x=0; x<21; x++){
            if(tab[y][x] == 1) {
                //glutWireCube( 1.0 );
                obiekt();
            }
            glTranslatef( 1.0, 0.0, 0.0 );
        }
        glTranslatef( -21.0, 0.0, 1.0 );
    }
}

// generowanie podlogi
void floorRender()
{
    for(int y=0; y<21; y++){
        for(int x=0; x<21; x++){
            obiekt();
            glTranslatef( 1.0, 0.0, 0.0 );
        }
        glTranslatef( -21.0, 0.0, 1.0 );
    }
}


// funkcja generujaca scene 3D
void Display()
{
    // kolor tla - zawartosc bufora koloru
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // czyszczenie bufora koloru
    glClear(GL_COLOR_BUFFER_BIT);

    // wybor macierzy modelowania
    glMatrixMode(GL_MODELVIEW);

    // macierz modelowania = macierz jednostkowa
    glLoadIdentity();

    // ustawienie obserwatora
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);

    //kolor podlogi + rysowanie podlogi
    glColor3f(0.0, 0.0, 0.0);
    floorRender();

    // kolor lairyntu + koraekta + rysowanie labiryntu
    glColor3f(0.2, 0.8, 0.2);
    glTranslatef( 0.0, 1.0, -21.0 );
    labirynt(tab);

    glTranslatef( 0.0, 1.0, -21.0 );
    labirynt(tab);

    glTranslatef( 0.0, 1.0, -21.0 );
    labirynt(tab);

    glTranslatef( 0.0, 1.0, -21.0 );
    glColor3f(0.0, 0.0, 0.0);
    floorRender();

    // skierowanie poleceñ do wykonania
    glFlush();

    // zamiana buforów koloru
    glutSwapBuffers();
}

// zmiana wielkosci okna
void Reshape( int width, int height )
{
    // obszar renderingu - cale okno
    glViewport( 0, 0, width, height );

    // wybor macierzy rzutowania
    glMatrixMode( GL_PROJECTION );

    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();

    // parametry bryly obcinania
    /*
    if( Aspect == ASPECT_1_1 )
    {
        // wysokoœæ okna wiêksza od wysokoœci okna
        if( width < height && width > 0 )
             glFrustum( - 2.0, 2.0, - 2.0 * height / width, 2.0 * height / width, 1.0, 5.0 );
        else

        // szerokoœæ okna wiêksza lub równa wysokoœci okna
        if( width >= height && height > 0 )
             glFrustum( - 2.0 * width / height, 2.0 * width / height, - 2.0, 2.0, 1.0, 5.0 );

    }
    else
         glFrustum( - 2.0, 2.0, - 2.0, 2.0, 1.0, 5.0 );
    */

    glFrustum(
              -5.0  * width / height,
              5.0 * width / height,
              -5.0, 5.0,
              0.6, 10.0
              );

    // generowanie sceny 3D
    Display();
}

// wypisz wspolzedne
void writePosition()
{
    cout<<"X: "<<eyex<<"\tX:"<<centerx<<endl;
    cout<<"Y: "<<eyey<<"\tY:"<<centery<<endl;
    cout<<"Z: "<<eyez<<"\tZ:"<<centerz<<endl;
    //cout<<"Z CEIL: "<<ceil(eyez)<<endl;
    cout<<"----------------\n";

    /*
    cout<<"X: "<<eyex<<endl;
    cout<<"X CEIL: "<<ceil(eyex)<<endl;
    cout<<"X FLOR: "<<floor(eyex)<<endl;
    cout<<"X ROUND: "<<round(eyex)<<endl;
    cout<<"----------------\n";


    double x_plus = eyex + 0.5;
    cout<<"X PLUS: "<<x_plus<<endl;
    cout<<"X PLUS CEIL: "<<ceil(x_plus)<<endl;
    cout<<"X PLUS FLOR: "<<floor(x_plus)<<endl;
    cout<<"X PLUS ROUND: "<<round(x_plus)<<endl;
    cout<<"----------------\n";

    double x_minus = eyex - 0.5;
    cout<<"X MINUS: "<<x_minus<<endl;
    cout<<"X MINUS CEIL: "<<ceil(x_minus)<<endl;
    cout<<"X MINUS FLOR: "<<floor(x_minus)<<endl;
    cout<<"X MINUS ROUND: "<<round(x_minus)<<endl;
    cout<<"----------------\n";





    bool wall = false;

    // sprawdz czy to nie sciana os X
    int x1 = ceil(eyex - 0.5);
    int x2 = ceil(eyex - 0.6);
    int z1 = ceil(eyez - 0.5);
    int z2 = ceil(eyez - 0.6);
    if (
        tab[z1][x1] == 1 ||
        tab[z1][x2] == 1 ||
        tab[z2][x1] == 1 ||
        tab[z2][x2] == 1
    ) {
        wall = true;
    }

    /*
    int c = ceil(eyez - 0.5);
    int d = ceil(eyez - 0.6);
    int x = ceil(eyex);
    if (tab[c][x] == 1 || tab[d][x] == 1) {
        wall = true;
    }


    if (wall) {
        cout<<"WALL!"<<endl;
    } else {
        cout<<"OK!"<<endl;
    }

    cout<<"----------------\n\n\n";

    */

}

// funkcja sprawdza czy pozycja podana w parametrze jest sciana
bool isWall(double x = 0.0, double z = 0.0)
{
    int x1 = ceil(x - 0.5);
    int x2 = ceil(x - 0.6);
    int z1 = ceil(z - 0.5);
    int z2 = ceil(z - 0.6);
    if (
        tab[z1][x1] == 1 ||
        tab[z1][x2] == 1 ||
        tab[z2][x1] == 1 ||
        tab[z2][x2] == 1
    ) {
        return true;
    }
    return false;
}

// funkcja wykonywana po wykryciu sciany
void afterWallDetect()
{
    writePosition();
    std::cout<<"WALL!\n";
}

// obserwator krok naprzod
void observerStepForward()
{
    bool wall = isWall(eyex, eyez-step);
    if (wall == false) {
        eyez -= step;
        centerz -= step;
        writePosition();
    } else {
        afterWallDetect();
    }
}

// obserwator krok wstecz
void observerStepBack()
{
    bool wall = isWall(eyex, eyez+step);
    if (wall == false) {
        eyez += step;
        centerz += step;
        writePosition();
    } else {
        afterWallDetect();
    }
}

// obserwator krok w lewo
void observerStepLeft()
{
    bool wall = isWall(eyex-step, eyez);
    if (wall == false) {
        eyex -= step;
        centerx -= step;
        writePosition();
    } else {
        afterWallDetect();
    }
}

// obserwator krok w prawo
void observerStepRight()
{
    bool wall = isWall(eyex+step, eyez);
    if (wall == false) {
        eyex += step;
        centerx += step;
        writePosition();
    } else {
        afterWallDetect();
    }
}

// obserwator krok w gore
void observerStepUp()
{
    eyey += 0.1;
    //centery += 0.1;
    writePosition();
}

// obserwator krok w dol
void observerStepDown()
{
    eyey -= 0.1;
    //centery -= 0.1;
    writePosition();
}


// obsluga klawiatury
void Keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
    // wyjscie
    case 27:
        exit(0);
        break;

    // krok naprzod
    case 'w':
    case 'W':
        observerStepForward();
        break;

    // krok wstecz
    case 's':
    case 'S':
        observerStepBack();
        break;

    // krok w lewo
    case 'a':
    case 'A':
        observerStepLeft();
        break;

    // krok w prawo
    case 'd':
    case 'D':
        observerStepRight();
        break;

    // krok w gore
    case 'e':
    case 'E':
    case '+':
        observerStepUp();
        break;

    // krok w dol
    case 'q':
    case 'Q':
    case '-':
        observerStepDown();
        break;
    }

    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

// obliczenia do obrotu kamery
void obliczenia()
{
    const float rad = M_PI / 180.0f;
    centerx = sin(lewoprawo * rad) * cos(goradol * rad);
    centery = -sin(goradol * rad);
    centerz = -cos(lewoprawo * rad) * cos(goradol * rad);
}

// spojrz w gore
void lookUp()
{
    if (goradol <= 85.0) {
        goradol += 1;
        obliczenia();
    }
}

// spojrz w dol
void lookDown()
{
    if (goradol >= -85.0) {
        goradol -= 1;
        obliczenia();
    }
}

// spojrz w lewo
void lookLeft()
{
    lewoprawo -= 1;
    obliczenia();
}

// spojrz w prawo
void lookRight()
{
    lewoprawo += 1;
    obliczenia();
}


// obsluga klawiszy funkcyjnych i klawiszy kursora
void SpecialKeys( int key, int x, int y )
{
    switch( key )
    {
    // patrz w gore
    case GLUT_KEY_UP:
        lookUp();
        break;

    // patrz w dol
    case GLUT_KEY_DOWN:
        lookDown();
        break;

    // patrz w lewo
    case GLUT_KEY_LEFT:
        lookLeft();
        break;

    // patrz w prawo
    case GLUT_KEY_RIGHT:
        lookRight();
        break;
    }

    // odrysowanie okna
    Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

// obsluga menu podrecznego

void Menu( int value )
{
    switch( value )
    {
        // obszar renderingu - cale okno
    case FULL_WINDOW:
        Aspect = FULL_WINDOW;
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
        break;

        // obszar renderingu - aspekt 1:1
    case ASPECT_1_1:
        Aspect = ASPECT_1_1;
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
        break;

        // wyjœcie
    case EXIT:
        exit( 0 );
    }
}

int main( int argc, char * argv[] )
{
    // inicjalizacja biblioteki GLUT
    glutInit( & argc, argv );

    // inicjalizacja bufora ramki
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );

    // rozmiary glownego okna programu
    glutInitWindowSize( 800, 800 );

    // utworzenie glownego okna programu

    glutCreateWindow( "Labirynt 3D" );

    // dolaczenie funkcji generujacej scene 3D
    glutDisplayFunc( Display );

    // dolaczenie funkcji wywolywanej przy zmianie rozmiaru okna
    glutReshapeFunc( Reshape );

    // dolaczenie funkcji obslugi klawiatury
    glutKeyboardFunc( Keyboard );

    // dolaczenie funkcji obslugi klawiszy funkcyjnych i klawiszy kursora
    glutSpecialFunc( SpecialKeys );

    // utworzenie menu podrecznego
    glutCreateMenu( Menu );

    // dodanie pozycji do menu podrecznego
    #ifdef WIN32

    glutAddMenuEntry( "Aspekt obrazu - ca³e okno", FULL_WINDOW );
    glutAddMenuEntry( "Aspekt obrazu 1:1", ASPECT_1_1 );
    glutAddMenuEntry( "Wyjœcie", EXIT );
    #else

    glutAddMenuEntry( "Aspekt obrazu - cale okno", FULL_WINDOW );
    glutAddMenuEntry( "Aspekt obrazu 1:1", ASPECT_1_1 );
    glutAddMenuEntry( "Wyjscie", EXIT );
    #endif

    // okreslenie przycisku myszki obsugujacej menu podrêczne
    glutAttachMenu( GLUT_RIGHT_BUTTON );

    // wprowadzenie programu do obslugi petli komunikatow
    glutMainLoop();
    return 0;
}
