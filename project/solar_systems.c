/**
 * Escala:
 * http://www.if.ufrgs.br/oei/cgu/sca/sca.htm
 * 
 * Raio em km, Obita em (1.000.000) km
 * 1. Sol -
 * 		Raio = 696.000 
 * 		orbita = 0
 * 
 * 2. Mercúrio -
 * 		Raio = 2.440
 * 		orbita = 57,91
 * 
 * 3. Vênus -
 * 		Raio = 12.104
 * 		orbita = 108,16
 * 
 * 4. Terra - 
 * 		Raio = 12.742
 * 		orbita = 149,6
 * 
 * 5. Marte - 
 * 		Raio = 6.780
 * 		orbita = 228
 * 
 * 6. Júpiter - 
 * 		Raio = 139.822
 * 		orbita = 778,4
 * 
 * 7. Saturno -
 * 		Raio = 116.464
 * 		orbita = 1.427
 * 
 * 8. Urano -
 * 		Raio = 50.724
 * 		orbita = 2.869,6
 * 
 * 9. Netuno - 
 * 		Raio = 49.248
 * 		orbita = 4.496.6
 *		 
 */

#include <stdbool.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Biblioteca para leitura de imagens
// https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "codes/stb_image.h"


//Estruturas
typedef struct{
	float Translacao;
	float vRotacao;
	float aRotacao;
	int Faces;
	int TamanhoTranslacao;
	float Raio;
	bool Estado;
	float posX;
	float posZ;
	int timerTrans;
	int timerRot;
}Planeta;

typedef struct{
	int X;
	int Y;
	int Z;
}EixoINT;

typedef struct{
	float X;
	float Y;
	float Z;
}EixoFLOAT;

typedef struct{
	float Largura;
	float Altura;
}Janela;

//Variaveis das planetas
Planeta sun;
Planeta mercury;
Planeta venus;
Planeta earth;
Planeta mars;
Planeta jupiter;
Planeta uranus;
Planeta neptune;
Planeta saturn;

//Variaveis da Camera
int modoCamera = 1;
float anguloCameraA = 90;
float anguloCameraB = 0;
EixoFLOAT camera;
EixoINT cursor;
EixoINT mouse;
Janela janela;
int horizonteEventos = 1;//Espaço Visivel
int escala_orbita = 1000;
int escala_raio_planet = 1;

// Variaves de movimento de camera
int xInicial;
int yInicial;
int xMove = 0;
int yMove = 0;
float anguloAtualX = M_PI_2;
float anguloAtualY = M_PI/6;
int distCamera = 4000;
float focoCamX = 0;
float focoCamY = 0;
float focoCamZ = 0;
float offsetX = 0;
float offsetZ = 0;
float tamRastro = 1;

// Variaveis de textura
// identificador das texturas
GLuint texturaId[11];

// textura 
// parametros são id -> identifica a imagem no opengl, filePath -> caminho da imagem
void carregaTextura(GLuint texId, char* filePath){
    // guarda os dados da imagem
    unsigned char* imgData;

    // dados da imagem
    int largura, altura, canais;
    
    stbi_set_flip_vertically_on_load(true);
    // leitura da imagem -> caminho/largura/altura/qtd de canais no arquivo/qtd de canais
    imgData = stbi_load(filePath, &largura, &altura, &canais, 4);

    if(imgData){
        // Liga um espaço de memoria no open gl ao id para criar a textura
        glBindTexture(GL_TEXTURE_2D, texId);
    
        // criação da textura
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(imgData);
    }
    else {
        printf("ERRO - Não foi possível carregar a Imagem!");
    }
}

void setup(){
	
  
  	/*Configurações dos Planetas*/
	// Link da translação e Rotação
	// http://www.invivo.fiocruz.br/cgi/cgilua.exe/sys/start.htm?infoid=1177&sid=9
 	// 
  //SOL

	sun.Translacao = 0;
	sun.Faces = 50;
	sun.Raio = 80; //fora de escala
	sun.Estado = true;
	sun.vRotacao = 0;
	sun.aRotacao = 0;

	//MERCURIO
	
	mercury.Translacao = 1; // 0.24 Anos - 0.24/0.24
	mercury.vRotacao = 14300; // 58.6 dias - 58.6/0.41 = 142.93
	mercury.aRotacao = 0;
	mercury.Raio = 1; //-> 2440/2440
	mercury.TamanhoTranslacao = 115.82; //-> 57.910.000/500.000
	mercury.Faces = 50;
	mercury.Estado = true;

	//VENUS

	venus.Translacao = 3; // 0.62 Anos - 0.62/0.24 = 2.58
	venus.vRotacao = 59300; // 243 dias - 243/0.41 = 592.68
	venus.aRotacao = 0;
	venus.Raio = 4.96; //-> 12104/2440
	venus.TamanhoTranslacao = 216.32; //-> 108.160.000/500.000
	venus.Faces = 50;
	venus.Estado = true;

	//TERRA

	earth.Translacao = 4; // 1 Ano - 1/0.24 = 4.16
	earth.vRotacao = 300; // 1 dia - 1/0.41 = 2.43
	earth.aRotacao = 0;
	earth.Raio = 5.22; //-> 12.742/2440
	earth.TamanhoTranslacao = 299.2; //-> 149.600.000/500.000
	earth.Faces = 50;
	earth.Estado = true;

	//MARTE

	mars.Translacao = 8; // 1.88 Anos - 1.88/0.24 = 7.83
	mars.vRotacao = 300; // 1.03 dia - 1.03/0.41 = 2.51
	mars.aRotacao = 0;
	mars.Raio = 2.77; //-> 6.780/2440
	mars.TamanhoTranslacao = 456; // 228.000.000/500.000
	mars.Faces = 50;
	mars.Estado = true;

	//JUPITER

	jupiter.Translacao = 49; // 11.86 Anos - 11.86/0.24 = 49.42
	jupiter.vRotacao = 100; // 0.41 dia - 0.41/0.41 = 1
	jupiter.aRotacao = 0;
	jupiter.Raio = 57.3; //-> 139.822/2.440
	jupiter.TamanhoTranslacao = 778.4; //-> 778.400.000/1.000.000
	jupiter.Faces = 50;
	jupiter.Estado = true;

	///Saturno

	saturn.Translacao = 123; // 29.46 Anos - 29.46/0.24 = 122.75
	saturn.vRotacao = 100; // 0.45 dia - 0.45/0.41 = 1.09
	saturn.aRotacao = 0; 
	saturn.Raio = 47.73; //-> 116.464/2440
	saturn.TamanhoTranslacao = 1427; //-> 1.427.000.000/1.000.000
	saturn.Faces = 50;
	saturn.Estado = true;

	//URANO
	uranus.Translacao = 350; // 84.01 Anos - 84.01/0.24 = 350.04
	uranus.vRotacao = 200; // 0.72 dia - 0.72/0.41 = 1.75
	uranus.aRotacao = 0;
	uranus.Raio = 20.79; //-> 50.724/2440
	uranus.TamanhoTranslacao = 1913.0; //-> 2.869.600.000/1.500.000
	uranus.Faces = 50;
	uranus.Estado = true;

	//NETUNO
	neptune.Translacao = 687; // 164.79 Anos - 164.79/0.24 = 686.62
	neptune.vRotacao = 200;	// 0.67 dia - 0.67/0.41 = 1.63
	neptune.aRotacao = 0;
	neptune.Raio = 20.19;  //-> 49.248/2440
	neptune.TamanhoTranslacao = 2997.7; //-> 4.496.600.000/1.500.000
	neptune.Faces = 50;
	neptune.Estado = true;
}

void pCam(){
	float theta = (xMove/500.0)*M_PI + anguloAtualX;
	float phi = (yMove/500.0)*M_PI + anguloAtualY;

	camera.X = distCamera*cos(theta)*cos(phi) + offsetX;
	camera.Y = distCamera*sin(phi);
	camera.Z = distCamera*sin(theta)*cos(phi) + offsetZ;
	
    float faixaAngulo = ((phi/(2.0*M_PI)) - (int)(phi/(2.0*M_PI)))*2.0*M_PI;
	
    if((faixaAngulo >= M_PI_2 && faixaAngulo <= 3*M_PI_2 )||(faixaAngulo <= -M_PI_2 && faixaAngulo >= -3*M_PI_2 ))
	    gluLookAt(camera.X, camera.Y, camera.Z, focoCamX, focoCamY, focoCamZ, 0, -1, 0);
    else 
        gluLookAt(camera.X, camera.Y, camera.Z, focoCamX, focoCamY, focoCamZ, 0, 1, 0);
}

void desenhaEsfera(float raio, int lHori, int lVert, GLint texId){
    glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texId);
            glRotated(-90, 1,0,0);
            GLUquadricObj* q = gluNewQuadric ( );
            gluQuadricDrawStyle ( q, GLU_FILL );
            gluQuadricNormals ( q, GLU_SMOOTH );
            gluQuadricTexture ( q, GL_TRUE );
            gluSphere ( q, raio, 50, 50 );
            gluDeleteQuadric ( q );
			glRotated(90, 1,0,0);
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void moveMouse(int x, int y){
	xMove = x - xInicial;
	yMove = y - yInicial;
	glutPostRedisplay();
}

void mousePress(int button, int state, int x, int y){
	switch (button)
    {
        case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				xInicial = x;
				yInicial = y;
			}

            break;
        case GLUT_RIGHT_BUTTON: printf("Botao direito.");
                                                        break;
        case GLUT_MIDDLE_BUTTON: printf("Botao do meio.");
                                                        break;
    }

	if(state == GLUT_UP){
		anguloAtualX += (xMove/500.0)*M_PI;
		anguloAtualY += (yMove/500.0)*M_PI;

		xMove = 0;
		yMove = 0;
	}
    
}

void renderSystem(){
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	// cria um objeto disco de orbita
    GLUquadric *disc;
    disc = gluNewQuadric(); 

    glPushMatrix();
		// Rotação para enquadra na camera
        glRotatef(90, 1, 0, 0);
        // gluDisk — desenha um disco
        // O gluDisk renderiza um disco no plano z = 0. 
        // O disco tem um raio externo e contém um furo circular concêntrico com um raio interno. 
        // Se internal for 0, nenhum furo será gerado.
        //  O disco é subdividido em torno do eixo z em fatias (como fatias de pizza) e também em torno do eixo z em anéis (conforme especificado por fatias e loops, respectivamente).
        gluDisk(disc, mercury.TamanhoTranslacao-tamRastro, mercury.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, venus.TamanhoTranslacao-tamRastro, venus.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, earth.TamanhoTranslacao-tamRastro, earth.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, mars.TamanhoTranslacao-tamRastro, mars.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, jupiter.TamanhoTranslacao-tamRastro, jupiter.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, saturn.TamanhoTranslacao-tamRastro, saturn.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, uranus.TamanhoTranslacao-tamRastro, uranus.TamanhoTranslacao+tamRastro, 600, 600);
		gluDisk(disc, neptune.TamanhoTranslacao-tamRastro, neptune.TamanhoTranslacao+tamRastro, 600, 600);
    glPopMatrix();
	
    // delete objeto para não ocupar memoria desnecessario
    gluDeleteQuadric(disc);

	// desenha esfera (raio, numeros de divisões em x, numero de divisoesem y, textura do objetos
	// Sol ->
	
	desenhaEsfera(sun.Raio, sun.Faces, sun.Faces, texturaId[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// Mercurio ->
	// Desloca para posição do planeta mercurio no espaco
	glTranslatef(mercury.posX, 0.0, mercury.posZ);
	// Responsavel pela rotação emtorno do proprio eixo
	glRotated(mercury.aRotacao, 0, 1, 0);
	desenhaEsfera(mercury.Raio, mercury.Faces, mercury.Faces, texturaId[1]);
	// Remove a rotação aplicada na esfera
	glRotated(-mercury.aRotacao, 0, 1, 0);
	// Remove o Deslocamento de posição volta para o (0,0,0)
	glTranslatef(-mercury.posX, 0.0, -mercury.posZ);

	// Venus ->
	glTranslatef(venus.posX, 0.0, venus.posZ);
	glRotated(venus.aRotacao, 0, 1, 0);
	desenhaEsfera(venus.Raio, venus.Faces, venus.Faces, texturaId[2]);
	glRotated(-venus.aRotacao, 0, 1, 0);
	glTranslatef(-venus.posX, 0.0, -venus.posZ);

	// Terra ->
	glTranslatef(earth.posX, 0.0, earth.posZ);
	glRotated(earth.aRotacao, 0, 1, 0);
	desenhaEsfera(earth.Raio, earth.Faces, earth.Faces, texturaId[3]);
	glRotated(-earth.aRotacao, 0, 1, 0);
	glTranslatef(-earth.posX, 0.0, -earth.posZ);
	
	// Marte ->
	glTranslatef(mars.posX, 0.0, mars.posZ);
	glRotated(mars.aRotacao, 0, 1, 0);
	desenhaEsfera(mars.Raio, mars.Faces, mars.Faces, texturaId[4]);
	glRotated(-mars.aRotacao, 0, 1, 0);
	glTranslatef(-mars.posX, 0.0, -mars.posZ);

	// Jupiter ->
	glTranslatef(jupiter.posX, 0.0, jupiter.posZ);
	glRotated(jupiter.aRotacao, 0, 1, 0);
	desenhaEsfera(jupiter.Raio, jupiter.Faces, jupiter.Faces, texturaId[5]);
	glRotated(-jupiter.aRotacao, 0, 1, 0);
	glTranslatef(-jupiter.posX, 0.0, -jupiter.posZ);

	// Saturno ->
	glTranslatef(saturn.posX, 0.0, saturn.posZ);

	// criando o Anel de saturno, abilita o uso de textura/cor no objeto
	glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			// "cola" a textura no objeto que sera gerado
			glBindTexture(GL_TEXTURE_2D, texturaId[7]);
            glRotated(10, 1,0,1);
            int fixo = 3;
			for (int i = 0; i < 360; i += 3){
				// Inicia um objeto do tipo quadrado 
                glBegin(GL_QUADS);
				    float ang = i*(1.0/60.0)*M_PI;
				    float ang2 = (fixo+i)*(1/60.0)*M_PI;
					// As texturas sao 2d logo:
					// glTexCoord2f - (0,0) corresponde a parte inferior esquerda da imagem/textura
					// (1,0) corresponde a parte superior esquerda da imagem/textura
					// (1,1) corresponde a parte superior direita da imagem/textura
					// (0,1) corresponde a parte inferior esquerda da imagem/textura
					// esse mapeamento e feito em no varios quadarados em torno do planeta saturno 
				    glTexCoord2f(0.0, 0.0); glVertex3f((saturn.Raio+5.0)*cos(ang), 0, (saturn.Raio+5.0)*sin(ang));
				    glTexCoord2f(1.0, 0.0); glVertex3f((saturn.Raio+40.0)*cos(ang), 0, (saturn.Raio+40.0)*sin(ang));
				    glTexCoord2f(1.0, 1.0); glVertex3f((saturn.Raio+40.0)*cos(ang2), 0, (saturn.Raio+40.0)*sin(ang2));
				    glTexCoord2f(0.0, 1.0); glVertex3f((saturn.Raio+5.0)*cos(ang2), 0, (saturn.Raio+5.0)*sin(ang2));
                glEnd();
			}
		glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	glRotated(saturn.aRotacao, 0, 1, 0);
	desenhaEsfera(saturn.Raio, saturn.Faces, saturn.Faces, texturaId[6]);
	glRotated(-saturn.aRotacao, 0, 1, 0);
	glTranslatef(-saturn.posX, 0.0, -saturn.posZ);

	// Urano ->
	glTranslatef(uranus.posX, 0.0, uranus.posZ);
	glRotated(uranus.aRotacao, 0, 1, 0);
	desenhaEsfera(uranus.Raio, uranus.Faces, uranus.Faces, texturaId[8]);
	glRotated(-uranus.aRotacao, 0, 1, 0);
	glTranslatef(-uranus.posX, 0.0, -uranus.posZ);

	// Netuno ->
	glTranslatef(neptune.posX, 0.0, neptune.posZ);
	glRotated(neptune.aRotacao, 0, 1, 0);
	desenhaEsfera(neptune.Raio, neptune.Faces, neptune.Faces, texturaId[9]);
	glRotated(-neptune.aRotacao, 0, 1, 0);
	glTranslatef(-neptune.posX, 0.0, -neptune.posZ);

	glutSwapBuffers();
}

void init(void) 
{
	mercury.timerTrans = 0;
	mercury.timerRot = 0;
	mercury.Estado = false;

	venus.timerTrans = 0;
	venus.timerRot = 0;
	venus.Estado = false;

	earth.timerTrans = 0;
	earth.timerRot = 0;
	earth.Estado = false;

	mars.timerTrans = 0;
	mars.timerRot = 0;
	mars.Estado = false;

	jupiter.timerTrans = 0;
	jupiter.timerRot = 0;
	jupiter.Estado = false;

	saturn.timerTrans = 0;
	saturn.timerRot = 0;
	saturn.Estado = false;

	uranus.timerTrans = 0;
	uranus.timerRot = 0;
	uranus.Estado = false;

	neptune.timerTrans = 0;
	neptune.timerRot = 0;
	neptune.Estado = false;

  	// limpa bufer de cor
  	glClearColor (0.0, 0.0, 0.0, 0.0);

	// Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);

    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);  

	// Habilita a luz de número 0
    glEnable(GL_LIGHT0);

	// Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);

	// Gera vetor de texturas -> Param1: qtdade de texturas e -> Param2: variavel que sera o identificador das texturas
    glGenTextures(11, texturaId);

    // Parametros da textura no objeto -> GL_REPLACE - Substitui o a cor pela textura
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // função implementada carrega a textura
	// https://www.solarsystemscope.com/textures/
    carregaTextura(texturaId[0], "texturas/Sol.jpg");
	carregaTextura(texturaId[1], "texturas/Mercurio.jpg");
	carregaTextura(texturaId[2], "texturas/Venus.jpg");
	carregaTextura(texturaId[3], "texturas/Terra.jpg");
	carregaTextura(texturaId[4], "texturas/Marte.jpg");
	carregaTextura(texturaId[5], "texturas/Jupiter.jpg");
	carregaTextura(texturaId[6], "texturas/Saturno.jpg");
	carregaTextura(texturaId[7], "texturas/AnelSaturno.png");
	carregaTextura(texturaId[8], "texturas/Urano.jpg");
	carregaTextura(texturaId[9], "texturas/Netuno.jpg");
	carregaTextura(texturaId[10], "texturas/Ceu.jpg");

  	// glShadeModel — selecione sombreamento plano ou suave
  	// As primitivas GL podem ter sombreamento plano ou suave. O sombreamento suave, 
  	// o padrão, faz com que as cores computadas dos vértices sejam interpoladas
  	//  à medida que o primitivo é rasterizado, normalmente atribuindo cores diferentes a cada fragmento de pixel resultante. 
  	// O sombreamento plano seleciona a cor calculada de apenas um vértice e a atribui a todos os fragmentos de pixel gerados pela rasterização de uma única primitiva.
  	//  Em ambos os casos, a cor calculada de um vértice é o resultado da iluminação se a iluminação estiver ativada ou é a cor atual no momento em que o vértice foi especificado se a iluminação estiver desativada
  	glShadeModel (GL_SMOOTH);
}

void DefineIluminacao (void)
{
        GLfloat luzAmbiente[4]={0.03,0.03,0.03,1.0}; 
        GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};          // "cor" 
        GLfloat luzEspecular[4]={0.2, 0.2, 0.2, 1.0};// "brilho" 
        GLfloat posicaoLuz[4]={0.0, 0.0, 0.0, 1.0};
 
        // Capacidade de brilho do material
        GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
        GLint especMaterial = 60;
 
        // Define a refletância do material 
        glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
        // Define a concentração do brilho
        glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
 
        // Ativa o uso da luz ambiente 
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
 
        // Define os parâmetros da luz de número 0
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
        glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
        glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );   
}

void display(void)
{
  //limpando buffer de cor e profundidade
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glLoadIdentity — substitui a matriz atual pela matriz identidade
  // lipando a matrix - retirando sugeiras
  glLoadIdentity();
  //setando cor padrão
  glColor3f (1.0, 1.0, 1.0);
  
  pCam();
  DefineIluminacao();
  renderSystem();

}

void reshape (int w, int h)
{
  //glViewport especifica a transformação afim de xey de coordenadas de dispositivo normalizadas para coordenadas de janela. Seja x nd y nd as coordenadas normalizadas do dispositivo. Em seguida, 
  // as coordenadas da janela x w y w são calculadas da seguinte forma:
  // x w = x nd + 1 ⁢ width 2 + x
  // y w = y nd + 1 ⁢ height 2 + y
  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 

  //glMatrixMode — especifica qual matriz é a matriz atual
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //gluPerspective — configura uma matriz de projeção em perspectiva
  //gluPerspective especifica um tronco de visualização no sistema de coordenadas do mundo. 
  //Em geral, a proporção no gluPerspective deve corresponder à proporção da janela de visualização associada. 
  //Por exemplo, aspecto = 2,0 significa que o ângulo de visão do espectador é duas vezes maior em x do que em y. 
  // Se a janela de visualização for duas vezes maior que a altura, ela exibirá a imagem sem distorção
  gluPerspective(60.0, (float)w/(float)h, 0.2, 9000.0);//2147483647
  //glMatrixMode — especifica qual matriz é a matriz atual
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 1080, 0, 0, 0, 0, 1, 0);
}
// função que faz controle de variaveis dinâmicas
void keyboard (unsigned char key, int x, int y)
{
   switch(key){
		case 'E':
			tamRastro = 1;
			distCamera = 4000;
			focoCamX = 0.0;
			focoCamY = 0.0;
			focoCamZ = 0.0;
			offsetX = 0.0;
			offsetZ = 0.0;

			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;

    	glutPostRedisplay();
		break;
		case 'e':
			tamRastro = 1;
			distCamera = 4000;
			focoCamX = 0.0;
			focoCamY = 0.0;
			focoCamZ = 0.0;
			offsetX = 0.0;
			offsetZ = 0.0;

			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;

    	glutPostRedisplay();
		break;

    	case '0':
			tamRastro = 0.1;
			distCamera = 250;
			focoCamX = 0.0;
			focoCamY = 0.0;
			focoCamZ = 0.0;
			offsetX = 0.0;
			offsetZ = 0.0;

			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;

    	glutPostRedisplay();
		break;

		case '1':
			tamRastro = 0.01;
			distCamera = 3;
			mercury.Estado = true;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '2':
			tamRastro = 0.01;
			distCamera = 15;
			mercury.Estado = false;
			venus.Estado = true;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '3':
			tamRastro = 0.01;
			distCamera = 16;
			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = true;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '4':
			tamRastro = 0.01;
			distCamera = 9;
			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = true;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '5':
			tamRastro = 0.01;
			distCamera = 172;
			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = true;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '6':
			tamRastro = 0.01;
			distCamera = 144;
			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = true;
			uranus.Estado = false;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '7':
			tamRastro = 0.01;
			distCamera = 63;
			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = true;
			neptune.Estado = false;
    	glutPostRedisplay();
		break;

		case '8':
			tamRastro = 0.01;
			distCamera = 61;
			mercury.Estado = false;
			venus.Estado = false;
			earth.Estado = false;
			mars.Estado = false;
			jupiter.Estado = false;
			saturn.Estado = false;
			uranus.Estado = false;
			neptune.Estado = true;
    	glutPostRedisplay();
		break;
	}
}

void movimentoPlaneta(Planeta* planeta){

	float ang = (*planeta).timerTrans*(M_PI/(180.0*(*planeta).Translacao));

	(*planeta).posX = (*planeta).TamanhoTranslacao*cos(ang);
	(*planeta).posZ = (*planeta).TamanhoTranslacao*sin(ang);

	if((*planeta).Estado){
		offsetX = (*planeta).posX;
		offsetZ = (*planeta).posZ;
		focoCamX = (*planeta).posX;
		focoCamZ = (*planeta).posZ;
	}

	(*planeta).timerTrans++;

	if((*planeta).timerTrans == 360*(*planeta).TamanhoTranslacao){
		(*planeta).timerTrans = 0;
	}

	(*planeta).aRotacao = (*planeta).timerRot*(360/(*planeta).vRotacao);

	(*planeta).timerRot++;

	if((*planeta).timerRot == (*planeta).vRotacao){
		(*planeta).timerRot = 0;
	}


}

void Timer(int value){
	// Animação dos planetas
	// Mercurio
	movimentoPlaneta(&mercury);
	glutPostRedisplay();
	// Venus
	movimentoPlaneta(&venus);
	glutPostRedisplay();
	// Terra
	movimentoPlaneta(&earth);
	glutPostRedisplay();
	// Marte
	movimentoPlaneta(&mars);
	glutPostRedisplay();
	// Jupiter
	movimentoPlaneta(&jupiter);
	glutPostRedisplay();
	// Saturno
	movimentoPlaneta(&saturn);
	glutPostRedisplay();
	// Urano
	movimentoPlaneta(&uranus);
	glutPostRedisplay();
	// Netuno
	movimentoPlaneta(&neptune);
	glutPostRedisplay();

	// Redesenha o quadrado com as novas coordenadas 
    glutTimerFunc(1,Timer, 1);
}

int main(int argc, char** argv)
{
  setup();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(169300, 169300);
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mousePress);
  glutMotionFunc(moveMouse);
  glutTimerFunc(1, Timer, 1);
  glutMainLoop();
  return 0;
}
