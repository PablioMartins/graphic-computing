//escala
// http://www.if.ufrgs.br/oei/cgu/sca/sca.htm

#include <stdbool.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>


//Estruturas
typedef struct{
	float Translacao;
	float Rotacao;
	int Faces;
	int TamanhoTranslacao;
	bool Estado;
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


void setup(){
	
  
  /*Configurações dos Planetas*/
  //SOL

	sun.Translacao = 0;
	sun.Faces = 50;
	sun.Estado = true;
	sun.Rotacao = 0;

	//MERCURIO

	mercury.Translacao = 125;
	mercury.Rotacao = 10;
	mercury.TamanhoTranslacao = 6900/escala_orbita;
	mercury.Faces = 50;
	mercury.Estado = true;

	//VENUS

	venus.Translacao = 30;
	venus.Rotacao = 10;
	venus.TamanhoTranslacao = 12800/escala_orbita;
	venus.Faces = 50;
	venus.Estado = true;

	//TERRA

	earth.Translacao = 90;
	earth.Rotacao = 90;
	earth.TamanhoTranslacao = 17700/escala_orbita;
	earth.Faces = 50;
	earth.Estado = true;

	//MARTE

	mars.Translacao = 10;
	mars.Rotacao = 10;
	mars.TamanhoTranslacao = 27000/escala_orbita;
	mars.Faces = 50;
	mars.Estado = true;

	//JUPITER

	jupiter.Translacao = 78;
	jupiter.Rotacao = 10;
	jupiter.TamanhoTranslacao = 92300/escala_orbita;
	jupiter.Faces = 50;
	jupiter.Estado = true;

	//URANO
	uranus.Translacao = 10;
	uranus.Rotacao = 10;
	uranus.TamanhoTranslacao = 340400/escala_orbita;
	uranus.Faces = 50;
	uranus.Estado = true;

	//NETUNO
	neptune.Translacao = 10;
	neptune.Rotacao = 90;
	neptune.TamanhoTranslacao = 533300/escala_orbita;
	neptune.Faces = 50;
	neptune.Estado = true;

	//Saturno
	saturn.Translacao = 10;
	saturn.Rotacao = 10;
	saturn.TamanhoTranslacao = 169300/escala_orbita;
	saturn.Faces = 50;
	saturn.Estado = true;
}

void cam(){
	camera.X = 800 * sin(anguloCameraA) * cos(anguloCameraB);
    camera.Y = 800 * sin(anguloCameraA);
    camera.Z = 800 * cos(anguloCameraA) * sin(anguloCameraB);
    switch(modoCamera){
    	case 1:
          // muda a modo de visão da câmera
          // gluLookAt cria uma matriz de visualização derivada de um ponto de olho, um ponto de referência indicando o centro da cena e um vetor UP.
          // A matriz mapeia o ponto de referência para o eixo z negativo e o ponto do olho para a origem. 
          //Quando uma matriz de projeção típica é usada, o centro da cena é mapeado para o centro da viewport. Da mesma forma, a direção descrita pelo vetor UP projetado no plano de visualização é mapeada para o eixo y positivo para que aponte para cima na janela de visualização. O vetor UP não deve ser paralelo à linha de visão do ponto do olho até o ponto de referência.
          //dinâmico 
          gluLookAt(cursor.X+camera.X, camera.Y, cursor.Z+camera.Z, cursor.X+0, 0, cursor.Z+0, 0, 1, 0);
    	break;
    	case 2:
      //estático
    		gluLookAt(0, 0, 1080, 0, 0, 0, 0, 1, 0);
    	break;
    }
}



void RenderModel(){
	glEnable(GL_TEXTURE_2D);
		
      // inicia bufer de matrix, para coloca no topo o que vai ser alterado
    	glPushMatrix();
	        // realiza a rotação de translação do planeta
          glRotatef(sun.Translacao, 0, 1, 0);
          // realiza rotação em torno do seu eixo
	        glRotatef(90, 1, 0, 0);
          // rendere a esfera no lugar informado raio,  
          //glutSolidSphere e glutWireSphere renderizam uma esfera sólida ou de estrutura de arame, respectivamente.
          //void glutWireSphere(GLdouble radius,
                    //fatias GLint, pilhas GLint);
          glutWireSphere(16.51* escala_raio_planet , sun.Faces, sun.Faces);
        // retira do bufer do topo o obe=jeto a ser modificado   		 
        glPopMatrix();


		//Mercurio
		
		glPushMatrix();
	        glRotatef(mercury.Translacao, 0, 1, 0);
	        glTranslatef(mercury.TamanhoTranslacao, 0.0, 0);
	        glRotatef(mercury.Rotacao, 0, 1, 0);
	        glRotatef(97, 1, 0, 0); 
          glutWireSphere(0.058 * horizonteEventos*escala_raio_planet, mercury.Faces, mercury.Faces);    /* draw smaller planet */
	        
	    glPopMatrix();

      /*Orbita dos Planetas*/
      // cria um objeto disco
      GLUquadric *disc_mercury;
      disc_mercury = gluNewQuadric(); 

      glPushMatrix();
        //aplica a rotação em torno do seu eixo
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        // aplica trasnlação para centro 
        glTranslatef(0, 0, 0);
        // gluDisk — desenha um disco
        // O gluDisk renderiza um disco no plano z = 0. 
        // O disco tem um raio externo e contém um furo circular concêntrico com um raio interno. 
        // Se internal for 0, nenhum furo será gerado.
        //  O disco é subdividido em torno do eixo z em fatias (como fatias de pizza) e também em torno do eixo z em anéis (conforme especificado por fatias e loops, respectivamente).
        gluDisk(disc_mercury, mercury.TamanhoTranslacao, mercury.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      // delete objeto para não ocupar memoria desnecessario
      gluDeleteQuadric(disc_mercury);
	    //Venus  
	    glPushMatrix();
	        glRotatef(venus.Translacao, 0, 1, 0);
	        glTranslatef(venus.TamanhoTranslacao, 0.0, 0);
	        glRotatef(venus.Rotacao, 0, 1, 0);
	        glRotatef(-267, 1, 0, 0);       
          glutWireSphere(00.14 * horizonteEventos*escala_raio_planet, venus.Faces, venus.Faces); 
	        
	    glPopMatrix();

      GLUquadric *disk_venus;
      disk_venus = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disk_venus, venus.TamanhoTranslacao, mercury.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disk_venus);
	    //Terra
	    glPushMatrix();
	        glRotatef(earth.Translacao, 0, 1, 0);
	        glTranslatef(earth.TamanhoTranslacao, 0.0, 0);
	        glRotatef(earth.Rotacao, 0, 1, 0);
	        glRotatef(-133.5, 1, 0, 0);        
          glutWireSphere(0.15 * horizonteEventos*escala_raio_planet, earth.Faces, earth.Faces); 
	        
	    glPopMatrix();
      GLUquadric *disk_earth;
      disk_earth = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disk_earth, earth.TamanhoTranslacao, earth.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disk_earth);
	    //Marte  
	    glPushMatrix();
	        glRotatef(mars.Translacao, 0, 1, 0);
	        glTranslatef(mars.TamanhoTranslacao, 0.0, 0);
	        glRotatef(mars.Rotacao, 0, 1, 0);
	        glRotatef(115, 1, 0, 0);        
	        glutWireSphere(0.08 * horizonteEventos*escala_raio_planet, mars.Faces, mars.Faces); 
          
	    glPopMatrix();

      GLUquadric *disk_mars;
      disk_mars = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disk_mars, mars.TamanhoTranslacao, mars.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disk_mars);
	    //Jupiter
	    glPushMatrix();
	        glRotatef(jupiter.Translacao, 0, 1, 0);
	        glTranslatef(jupiter.TamanhoTranslacao, 0.0, 0);
	        glRotatef(jupiter.Rotacao, 0, 1, 0);
	        glRotatef(93, 1, 0, 0);        
	        glutWireSphere(1.7 * horizonteEventos* escala_raio_planet, jupiter.Faces, jupiter.Faces); 
          
	    glPopMatrix();

      GLUquadric *disk_jupiter;
      disk_jupiter = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disk_jupiter, jupiter.TamanhoTranslacao, jupiter.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disk_jupiter);
	    //Urano
	    glPushMatrix();
	        glRotatef(uranus.Translacao, 0, 1, 0);
	        glTranslatef(uranus.TamanhoTranslacao, 0.0, 0);
	        glRotatef(uranus.Rotacao, 0, 1, 0);
	        glRotatef(-188, 1, 0, 0);        
	        glutWireSphere(0.6 * horizonteEventos*escala_raio_planet, uranus.Faces, uranus.Faces); 
          
	    glPopMatrix();

      GLUquadric *disku_ranus;
      disku_ranus = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disku_ranus, uranus.TamanhoTranslacao, uranus.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disku_ranus);
	    //Netuno 
	    glPushMatrix();
	        glRotatef(neptune.Translacao, 0, 1, 0);
	        glTranslatef(neptune.TamanhoTranslacao, 0.0, 0);
	        glRotatef(neptune.Rotacao, 0, 1, 0);
	        glRotatef(120, 1, 0, 0);        
	        glutWireSphere(0.6 * horizonteEventos *escala_raio_planet, neptune.Faces, neptune.Faces); 
          
	    glPopMatrix();

      GLUquadric *disk_neptune;
      disk_neptune = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disk_neptune, neptune.TamanhoTranslacao, neptune.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disk_neptune);
	    //Saturno  
	    glPushMatrix();
	        glRotatef(saturn.Translacao, 0, 1, 0);
	        glTranslatef(saturn.TamanhoTranslacao, 0.0, 0);
	        glRotatef(saturn.Rotacao, 0, 1, 0);
	        glRotatef(117, 1, 0, 0);        
	        glutWireSphere(1.4 * horizonteEventos*escala_raio_planet, saturn.Faces, saturn.Faces); 
          
	    glPopMatrix();

      GLUquadric *disk_saturn;
      disk_saturn = gluNewQuadric(); 

      glPushMatrix();
    
        glRotatef(0, 0, -1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0, 0);
        gluDisk(disk_saturn, saturn.TamanhoTranslacao, saturn.TamanhoTranslacao+0.5, 600, 600);
      glPopMatrix();
      gluDeleteQuadric(disk_saturn);
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

void init(void) 
{
  // limpa bufer de cor
  glClearColor (0.0, 0.0, 0.0, 0.0);
  // glShadeModel — selecione sombreamento plano ou suave
  // As primitivas GL podem ter sombreamento plano ou suave. O sombreamento suave, 
  // o padrão, faz com que as cores computadas dos vértices sejam interpoladas
  //  à medida que o primitivo é rasterizado, normalmente atribuindo cores diferentes a cada fragmento de pixel resultante. 
  // O sombreamento plano seleciona a cor calculada de apenas um vértice e a atribui a todos os fragmentos de pixel gerados pela rasterização de uma única primitiva.
  //  Em ambos os casos, a cor calculada de um vértice é o resultado da iluminação se a iluminação estiver ativada ou é a cor atual no momento em que o vértice foi especificado se a iluminação estiver desativada
  glShadeModel (GL_FLAT);
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
  cam();
  RenderModel();

  
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
  gluPerspective(60.0, (float)w/(float)h, 0.2, 2147483647.0);
  //glMatrixMode — especifica qual matriz é a matriz atual
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 1080, 0, 0, 0, 0, 1, 0);
}
// função que faz controle de variaveis dinâmicas
void keyboard (unsigned char key, int x, int y)
{
   switch(key){
		case 'e':
			if(horizonteEventos == 1){
				horizonteEventos = 10;
			} else{
				horizonteEventos = 1;
			}
    //glutPostRedisplay marca a janela atual como precisando ser exibida novamente.
    // renderiza a tela para aplicar as mudanças 
    glutPostRedisplay();
		break;
		case 'E':
			if(horizonteEventos == 1){
				horizonteEventos = 10;
			} else{
				horizonteEventos = 1;
			}
    glutPostRedisplay();
		break;
    case '1':
			modoCamera = 1;
    glutPostRedisplay();
		break;
		case '2':
			modoCamera = 2;
    glutPostRedisplay();
		break;
		case 27:
			exit(0);
    glutPostRedisplay();
		break;
		case 'w':
			cursor.X--;
    glutPostRedisplay();
		break;
		case 'W':
			cursor.X--;
    glutPostRedisplay();
		break;
		case 's':
			cursor.X++;
    glutPostRedisplay();
		break;
		case 'S':
			cursor.X++;
    glutPostRedisplay();
		break;
		case 'a':
			cursor.Z--;
    glutPostRedisplay();
		break;
		case 'A':
			cursor.Z--;
    glutPostRedisplay();
		break;
		case 'd':
			cursor.Z++;
    glutPostRedisplay();
		break;
		case 'D':
			cursor.Z++;
    glutPostRedisplay();
		break;
	}
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
  glutMainLoop();
  return 0;
}
