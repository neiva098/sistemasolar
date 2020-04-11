#include <iostream>
#include <fstream>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include<time.h>
#include <string.h>
#include <SDL2/SDL.h>
#define PI 3.14
#define FILE_PATH "/home/sinval/Desktopp/sistema/Musica.wav" //diretorio da musica
using namespace std;
static float d = 1.0;           // Intensidade da cor difusa da luz branca
static float e = 1.0;           // Intensidade da cor especular da luz branca
static float m = 0.2;           // Intensidade da luz ambiente global          // A luz branca é posicional?
static float s = 50.0;          // Expoente especular do material (shininess)
float matShine[] = { s };                       // expoente especular (shininess)   // Fonte usada para imprimir na tela              // String buffer
static float xMouse = 250, yMouse = 250;        // (x,y) do ponteiro do mouse
static float larguraJanela, alturaJanela;       // (w,h) da janela
static bool isLightingOn = false;               // O sistema de iluminação está ligado?
static float anguloEsferaY = 0;                 // Rotação da esfera em torno do eixo y
static int esferaLados = 200;                   // Quantas subdivisões latitudinais/longitudinais da esfera
static bool localViewer = false;
static float anguloEsferaX = 0;
static float anguloCubo = 0;
static int marsTexture;
static int earth;
static int mercurio;
static int venus;
static int jupiter;
static int saturno;
static int urano;
static int netuno;
static int sol;
static int lua;
int nave;
static int mostrarLinhas=1;
int c=0;
int w=-1;
int vaux[9];
struct AudioData
{
	Uint8* pos;
	Uint32 length;
};
struct planet{
    double x,y,z,r;
    int* txt;
};
int i=1;
int zoom=0;
planet v[9];
void MyAudioCallback(void* userdata, Uint8* stream, int streamLength)
{
	AudioData* audio = (AudioData*)userdata;

	if(audio->length == 0)
	{
		return;
}

	Uint32 length = (Uint32)streamLength;
	length = (length > audio->length ? audio->length : length);

	SDL_memcpy(stream, audio->pos, length);

	audio->pos += length;
	audio->length -= length;
}
void escreveTexto(void * font, char *s, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);                      //Funcao para escrever texto
    }
}
void cadastrar(planet *x,double raio,double px,double py,double pz,int*textura)
{
    (*x).r=raio;
    (*x).x=px;
    (*x).y=py;
    (*x).z=pz;
    (*x).txt=textura;
}

void setup(void)
{
    //glClearColor(.4,.4,.4, 0.0);                    // fundo cinza
    glClearColor(0,0,0, 0.0);
    glEnable(GL_DEPTH_TEST);                        // Ativa teste Z

    // Propriedades do material da esfera
    float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};    // cor ambiente e difusa: branca
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };       // cor especular: branca

    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Carrega a textura
    marsTexture = SOIL_load_OGL_texture
	(
		"mars-small.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(marsTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
    earth = SOIL_load_OGL_texture
	(
		"earth.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(earth == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       mercurio = SOIL_load_OGL_texture
	(
		"mercurio.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(mercurio == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       venus = SOIL_load_OGL_texture
	(
		"venus.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(venus == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       urano = SOIL_load_OGL_texture
	(
		"netuno.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(urano == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       netuno = SOIL_load_OGL_texture
	(
		"netuno.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(netuno == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       saturno = SOIL_load_OGL_texture
	(
		"saturno.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(saturno == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       jupiter = SOIL_load_OGL_texture
	(
		"jupiter.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(jupiter == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
      sol = SOIL_load_OGL_texture
	(
		"sol.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(sol == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       lua = SOIL_load_OGL_texture
	(
		"lua.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(lua == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
       nave = SOIL_load_OGL_texture
	(
		"nave.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(nave == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }
    // Não mostrar faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glutSetCursor(GLUT_CURSOR_NONE);
}
void specialKeyInput(int key, int x, int y)
{
    if(key == GLUT_KEY_DOWN)
    {
        zoom-=1;
    }
    if(key == GLUT_KEY_UP)
    {
        zoom+=1;
    }
    glutPostRedisplay();
}

void solidSphere(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
}
void planeta(double r,double x,double y,double z,int* txt,int h,int aux)
{
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *txt);
        glPushMatrix();
        glRotatef(anguloEsferaY+aux, 0, 1, 0);
        glTranslatef(x,y, z);
        glRotatef(anguloEsferaY,1, 0,1);
        solidSphere(r, esferaLados, esferaLados);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
}
void luas(int ix,int aux)
{
    int d=5*(v[ix].r)/4;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,lua);

    glPushMatrix();


    glTranslatef((v[ix].x)*sin(1.5708+((anguloEsferaY+aux)/57.2958)),v[ix].y, (v[ix].x)*cos(1.5708+((anguloEsferaY+aux)/57.2958)));
    glRotatef(anguloEsferaX, 0, 1, 0);
    glTranslatef(d,0,d);
    glRotatef(anguloEsferaX, 0, 1, 0);


    solidSphere((v[ix].r)/4, esferaLados, esferaLados);
    glPopMatrix();
        glDisable(GL_TEXTURE_2D);
}
void satelite(int ix,int aux,int aleatorio){
    int d=(v[ix].r)+0.5;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,nave);
    glPushMatrix();
    glTranslatef((v[ix].x)*sin(1.5708+((anguloEsferaY+aux)/57.2958)),v[ix].y, (v[ix].x)*cos(1.5708+((anguloEsferaY+aux)/57.2958)));
    glRotatef(anguloCubo+aleatorio, 0, 1, 0);
    glTranslatef(d,0,d);
    glRotatef(anguloEsferaX, 0, 1, 0);
    glutSolidCube(0.5);
    glPopMatrix();
    glPushMatrix();
    glTranslatef((v[ix].x)*sin(1.5708+((anguloEsferaY+aux)/57.2958)),v[ix].y, (v[ix].x)*cos(1.5708+((anguloEsferaY+aux)/57.2958)));
    glRotatef(anguloCubo+aleatorio, 0, 1, 0);
    glTranslatef(d,0,d);
    glRotatef(anguloEsferaX, 0, 1, 0);
    solidSphere(1, 2, 2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
int* txt[10]={&sol,&mercurio,&venus,&earth,&marsTexture,&jupiter,&saturno,&urano,&netuno,&lua};
void camera(int w)
{
    if(w>=0)
    {
        gluLookAt(((v[w].x)*sin(1.57+(anguloEsferaY+vaux[w])/57.2958))+20,20 , ((v[w].x)*cos(1.57+(anguloEsferaY+vaux[w])/57.2958))+20,
        ((v[w].x)*sin(1.57+(anguloEsferaY+vaux[w])/57.2958)), 0,((v[w].x)*cos(1.57+(anguloEsferaY+vaux[w])/57.2958)),
        0, 1, 0);//57...e conversao graus radiano
    }
    else
        gluLookAt(17+2*(xMouse-larguraJanela/2)/(larguraJanela/16)+30, -6*(yMouse-alturaJanela/2)/(alturaJanela/16) +30, 5,
        0, 0, 0,
        0, 1, 0);
}
void camerac(int c)
{
    if(c==0)
        gluLookAt(0,800, 0,
        0, 0,0,
        1, 0, 0);
    if(c==1)
        gluLookAt(50,15 ,50,
        ((v[w].x)*sin(1.57+(anguloEsferaY+vaux[w])/57.2958)), v[w].y,((v[w].x)*cos(1.57+(anguloEsferaY+vaux[w])/57.2958)),
        0, 1, 0);

    if(c==2)
        gluLookAt(((v[w].x)*sin(1.57+(anguloEsferaY+vaux[w])/57.2958)),v[w].y+v[w].r, ((v[w].x)*cos(1.57+(anguloEsferaY+vaux[w]+zoom)/57.2958)),
        0, 0,0,
        0, 1, 0);
    if(c==3)
        camera(w);
}
// Callback de desenho
void desenhaCena()
{
    // Propriedades das fontes de luz
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDif0[] = { d, d, d, 1.0 };
    float lightSpec0[] = { e, e, e, 1.0 };
    float lightDifAndSpec1[] = { 1.0, 1.0, 0.0, 1.0 };
    float lightPos1[] = { 1.0, 2.0, 0.0, 1.0 };
    float globAmb[] = { m, m, m, 1.0 };

    // Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec0);

    // Propriedades da fonte de luz LIGHT1
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);// Enable local viewpoint
    if (isLightingOn) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }
    glEnable(GL_LIGHT1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camerac(c);

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glColor3f(1, 1, 1);
    planeta(v[0].r,v[0].x,v[0].y,v[0].z,txt[0],0,0);
    glutWireSphere(0.05, 8, 8);
    glPopMatrix();
    glColor3f(1,1, 0);
    //Escreve os nomes dos planetas
    if(w==0)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Sol", v[0].x, v[0].y+10, v[0].z);
    glColor3f(1,1, 0);
    if(w==1)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Mercurio", v[1].x, v[1].y, v[1].z);
    glColor3f(1,1, 0);
    if(w==2)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Venus", v[2].x, v[2].y, v[2].z);
    glColor3f(1,1, 0);
    if(w==3)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Terra", v[3].x, v[3].y, v[3].z);
    glColor3f(1,1, 0);
    if(w==4)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Marte", v[4].x, v[4].y, v[4].z);
    glColor3f(1,1, 0);
    if(w==5)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Jupiter", v[5].x, v[5].y, v[5].z);
    glColor3f(1,1, 0);
    if(w==6)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Saturno", v[6].x, v[6].y, v[6].z);
    glColor3f(1,1, 0);
    if(w==7)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Urano", v[7].x, v[7].y, v[7].z);
    glColor3f(1,1, 0);
    if(w==8)glColor3f(1,0, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, "Netuno", v[8].x, v[8].y,v[8].z);
    //Escreve as descricoes
    if(c==0 && w==0){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Sol", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  695508 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Idade: 4,603x10^9 anos", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura na superficie: 5.778 K", 250, 0,-460);
    }
    if(c==0 && w==1){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Mercurio", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  2440 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 58d 15h 30m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 88 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : 166,65 C ", 200, 0,-460);
    }
    if(c==0 && w==2){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Venus", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  6052 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 116d 18h 0m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 225 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : 461 C ", 200, 0,-460);
    }
    if(c==0 && w==3){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Terra", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  6371 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 1d", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 365 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : 14 C ", 200, 0,-460);
    }
    if(c==0 && w==4){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Marte", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  3390 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 1d 0h 37m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 687 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : -63 C ", 200, 0,-460);
    }
    if(c==0 && w==5){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Jupiter", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  69911 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 0d 9h 56m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 4380 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : -108 C ", 200, 0,-460);
    }
    if(c==0 && w==6){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Saturno", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  58232 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 0d 10h 42m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 10585 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : -139 C ", 200, 0,-460);
    }
    if(c==0 && w==7){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Urano", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  25362 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 0d 17h 14m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 30660 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : -195 C ", 200, 0,-460);
    }
    if(c==0 && w==8){
        glColor3f(0,0, 1);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Nome:  Netuno", 400, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Raio:  24622 quilometros", 350, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Duracao do dia: 0d 16h 6m", 300, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Periodo Orbital: 60225 dias", 250, 0,-460);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Temperatura media : -215 C ", 200, 0,-460);
    }
    glEnable(GL_LIGHTING);
    matShine[0] = s;
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    glColor3f(1, 1, 1);
    //desenha os planetas,satelites e luas
    for(int i=1;i<9;i++)
    {
        int aux=100%rand()*i;
        vaux[i]=aux;
        planeta(v[i].r,v[i].x,v[i].y,v[i].z,txt[i],i,aux);
        luas(i,aux);
        if(i==3)
            for(int j=0;j<4;j++)
            {
                int aleatorio=100%rand()*j;
                satelite(i,aux,aleatorio);
            }
        satelite(i,aux,aux);
    }

    float rx,ry;
    glColor3f(1,1, 0);
    //desenha as orbitas
    if(mostrarLinhas==1){
        glBegin(GL_LINE_STRIP);
        for(int k=1;k<9;k++)
        {
            double l=0.1;
            for(double i=0;i<6.29;i+=l)
            {
                rx=(v[k].x)*sin(i);
                ry=(v[k].x)*cos(i);
                glVertex3f(ry,0,rx);
            }
        }
         glEnd();
    }

    glutSwapBuffers();
}
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'l':
    case 'L':
        isLightingOn = !isLightingOn;
        break;
    case 'W':
        w++;
        if(w==9)
            w=-1;
        break;
    case 'w':
        w--;
        if(w==-2)
            w=8;
        break;
    case 'd':
        mostrarLinhas=1;
        break;
    case 'D':
        mostrarLinhas=0;
        break;
    case 'c':
        c++;
        if(c==4)
            c=0;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
// Callback de redimensionamento
void resize(int w, int h)
{
    larguraJanela = w;
    alturaJanela = h;
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 1.0, 1500);
    glMatrixMode(GL_MODELVIEW);
}

void posicionaCamera(int x, int y) {
    xMouse = x;
    yMouse = y;;
    glutPostRedisplay();
}

void rotacionaEsfera() {
    anguloEsferaY +=1;
    anguloEsferaX +=8;
    anguloCubo+=rand()%4;
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    //configura o audio
    SDL_Init(
    SDL_INIT_AUDIO);

	SDL_AudioSpec wavSpec;
	Uint8* wavStart;
	Uint32 wavLength;

	if(SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength) == NULL)
	{
		// TODO: Proper error handling
		std::cerr << "Error: " << FILE_PATH
			<< " could not be loaded as an audio file" << std::endl;
			return 1;


	}

	AudioData audio;
	audio.pos = wavStart;
	audio.length = wavLength;

	wavSpec.callback = MyAudioCallback;
	wavSpec.userdata = &audio;

	SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL,
			SDL_AUDIO_ALLOW_ANY_CHANGE);
	if(device == 0)
	{
		// TODO: Proper error handling
		std::cerr << "Error: " << SDL_GetError() << std::endl;
		return 1;

	}


	SDL_PauseAudioDevice(device, 0);
	srand(time(0));
    cadastrar(&v[0],5,0,0,0,txt[0]);
    while(i<9)
    {
            if(i==1)
                cadastrar(&v[i],4,4*(v[i-1].r)+v[i-1].x,2,v[i-1].z,txt[i]);
            if(i<4&&i!=1)
                cadastrar(&v[i],(v[i-1].r)*1.2,4*(v[i-1].r)+v[i-1].x,rand()%12,v[i-1].z,txt[i]);
            if(i==4)
                cadastrar(&v[i],(v[i-1].r)*0.9,4*(v[i-1].r)+v[i-1].x,rand()%12,v[i-1].z,txt[i]);
            if(i==5)
                cadastrar(&v[i],(v[i-1].r)*3,9*(v[i-1].r)+v[i-1].x,rand()%12,v[i-1].z,txt[i]);
            if(i>=6)
                cadastrar(&v[i],(v[i-1].r)*0.9,6*(v[i-1].r)+v[i-1].x,rand()%1,v[i-1].z,txt[i]);
            i++;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Sistema Solar");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutPassiveMotionFunc(posicionaCamera);
    glutSpecialFunc(specialKeyInput);
    glutIdleFunc(rotacionaEsfera);

    setup();

    glutMainLoop();
    SDL_CloseAudioDevice(device);
	SDL_FreeWAV(wavStart);
	SDL_Quit();


}
