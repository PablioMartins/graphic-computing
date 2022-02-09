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
int modoCamera = 0;
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
GLuint texturaId[10];

// textura 
// parametros são (texId -> identifica a imagem no opengl), (filePath -> caminho da imagem)
void carregaTextura(GLuint texId, char* filePath){
    // Variável para guardar os dados da imagem
    unsigned char* imgData;

    // Variaveis para guardar os parametros da imagem
    int largura, altura, canais;
    
    // Define um flip vertical de leitura da imagem
	/**
	 * x - onde começa a leitura
	 * 
	 *  ________________				 ________________
	 * |x				|				|				 |
	 * |				|				|				 |
	 * |				|				|				 |
	 * |	imagem		| 				|	  imagem	 |
	 * |				|		->		|				 |
	 * |				|				|				 |
	 * |________________|				|x_______________|
	 *    antes do flip						apos o flip
	 * 
	 * A mudança foi necessária devido ao opengl que tem como padrão de leitura
	 * o canto inferior esquerdo
	 */
	stbi_set_flip_vertically_on_load(true);

    // leitura da imagem -> caminho/largura/altura/qtd de canais no arquivo/qtd de canais
    imgData = stbi_load(filePath, &largura, &altura, &canais, 4);

    if(imgData){
        // Liga um espaço de memoria no opengl ao texId passado para criar a textura
        glBindTexture(GL_TEXTURE_2D, texId);
    
        // Criação da textura apartir de uma imagem 2D
		// Parametros:
		// 1 - O objeto: GL_TEXTURE_2D
		// 2 - O nivel de detalhamento: para imagens é "0" 
		// 3 - Especifica o formato das cores na textura: GL_RGBA8 (Red - Gree - Blue - Alpha de 8bits)
		// 4 - lagura da textura: no momento da leitura da imagem foi atribuida a variável "largura"
		// 5 - altura da textura: no momento da leitura da imagem foi atribuida a variável "altura"
		// 6 - tamanho da borda: "0" (sem borda)
		// 7 - Formato de cores dos dados da imagem: GL_RGBA
		// 8 - Especifica o tipo de dado da imagem: GL_UNSIGNED_BYTE (8 bits sem sinal)
		// 9 - Dados da imagem: Armazenado em "imgData"
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		// Resolução de problema de miniaturização (Textura com alta resolução quando distante da camera)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Quando há necessidade de ampliação da textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Repete a imagem em x (S no opengl) sempre que a largura da imagem for menor que a lagura da textura
		// Para esse projeto a "lagura da Imagem" = "lagura da textura"
		// devido ao parametro "largura" da textura ser o mesmo extraído da imagem
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		// Repete a imagem em y (T no opengl) sempre que a altura da imagem for menor que a altura da textura
		// Para esse projeto a "altura da Imagem" = "altura da textura"
		// devido ao parametro "altru" da textura ser o mesmo extraído da imagem
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Liberação da memoria oculpada pela imagem
        stbi_image_free(imgData);
    }
    else {
        printf("ERRO - Não foi possível carregar a Imagem! - ");
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
	mercury.vRotacao = 14293; // 58.6 dias - 58.6/0.41 = 142.93 -> x100 (diminuir a velocidade de rotação)
	mercury.aRotacao = 0;
	mercury.Raio = 1; //-> 2440/2440
	mercury.TamanhoTranslacao = 115.82; //-> 57.910.000/500.000
	mercury.Faces = 50;
	mercury.Estado = true;

	//VENUS

	venus.Translacao = 3; // 0.62 Anos - 0.62/0.24 = 2.58
	venus.vRotacao = 59268; // 243 dias - 243/0.41 = 592.68 -> x100 (diminuir a velocidade de rotação)
	venus.aRotacao = 0;
	venus.Raio = 4.96; //-> 12104/2440
	venus.TamanhoTranslacao = 216.32; //-> 108.160.000/500.000
	venus.Faces = 50;
	venus.Estado = true;

	//TERRA

	earth.Translacao = 4; // 1 Ano - 1/0.24 = 4.16
	earth.vRotacao = 243; // 1 dia - 1/0.41 = 2.43 -> x100 (diminuir a velociade de rotação)
	earth.aRotacao = 0;
	earth.Raio = 5.22; //-> 12.742/2440
	earth.TamanhoTranslacao = 299.2; //-> 149.600.000/500.000
	earth.Faces = 50;
	earth.Estado = true;

	//MARTE

	mars.Translacao = 8; // 1.88 Anos - 1.88/0.24 = 7.83
	mars.vRotacao = 251; // 1.03 dia - 1.03/0.41 = 2.51 -> x100 (diminuir a velociade de rotação)
	mars.aRotacao = 0;
	mars.Raio = 2.77; //-> 6.780/2440
	mars.TamanhoTranslacao = 456; // 228.000.000/500.000
	mars.Faces = 50;
	mars.Estado = true;

	//JUPITER

	jupiter.Translacao = 49; // 11.86 Anos - 11.86/0.24 = 49.42
	jupiter.vRotacao = 100; // 0.41 dia - 0.41/0.41 = 1 -> x100 (diminuir a velociade de rotação)
	jupiter.aRotacao = 0;
	jupiter.Raio = 57.3; //-> 139.822/2.440
	jupiter.TamanhoTranslacao = 778.4; //-> 778.400.000/1.000.000
	jupiter.Faces = 50;
	jupiter.Estado = true;

	///Saturno

	saturn.Translacao = 123; // 29.46 Anos - 29.46/0.24 = 122.75
	saturn.vRotacao = 109; // 0.45 dia - 0.45/0.41 = 1.09 -> x100 (diminuir a velociade de rotação)
	saturn.aRotacao = 0; 
	saturn.Raio = 47.73; //-> 116.464/2440
	saturn.TamanhoTranslacao = 1427; //-> 1.427.000.000/1.000.000
	saturn.Faces = 50;
	saturn.Estado = true;

	//URANO
	uranus.Translacao = 350; // 84.01 Anos - 84.01/0.24 = 350.04
	uranus.vRotacao = 175; // 0.72 dia - 0.72/0.41 = 1.75 -> x100 (diminuir a velociade de rotação)
	uranus.aRotacao = 0;
	uranus.Raio = 20.79; //-> 50.724/2440
	uranus.TamanhoTranslacao = 1913.0; //-> 2.869.600.000/1.500.000
	uranus.Faces = 50;
	uranus.Estado = true;

	//NETUNO
	neptune.Translacao = 687; // 164.79 Anos - 164.79/0.24 = 686.62
	neptune.vRotacao = 163;	// 0.67 dia - 0.67/0.41 = 1.63 -> x100 (diminuir a velociade de rotação)
	neptune.aRotacao = 0;
	neptune.Raio = 20.19;  //-> 49.248/2440
	neptune.TamanhoTranslacao = 2997.7; //-> 4.496.600.000/1.500.000
	neptune.Faces = 50;
	neptune.Estado = true;
}

// Definições de câmera
void pCam(){
	/**
	 * Equação parametrica da esfera no espaço (definição para esse projeto com up da camera em y)
	 * x = (raio)*cos(theta)*cos(phi)
	 * y = (raio)*sin(phi)
	 * z = (raio)*sin(theta)*cos(phi)
	 * 
	 * (raio) - distancia entre o foco da camera e a camera
	 * theta - corresponde ao ângulo rotacionado com eixo rotacional vertical
	 * phi - corresponde ao ângulo rotacionado com eixo rotacional horizontal
	 * 
	 * a cada 500 pixel corresponde a 180 graus de rotação
	 */
	float theta = (xMove/500.0)*M_PI + anguloAtualX;
	float phi = (yMove/500.0)*M_PI + anguloAtualY;

	// Define a posição de camera (x,y,z) apartir dos parametros theta e phi
	// OffsetX e OffsetZ deslocamento de posição de camera quando o foco e atribuido aos planetas
	camera.X = distCamera*cos(theta)*cos(phi) + offsetX;
	camera.Y = distCamera*sin(phi);
	camera.Z = distCamera*sin(theta)*cos(phi) + offsetZ;
	
	// define qual o valor de phi na faixa 0 até 2pi
    float faixaAngulo = ((phi/(2.0*M_PI)) - (int)(phi/(2.0*M_PI)))*2.0*M_PI;
	
    if((faixaAngulo >= M_PI_2 && faixaAngulo <= 3*M_PI_2 )||(faixaAngulo <= -M_PI_2 && faixaAngulo >= -3*M_PI_2 ))
	    // orientação do up da camera em -y quando -90° < phi < 90°
		gluLookAt(camera.X, camera.Y, camera.Z, focoCamX, focoCamY, focoCamZ, 0, -1, 0);
    else 
		// orientação do up da camera em -y quando phi < -90° e phi > 90°
        gluLookAt(camera.X, camera.Y, camera.Z, focoCamX, focoCamY, focoCamZ, 0, 1, 0);
}

// Desenha e atribui textura a esfera
void desenhaEsfera(float raio, int lHori, int lVert, GLint texId){
	// Habilita Textura
    glEnable(GL_TEXTURE_2D);
        glPushMatrix();
			// Faz com que o opengl use a textura associado a texId
            glBindTexture(GL_TEXTURE_2D, texId);

			// Execulta rotacao de -90 graus com eixo rotativo em x (devido ao up de camera)
            glRotated(-90, 1,0,0);

			// cria um objeto Quadric (quádrico)
            GLUquadricObj* q = gluNewQuadric ( );

			// Define o estilo de desenho  - GLU_FILL -> renderização das quádricas como poligonos primitivos
            gluQuadricDrawStyle ( q, GLU_FILL );

			// Define os vettores normas da Quadric - GLU_SMOOTH -> Uma normal para cada vértice
            gluQuadricNormals ( q, GLU_SMOOTH );

			// Abilita textura na Quadric
            gluQuadricTexture ( q, GL_TRUE );

			// Desenha esfera com a Quadric - (lHori e lVert - corresponde a quantidade meridianos e paralelos)
            gluSphere ( q, raio, lHori, lVert);

			// exclui a qua quadric
            gluDeleteQuadric ( q );

			// remove a rotação feita no inicio (retorna a posição inicial)
			glRotated(90, 1,0,0);
        glPopMatrix();
	
	// Desabilita a textura
    glDisable(GL_TEXTURE_2D);
}

// Captura do movimento do mouse
void moveMouse(int x, int y){
	// modo rotação
	if(modoCamera == 1){
		// Calcula o quanto o mouse se movel em "x" e "y" apartir "x" e "y" em que ocoreu o pressionamento 
		// do botão esquerdo do mouse
		xMove = x - xInicial;
		yMove = y - yInicial;

		// Faz um redesenho na tela
		glutPostRedisplay();
	} 
	// modo zoom
	else if(modoCamera == 2){
		// muda a distancia de camera
		distCamera += (y - yInicial);
		yInicial = y;

		// Limita a aproximação
		if(distCamera < 1){
			distCamera = 1;
		}

		// Limita o afastamento 
		if(distCamera > 4500){
			distCamera = 4500;
		}

		glutPostRedisplay();
	}
	
}

void mousePress(int button, int state, int x, int y){
	switch (button)
    {
        // Avalia o pressionamento do botão esquerdo do mouse
		case GLUT_LEFT_BUTTON:
			// Quando pressionado
			if(state == GLUT_DOWN){
				// Define a posição (x,y) na tela em que foi pressinado
				xInicial = x;
				yInicial = y;

				// Define o modo de camera 1 (rotação de visualização)
				modoCamera = 1;
			}
			
			// Quando liberado
			if(state == GLUT_UP){
				// define os ângulos em que parou após a rotação (theta e phi)
				anguloAtualX += (xMove/500.0)*M_PI;
				anguloAtualY += (yMove/500.0)*M_PI;

				// Seta os valores para "0"
				xMove = 0;
				yMove = 0;

				// Define o modo de camera para "0" (Nada ocorre com a camera)
				modoCamera = 0;
			}
            break;
		// Avalia o pressionamento do botão direito do mouse
        case GLUT_RIGHT_BUTTON: 
			// Quando pressionado
			if (state == GLUT_DOWN){
				// Define yInicial para o valor de y no momento em que foi pressionado
				yInicial = y;

				// Muda o modo de Camera 2 (Zoom de camera)
				modoCamera = 2;
			}
			
			// Quando e liberado
			if(state == GLUT_UP){
				// Define o modo de camera para "0" (Nada ocorre com a camera)
				modoCamera = 0;
			}
            break;
    }
    
}

void renderSystem(){
	// Modo de textura como adição (para que a luz nas cordenadas (0,0,0) não afete na textura do sol e os disco de orbita)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	// cria um objeto Quadric que ira desenhar o disco de orbita
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

	// Muda para o modo de textura para Modulate, fara então uma multiplicação entre a textura e a cor do objeto
	// os parametros de cor variam de 0 a 1 em cada parametro de cor RGB
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
	// Seta algumas variáves que serão usadas inicialmente
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
    glGenTextures(10, texturaId);

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
		// Parâmetro de iluminação ambiente
        GLfloat luzAmbiente[4]={0.03,0.03,0.03,1.0};

		// Cor - variação (0.0 a 1.0) - define como cor da luz "branca"
        GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};

		// Brilho
        GLfloat luzEspecular[4]={0.2, 0.2, 0.2, 1.0};

		// Posição da luz no espaço
        GLfloat posicaoLuz[4]={0.0, 0.0, 0.0, 1.0};
 
        // Capacidade de brilho do material
        GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
        GLint especMaterial = 60;
 
        // Define a reflectância do material 
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
  // lipando a matrix - retirando sujeiras
  glLoadIdentity();

  //setando cor padrão
  glColor3f (1.0, 1.0, 1.0);
  
  // Função que define posição de camera
  pCam();

  // Função que define Iluminação
  DefineIluminacao();

  // Função que desenha os planetas
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
  gluPerspective(60.0, (float)w/(float)h, 0.2, 9000.0);
  //glMatrixMode — especifica qual matriz é a matriz atual
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 1080, 0, 0, 0, 0, 1, 0);
}

// função que faz controle de variaveis dinâmicas
void keyboard (unsigned char key, int x, int y)
{
   switch(key){
	   	// Configuração inicial
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

		// Sol
    	case '0':
			// Lagura do Rastro dos planetas
			tamRastro = 0.1;

			// Distancia de camera
			distCamera = 250;

			// Define a posição do foco de camera para a posição do sol (0,0,0)
			focoCamX = 0.0;
			focoCamY = 0.0;
			focoCamZ = 0.0;

			// Define a posição da orbita do Sol neste caso (0,0) 
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

			// Habilita Mercúrio -
			// A posição de Mercúrio sera o foco da camera
			// assim quando o planeta for abilitado uma sequência de códigos
			// na função "movimentoPlaneta" será execultada
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

			// Habilita Venus -
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

			// Habilita Terra -
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

			// Habilita Marte -
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

			// Habilita Jupiter -
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

			// Habilita Saturno -
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

			// Habilita Urano -
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

			// Habilita Neturno -
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

// Definição do Movimento do planeta (Rotação e Translação)
void movimentoPlaneta(Planeta* planeta){

	// Calcula qual é o ângulo de translação do planeta.
	// cada planeta pecorre sua translação de forma diferente
	// o calculo do passo (ângulo) deve ser em radianos
	// e necessaria a converção e adequação de cada para cada planeta
	// pi/(180*Translação) -> define que a equivalencia de 1 grau de de rotação para cada planeta
	// timerTrans varia 0 a 360*Translação
	// com isso a translação irá 0 até 360 graus mas de forma diferente para cada planeta
	float ang = (*planeta).timerTrans*(M_PI/(180.0*(*planeta).Translacao));

	// Define a posição em X e Z de cada planeta
	(*planeta).posX = (*planeta).TamanhoTranslacao*cos(ang);
	(*planeta).posZ = (*planeta).TamanhoTranslacao*sin(ang);

	// Quando o planeta está abilitado
	if((*planeta).Estado){
		// faz um deslocamento de posição para o ponto em que o planeta se localiza (variáveis de câmera)
		offsetX = (*planeta).posX;
		offsetZ = (*planeta).posZ;

		// Atualiza o foco para posição atual do planeta (variáveis de câmera)
		focoCamX = (*planeta).posX;
		focoCamZ = (*planeta).posZ;
	}

	// Incremento do tempo de translação
	(*planeta).timerTrans++;

	// Verifica se o timerTrans chegou ao valor máximo (a volta completa em torno do sol)
	if((*planeta).timerTrans == 360*(*planeta).TamanhoTranslacao){
		(*planeta).timerTrans = 0;
	}

	// Define qual o ângulo atual de rotação do planeta
	// cada planeta rotaciona de forma diferente
	// a função glRotated(ângulo, x, y, z) os valores assumidos por ângulo devem ser
	// de 0 a 360
	// logo: timerRot ira de 0 até vRotação
	// para assim mapear a rotação de cada planeta 0 a 360
	(*planeta).aRotacao = (*planeta).timerRot*(360.0/(*planeta).vRotacao);

	// Incremeto do tempo de rotação
	(*planeta).timerRot++;

	// Verifica se timerRot chegou ao valor máximo (a volta completa em torno do seu eixo)
	if((*planeta).timerRot == (*planeta).vRotacao){
		(*planeta).timerRot = 0;
	}


}

// Animação
void Timer(int value){
	// Animação dos planetas
	// Mercurio
	movimentoPlaneta(&mercury);

	// Venus
	movimentoPlaneta(&venus);

	// Terra
	movimentoPlaneta(&earth);

	// Marte
	movimentoPlaneta(&mars);

	// Jupiter
	movimentoPlaneta(&jupiter);

	// Saturno
	movimentoPlaneta(&saturn);

	// Urano
	movimentoPlaneta(&uranus);
	
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
