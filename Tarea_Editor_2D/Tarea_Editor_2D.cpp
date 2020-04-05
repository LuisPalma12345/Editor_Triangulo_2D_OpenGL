//============================================================================
//
//          UNIVERSIDAD NACIONAL DEL ALTIPLANO - PUNO
//          DOCTORADO EN CIENCIAS DE LA COMPUTACIÓN
//  
//      ASIGNATURA  : PROCESAMIENTO DE IMAGENES, VISUÓN COMPUTACIONAL
//                    Y COMPUTACIÓN GRÁFICA
//      DOCENTE     : DR. YALMAR PONCE ATENCIO
//      TAREA       : EDITOR DE GRÁFICOS VECTORIALES 2D
//      AUTOR       : LUIS BELTRAN PALMA TTITO
//      FECHA       : 29-MARZO-2020
//
//============================================================================

//============================================================================
//				INSTALACION DE OPENGL VISUAL STUDIO 2019 ES
//============================================================================
// Acceder a Propiedades de proyecto (clic derecho proyecto (árbol archivos))
// 1. Propiedades Proyecto --> Propiedades de configuración
//    --> C/C++ --> General --> Directorio de inclusión adicionales
//    AGREGAR LA CARPETA INCLUDE DE freeglut (Ejm: E:\freeglut\include)
//     (Nota: debe estar habilitado: Configuración: Todas las Config.)
// 2. Propiedades Proyecto --> Propiedades de configuración
//    --> Vinculador --> General --> Directorio de bibliotecas adicionales
//    AGREGAR LA CARPETA LIB DE freeglut (Ejm: E:\freeglut\lib)
//    (Nota: debe estar habilitado: Configuración: Todas las Config.)
// 3. Propiedades Proyecto --> Propiedades de configuración
//    --> Vinculador --> Entrada
//    AGREGAR EL ARCHIVO freglut.lib
//    (Nota: debe estar habilitado: Configuración: Debug)
// 4. Copiar archivo freeglut.dll a la carpeta del proyecto
//    Para acceder a la carpeta: 
//    Clic derecho --  Abrir carpeta en el explorador de archivos
//============================================================================

//============================================================================
//							LIBRERIAS
//============================================================================
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

//============================================================================
//				ESTRUCTURAS DE DATOS y VARIABLES GLOBALES
//============================================================================

//----------------------------------------------------------------------------
// CONTROL DE ANIMACIÓN
// Velocidad de avance de tiempo
double VEL_TIEMPO = 0.1;
// Contador de tiempo
double Tiempo = 0.0;
// Tiempo de animación
double TiempoRotacion, TiempoTraslacion, TiempoEscalado;
// Sentido de giro, avance eje x, eje y, sentido traslacion eje x, eje y, estacado +/-
double Giro, SentidoX, SentidoY, TrasX, TrasY, EscaladoAD;
// Orden de animación Rota, Escala, Traslada
int R, E, T; 

//----------------------------------------------------------------------------
// Ancho y alto de la ventana
int ALTO = 800;
int ANCHO = 1600;

//----------------------------------------------------------------------------
// Estructura de dato Punto 2D:
// x, y: Cooordenda en 2D
// R, G, B: Color del punto en RBG
struct TPunto {
	GLdouble x;
	GLdouble y;
	GLfloat R;
	GLfloat G;
	GLfloat B;
}P1, P2, P3;

//----------------------------------------------------------------------------
// Estructura de dato Triangulo 2D:
// P1, P2, P3: Puntos del triangulo
// BC: Baricentro
// Sel: Bool que indica si esta o no seleccionado
struct TTriangulo {
	TPunto P1;
	TPunto P2;
	TPunto P3;
	TPunto BC;
	bool Sel;
}Triangulo;

//----------------------------------------------------------------------------
// Estructura de dato color RGB:
// R, G, B: Red, Green, Blue
struct TColor {
	GLfloat R;
	GLfloat G;
	GLfloat B;
}ColorSel;

//----------------------------------------------------------------------------
// Número de clic que hace el usuario para dibujar un triangulo
int NroClick = 0;
// Coordenadas hasta las que se dibuja una linea desde el punto clic del mouse
int Cordx, Cordy;
// Coordenadas para mover triángulo con arrastre de mouse
int rx, ry;
// Bandera que indica si debe dibujarse el trazado de la linea al mover el mouse
bool band = false;
// Banderas para saber si se hizo clic en el primer y segundo vectice del triangulo
bool p1b = false, p2b = false;
// Vector de triangulos: registra todos los triangulos trazados
vector<TTriangulo> Triangulos;
// Vector de colores: registra un total de 10 colores
vector<TColor> Colores;
// Opcion de menu seleccionado: D Dibujar, S Seleccionar, R Repintar, Z Zoom, F Fotograma
char Tecla = 'D';
// Opcion de menu numerico: 0 Dibujar, 1 Seleccionar, 2 Repintar, 3 Zoom, 4 Fotograma
int unsigned Opcion = 0;
// Triangulo seleccionado, Toma el valor >= 0 si algun triangulo esta seleccionado
int TriSel = -1;
// Valor que avanza por defecto un triangulo que es trasladado con teclado
int avance = 5;
// Valor que escala un triangulo al se escalado por teclado
double escala = 0.05;
// Factor de Zoom de toda la escena
double FactorEscala = 1.0;
// Angulo de toración por defecto: 1° = Pi/180
double angulo = 3.1416 / 180;
// Menu de opciones
string Mensaje = "<D> DIBUJAR       <S> SELECCIONAR   <R> REPINTAR      <Z> ZOOM          <F> FOTOGRAMA      ";
// Ayuda para manejo del programa: teclas a utilizar para diferentes operaciones
string Msg = "ESCALAR <RePag><AvPag> ROTAR <Inicio><Fin> MOVER <Flechas><Mouse> BORRAR <SUPR> ZOOM <+/-><Flechas> FOTOG <CTRL-L> COLORES < 0 1 2 3 4 5 6 7 8 9 > CANCELAR <ESC>";


//============================================================================
//					INICIALIZA COLORES
//============================================================================
// Inicializa el Vector con 10 colores
// Gris, Rojo, Verde Azul, Celeste, Fucsia, Amarillo, 
// Rojo oscuro, Verde oscuro, Azul Oscuro
//============================================================================
void inline InicializaColores()
{
	TColor C0, C1, C2, C3, C4, C5, C6, C7, C8, C9;
	C0.R = 0.2f; C0.G = 0.2f; C0.B = 0.2f;
	C1.R = 1.0f; C1.G = 0.0f; C1.B = 0.0f;
	C2.R = 0.0f; C2.G = 1.0f; C2.B = 0.0f;
	C3.R = 0.0f; C3.G = 0.0f; C3.B = 1.0f;
	C4.R = 0.0f; C4.G = 1.0f; C4.B = 1.0f;
	C5.R = 1.0f; C5.G = 0.0f; C5.B = 1.0f;
	C6.R = 1.0f; C6.G = 1.0f; C6.B = 0.0f;
	C7.R = 0.5f; C7.G = 0.0f; C7.B = 0.0f;
	C8.R = 0.0f; C8.G = 0.5f; C8.B = 0.0f;
	C9.R = 0.0f; C9.G = 0.0f; C9.B = 0.5f;

	Colores.clear();

	Colores.push_back(C0);
	Colores.push_back(C1);
	Colores.push_back(C2);
	Colores.push_back(C3);
	Colores.push_back(C4);
	Colores.push_back(C5);
	Colores.push_back(C6);
	Colores.push_back(C7);
	Colores.push_back(C8);
	Colores.push_back(C9);

	// Color inicial seleccionado
	ColorSel = C1;
}

//============================================================================
//					CUADRO COLOR
//============================================================================
// Dibuja el recuadro (parte superior izquierda) de color seleccionado, 
// para utilizar en el pintado del triángulo
// El color se selecciona conlas teclas: 0, 1, 2, ... 9
//============================================================================
void CuadroColor() {

	glColor3f(ColorSel.R, ColorSel.G, ColorSel.B);
	glLineWidth(1.0);
	glBegin(GL_QUADS);
	glVertex2d(10, ALTO - 10);
	glVertex2d(10, ALTO - 30);
	glVertex2d(30, ALTO - 30);
	glVertex2d(30, ALTO - 10);
	glEnd();
}

//============================================================================
//					MENU
//============================================================================
// Muestra menu de opciones (2 lineas de texto):
// 1° Linea: Menu Dibuja, Selecciona, Repinta, Zoom, Fotograma 
// 2° Linea: Muestra teclas a utilizar para diferentes operaciones
//============================================================================
void inline Menu()
{
	// MUESTRA MENU DE OPCIONES
	// D Dibujar, S Seleccionar, R Repintar, Z Zoom, F Fotograma
	int Inicio = ANCHO / 2 - (Mensaje.length() / 2 * 10);
	for (int unsigned i = 0; i < Mensaje.length(); i++) {
		if (i >= (17 * Opcion) && i < (17 * Opcion + 17)) glColor3f(0.0f, 0.5f, 0.0f);
		else glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2f(GLfloat(Inicio + (i * 10)), GLfloat(ALTO - 25));
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, Mensaje[i]);
	}

	// DIBUJA RECUADRO DE COLOR COMO FONDO DE LOS NÚMEROS 0, 1, 2, ... 9
	// PARA CONOCER A QUE COLOR CORRESPONDE CADA NUMERO
	int inicio = 1182;
	int ancho = 18;
	for (int unsigned k = 0; k < Colores.size(); k++) {
		glColor3f(Colores[k].R, Colores[k].G, Colores[k].B);
		glLineWidth(1.0);
		glBegin(GL_QUADS);
		glVertex2d(inicio + (k + 1) * ancho, ALTO - 40);
		glVertex2d(inicio + (k + 1) * ancho, ALTO - 60);
		glVertex2d(inicio + (k + 1) * ancho + ancho, ALTO - 60);
		glVertex2d(inicio + (k + 1) * ancho + ancho, ALTO - 40);
		glEnd();
	}

	// MUESTRA TECLAS PARA EJECUTAR OPERACIONES
	bool Abre = false;
	Inicio = ANCHO / 2 - (Msg.length() / 2 * 9);
	for (int unsigned i = 0; i < Msg.length(); i++) {
		if (Msg[i] == '<') Abre = true;
		if (Msg[i] == '>') Abre = false;

		if (i > 123 && i < 140) glColor3f(0.0, 0.0, 0.0);
		else if (Abre) glColor3f(0.0f, 0.5f, 0.5f);
		else if (!Abre)glColor3f(0.2f, 0.2f, 0.2f);

		glRasterPos2f(GLfloat(Inicio + (i * 9)), GLfloat(ALTO - 55));
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, Msg[i]);
	}
}

//============================================================================
//					DIBUJA TRIANGULOS
//============================================================================
// Dibuja los triangulos almacenados en el vector "Triangulos". 
// en la posicion y color, con el que se registra
// El triangulo seleccionados que resaltado con borde blanco
//============================================================================
void DibujaTriangulos() {

	// LIMPIA VENTANA Y DEFINE GROSOR DE LINEA
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1.0);

	// DIBUJA TRIANGULOS
	for (int unsigned i = 0; i < Triangulos.size(); i++) {
		glBegin(GL_TRIANGLES);

		glColor3f(Triangulos[i].P1.R, Triangulos[i].P1.G, Triangulos[i].P1.B); // establece el color
		glVertex2d(Triangulos[i].P1.x, Triangulos[i].P1.y); // dibuja el vertice

		glColor3f(Triangulos[i].P2.R, Triangulos[i].P2.G, Triangulos[i].P2.B);
		glVertex2d(Triangulos[i].P2.x, Triangulos[i].P2.y);

		glColor3f(Triangulos[i].P3.R, Triangulos[i].P3.G, Triangulos[i].P3.B);
		glVertex2d(Triangulos[i].P3.x, Triangulos[i].P3.y);

		glEnd();

		// DIBUJA BORDE DE TRIANGULO SELECCIONADO
		if (Triangulos[i].Sel) {

			glLineWidth(2.0);
			glColor3f(1, 1, 1);

			glBegin(GL_LINES);
			glVertex2d(Triangulos[i].P1.x, Triangulos[i].P1.y);
			glVertex2d(Triangulos[i].P2.x, Triangulos[i].P2.y);
			glEnd();

			glBegin(GL_LINES);
			glVertex2d(Triangulos[i].P1.x, Triangulos[i].P1.y);
			glVertex2d(Triangulos[i].P3.x, Triangulos[i].P3.y);
			glEnd();

			glBegin(GL_LINES);
			glVertex2d(Triangulos[i].P3.x, Triangulos[i].P3.y);
			glVertex2d(Triangulos[i].P2.x, Triangulos[i].P2.y);
			glEnd();

		}
	}

	// DIBUJA PRIMERA LINEA DE TRIANGULO EN CASO DE HABER TRAZADO
	// Si se hizo clic en los 2 primeros vertices del triangulo
	// debe dibujarse el primer lado del triangulo fijo de P1 hasta P2
	if (p1b && p2b) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2d(P1.x, P1.y);
		glVertex2d(P2.x, P2.y);
		glEnd();
	}
	
	// MUESTRA MENU DE OPCIONES Y RECUADRO DE COLOR SELECCIONADO
	Menu();
	CuadroColor();

	// Color de trazado para trazado de nuevos triángulos
	glColor3f(0.0f, 0.0f, 0.0f);

	glFlush();
}

//============================================================================
//					TRASLADA TRIANGULO
//============================================================================
// Traslada el triangulo seleccionado, +/-  (x, y) a la coordenada actual.
// NroTri: Número de triángulo seleccionado
// x, y: valor que deben +/- a la posicion actual del triangulo
// Traslada el triángulo de la posición P a la posicion   P + (x, y)
//============================================================================
void TrasladaTriangulo(int NroTri, double x, double y) {
	if (NroTri >= 0) {

		// Agrega (x, y) a cada vértice del triángulo

		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x + x;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y + y;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x + x;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y + y;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x + x;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y + y;

		// Recalcula centroide
		Triangulos[NroTri].BC.x = (Triangulos[NroTri].P1.x + Triangulos[NroTri].P2.x + Triangulos[NroTri].P3.x) / 3;
		Triangulos[NroTri].BC.y = (Triangulos[NroTri].P1.y + Triangulos[NroTri].P2.y + Triangulos[NroTri].P3.y) / 3;

		DibujaTriangulos();
	}
}


//============================================================================
//					ARRASTRAR MOUSE
//============================================================================
// Evento que se ejecuta al arrastrar el mouse 
// (mover el mouse presionando el botón izquierdo)
// Se utiliza para mover el triángulo seleccionado
// x, y: es la coordenada actual del mouse
// ALGORITMO:
// Para trasladar, se calcula la diferencia de la coordenada anterior 
// del mouse (rx, ry) y la coordenada actual del mouse (x, y).
// NOTA: la diferencia en el eje y es NEGATIVO porque la coordenada (0, 0) 
// de la pantalla esta en el esquinero superior izquierdo, mientras que 
// la coordenada (0, 0) del eje XY imaginario esta en el borde inferior izquierdo
//============================================================================
void ArrastrarMouse(int x, int y)
{
	// Calcula la diferencia entre la posición anterior y actual del mouse
	int difx = (x - rx);
	int dify = -(y - ry);
	// Traslada el triangulo seleccionado
	if (TriSel >= 0) {
		TrasladaTriangulo(TriSel, difx, dify);
		// Actualiza la coordenada del mouse para la siguiente iteracion
		rx = x;
		ry = y;
	}
}

//============================================================================
//					MOVER MOUSE
//============================================================================
// Evento que se ejecuta al mover el mouse sin presionar ningún boton
// Se utiliza para dibujar los lados del triángulo según son trazados
// x, y: Coordenadas de la posición actual del mouse
//============================================================================
void MoverMouse(int x, int y) {
	// Si band == true, entonces debe dibujarse
	if (band == true) {
		Cordx = x;
		Cordy = ALTO - y;

		// Limpia escena
		glClear(GL_COLOR_BUFFER_BIT);
		// dibuja los triangulos existentes 
		// y la primera linea del siguiente triangulo si existe
		DibujaTriangulos();

		// SE HIZO EL PRIMER CLIC (UN VERTICE FIJO)
		// Entoces dibujar el lado1 de P1 hasta la posicion del mouse
		if (p1b && !p2b) {
			glBegin(GL_LINES);
			glVertex2d(P1.x, P1.y);
			glVertex2d(Cordx, Cordy);
			glEnd();
		}

		// SE HIZO EL SEGUNDO CLIC (UN VERTICE FIJO)
		// Entonces trazar el segundo y tercer lado del triangulo
		// 2° lado: de P2 hasta el mouse, 3° lado: de P1 hasta el mouse 
		if (p1b && p2b) {
			// 2° lado: de P2 hasta la posicion del mouse
			glBegin(GL_LINES);
			glVertex2d(P2.x, P2.y);
			glVertex2d(Cordx, Cordy);
			glEnd();
			// 3° lado: de la posicion del mouse hasta P1 
			glBegin(GL_LINES);
			glVertex2d(Cordx, Cordy);
			glVertex2d(P1.x, P1.y);
			glEnd();
		}

		glFlush();
	}
}

//============================================================================
//					ESCALA TRIANGULO
//============================================================================
// Escala un triangulo, multiplicando una constante a cada punto del triánfgulo
// NroTri: Número de tríangulo al que debe aplicarse el escalado
// escala: nuevo valor de la escala: (Ejm: escala = 1.05, incrementa el 5%) 
// ALGORITMO:
// 1. Almacena centroide del triángulo
// 2. Traslada centroide al punto (0, 0) del eje XY
// 3. Escalar el triangulo
// 4. Traslada centroide a la coordenada original
// 5. Recalcula centroide 
//============================================================================
void EscalaTriangulo(int NroTri, double escala) {
	if (NroTri >= 0) {

		// Almacena centroide
		double cx = Triangulos[NroTri].BC.x;
		double cy = Triangulos[NroTri].BC.y;

		// Trasladar el centroide a (0, 0)
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y - Triangulos[NroTri].BC.y;

		// Escala triángulo 
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x * escala;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y * escala;;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x * escala;;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y * escala;;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x * escala;;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y * escala;;

		// Trasladar centroide a la coordenada original
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x + cx;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y + cy;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x + cx;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y + cy;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x + cx;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y + cy;

		// Recalcular centroide
		Triangulos[NroTri].BC.x = (Triangulos[NroTri].P1.x + Triangulos[NroTri].P2.x + Triangulos[NroTri].P3.x) / 3;
		Triangulos[NroTri].BC.y = (Triangulos[NroTri].P1.y + Triangulos[NroTri].P2.y + Triangulos[NroTri].P3.y) / 3;

		DibujaTriangulos();
	}
}

//============================================================================
//					ROTAR TRIANGULO
//============================================================================
// Rota un triangulo un angulo y sentido dado
// angulo: angulo que debe rotarse, expresado en radianes
// NroTri: número de triangulo que debe rotarse
// sentido: sentido de giro (-1 horario) (+1 antihorario)
// ALGORITMO:
// 1. Almacena centroide
// 2. Traslada centroide al punto (0, 0)
// 3. Rotar triangulo
// 4. Traslada a la coordenada original
// 5. Recalcula centroide 
//============================================================================
void RotaTriangulo(double angulo, int NroTri, double sentido) {
	if (NroTri >= 0) {

		// Almacena centroide
		double cx = Triangulos[NroTri].BC.x;
		double cy = Triangulos[NroTri].BC.y;

		// Trasladar centroide a (0, 0)
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y - Triangulos[NroTri].BC.y;

		// Rotar el angulo solicitado aplicando senos y cosenos
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x * cos(angulo) - Triangulos[NroTri].P1.y * sin(angulo) * sentido;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.x * sin(angulo) * sentido + Triangulos[NroTri].P1.y * cos(angulo);

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x * cos(angulo) - Triangulos[NroTri].P2.y * sin(angulo) * sentido;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.x * sin(angulo) * sentido + Triangulos[NroTri].P2.y * cos(angulo);

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x * cos(angulo) - Triangulos[NroTri].P3.y * sin(angulo) * sentido;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.x * sin(angulo) * sentido + Triangulos[NroTri].P3.y * cos(angulo);

		// Trasladar centroide a su coordenada original
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x + cx;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y + cy;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x + cx;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y + cy;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x + cx;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y + cy;

		// Recalcular centroide
		Triangulos[NroTri].BC.x = (Triangulos[NroTri].P1.x + Triangulos[NroTri].P2.x + Triangulos[NroTri].P3.x) / 3;
		Triangulos[NroTri].BC.y = (Triangulos[NroTri].P1.y + Triangulos[NroTri].P2.y + Triangulos[NroTri].P3.y) / 3;

		DibujaTriangulos();
	}
}


//============================================================================
//			SELECCIONA TRIANGULO
//============================================================================
// Selecciona un triángulo al hacer encima
// x,y: coordenadas del mouse
// ALGORITMO:
// Aplica formula de productos vectoriales 
// Se determina las expresiones siguientes
//   (x1 - x4)((y2 - y4) - (x2 - x4)(y1 - y4)
//   (x2 - x4)((y3 - y4) - (x3 - x4)(y2 - y4)
//   (x3 - x4)((y1 - y4) - (x1 - x4)(y3 - y4)
// donde: P1 = (x1,y1), P2 = (x2,y2), P3 = (x3,y3), y POSICION-MOUSE = (x4,y4)
// P1, P2, P3, son los vertices del triangulo y POSICION-MOUSE es el punto 
// que debe verificarse si pertenece o no al triangulo
// 
// Si las expresiones anteriores son del mismo signo, SI PERTENECE
//============================================================================
int SeleccionaTriangulo(int x, int y) {
	y = ALTO - y;
	TriSel = -1;

	// Analiza cada triangulo del vector de final al inicio
	for (int k = Triangulos.size() - 1; k >= 0; k--) {
		// Calcula productos vectoriales
		double oPAB = (Triangulos[k].P1.x - x) * (Triangulos[k].P2.y - y) - (Triangulos[k].P2.x - x) * (Triangulos[k].P1.y - y);
		double oPBC = (Triangulos[k].P2.x - x) * (Triangulos[k].P3.y - y) - (Triangulos[k].P3.x - x) * (Triangulos[k].P2.y - y);
		double oPCA = (Triangulos[k].P3.x - x) * (Triangulos[k].P1.y - y) - (Triangulos[k].P1.x - x) * (Triangulos[k].P3.y - y);
		// Determinar si (x, y) pertenece al triangulo, si es asi 
		// retorna el número de triangulo
		if ((oPAB > 0 && oPBC > 0 && oPCA > 0) || (oPAB < 0 && oPBC < 0 && oPCA < 0))
		{
			if (TriSel == -1) {
				TriSel = k;
				Triangulos[k].Sel = true;
			}
			else
				Triangulos[k].Sel = false;
		}
		else
			Triangulos[k].Sel = false;
	}
	return TriSel;
}

//============================================================================
//			REPINTA VERTICE
//============================================================================
// Repinta el vértice de un triangulo del color seleccionado 
// x, y: coordenadas del mouse
// ALGORITMO:
// Se utiliza la distancia entre 2 puntos
// 1. Verificar si (x, y) pertenece a algún triángulo
// 2. Calcular las siguientes distancias
//    dist(MOUSE, P1), dist(MOUSE, P2), dist(MOUSE, P3)
//    la menor distancia nos indica a que vertice debemos REPINTAR
//    CON EL COLOR SELECCIONADO
//============================================================================
void RepintaVectice(int x, int y) {

	int tri_sel = SeleccionaTriangulo(x, y);
	if (tri_sel >= 0) {
		y = ALTO - y;
		double DP1_XY = sqrt(pow(x - Triangulos[tri_sel].P1.x, 2) + pow(y - Triangulos[tri_sel].P1.y, 2));
		double DP2_XY = sqrt(pow(x - Triangulos[tri_sel].P2.x, 2) + pow(y - Triangulos[tri_sel].P2.y, 2));
		double DP3_XY = sqrt(pow(x - Triangulos[tri_sel].P3.x, 2) + pow(y - Triangulos[tri_sel].P3.y, 2));
		if (DP1_XY <= DP2_XY && DP1_XY <= DP3_XY) {
			Triangulos[tri_sel].P1.R = ColorSel.R;
			Triangulos[tri_sel].P1.G = ColorSel.G;
			Triangulos[tri_sel].P1.B = ColorSel.B;
		}
		else if (DP2_XY <= DP1_XY && DP2_XY <= DP3_XY) {
			Triangulos[tri_sel].P2.R = ColorSel.R;
			Triangulos[tri_sel].P2.G = ColorSel.G;
			Triangulos[tri_sel].P2.B = ColorSel.B;
		}
		else if (DP3_XY <= DP1_XY && DP3_XY <= DP2_XY) {
			Triangulos[tri_sel].P3.R = ColorSel.R;
			Triangulos[tri_sel].P3.G = ColorSel.G;
			Triangulos[tri_sel].P3.B = ColorSel.B;
		}
		// Con esta operacion, un triángulo queda seleccionado, DESACTIVAR
		Triangulos[TriSel].Sel = false;
		TriSel = -1;
	}
}

//============================================================================
//			ELIMINA TRIANGULO
//============================================================================
// Elimina el K-ésimo triangulo del vector
// k: k-ésima posision a eliminar, del vector de triángulos
//============================================================================
void EliminaTriangulo(int k) {
	// Solo borrar si k >= 0
	if (k >= 0) {
		Triangulos.erase(Triangulos.begin() + k);
		DibujaTriangulos();
	}
}

//============================================================================
//			CLIC MOUSE 
//============================================================================
// Evencto clic del mouse
// Se utiliza para las siguientes acciones:
// 1. Almacena la coordenada actual del mouse en caso de mover 
//    un triángulo por arrastre de mouse
// 2. Repintar el vértice un triangulo, si esta activada la opción <R> REPINTAR
// 3. Selecciona un triángulo si esta activada la opcion <S> SELECCIONAR
// 4. Almacena los vértices del triángulo que se este trazando
//    si la opción activada es <D> DIBUJAR
//    Al final del del trazado del triangulo, se almacena los puntos del
//    triangulo, color de cada vértice, baricentro, en el vector de triángulos
//============================================================================
void ClicMouse(int boton, int estado, int x, int y) {

	// MOVER TRIÁNGULO
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN && TriSel >= 0) {
		// Almacena posición actual del mouse, para mover un triangulo
		rx = x;
		ry = y;
	}

	// REPINTA TRIÁNGULO
	if (Tecla == 'R') {
		if (boton == GLUT_LEFT_BUTTON) {
			if (estado == GLUT_DOWN) {
				RepintaVectice(x, y);
				DibujaTriangulos();
			}
		}
	}

	// SELECCIONAR TRIÁNGULO
	if (Tecla == 'S') {
		if (boton == GLUT_LEFT_BUTTON) {
			if (estado == GLUT_DOWN) {
				band = false;
				SeleccionaTriangulo(x, y);
			}
		}
	}

	// ALMANCENA VÉRTICES DE UN TRIANGULO Y ALMACENA TRIANGULO EN EL VECTOR
	if (Tecla == 'D'){
		if (boton == GLUT_LEFT_BUTTON) {
			if (estado == GLUT_UP) {
				// Activar trazado de lineas, si se suelta el boton 
				// izquierdo del mouse: band <-- true
				// variable utilizada mientras se mueve el mouse sin preionar
				band = false;

				// Si se solto el boton izquierdo del mouse y NroClick == 0 entonces 
				// almacenar el primer vertice del triangulo, e incrementar NroClicks
				if (NroClick == 0) {
					NroClick++;
					P1.x = x; P1.y = ALTO - y; p1b = true;
					band = true;
				}
				// Si se solto el boton izquierdo del mouse y NroClick = 1 entonces 
				// almacemar el segundo vertice del triangulo, e incrementar NroClicks
				else if (NroClick == 1) {
					NroClick++;
					P2.x = x; P2.y = ALTO - y; p2b = true;
					band = true;
				}
				// Si se solto el boton izquierdo del mouse y NroClick = 2 entonces 
				// reincializar NroClicks a cero y las banderas a falso, ya que se 
				// termino de dibujar el triángulo
				// LUEGO GRABAR UN NUEVO TIRNAGULO EN EL VECTOR
				else if (NroClick == 2) {
					// Reiniciarlizar Clicks de mouse y
					// Banderas de trazado de lineas en falso
					NroClick = 0;
					band = false; p1b = false; p2b = false;

					// Crear triangulo con P1, P2 y posición actual del mouse
					Triangulo.P1.x = P1.x; Triangulo.P1.y = P1.y;
					Triangulo.P2.x = P2.x; Triangulo.P2.y = P2.y;
					Triangulo.P3.x = x; Triangulo.P3.y = ALTO - y;

					// Color del triangulo, color seleccionado
					Triangulo.P1.B = ColorSel.B; Triangulo.P1.G = ColorSel.G; Triangulo.P1.R = ColorSel.R;
					Triangulo.P2.B = ColorSel.B; Triangulo.P2.G = ColorSel.G; Triangulo.P2.R = ColorSel.R;
					Triangulo.P3.B = ColorSel.B; Triangulo.P3.G = ColorSel.G; Triangulo.P3.R = ColorSel.R;

					// Calcula centroide
					Triangulo.BC.x = (Triangulo.P1.x + Triangulo.P2.x + Triangulo.P3.x) / 3;
					Triangulo.BC.y = (Triangulo.P1.y + Triangulo.P2.y + Triangulo.P3.y) / 3;

					// Establecer como triangulo no seleccionado
					Triangulo.Sel = false;

					// Agregar al vector
					Triangulos.push_back(Triangulo);
				}
			}
		}
	}
	glutPostRedisplay();
}

//============================================================================
//					ZOOM
//============================================================================
// Aumenta o Disminuye el Zoom de toda la escena (todos los triangulos)
// Pos: Posicion en la que se realiza en zoom
//      'M': Mismo sitio
//      'C': centro ventana
//      'I': alinear a la izquierda de la ventana
//      'D': alinear a la derecha de la ventana
//      'A': alinear a la arriba de la ventana
//      'J': alinear a la abajo de la ventana
// ALGORITMO:
// 1. Calcular el centroide de toda la escena
// 2. Mover centroide a (0, 0)
// 3. Escalar escena
// 4. Retornar centroide a la posicion original
// 5. Recalcular centroide de cada primitiva
//============================================================================
void Zoom(int Pos) {

	// CALCULO DEL CENTROIDE DE TODA LA ESCENA
	double MinX = 5000, MinY = 5000, MaxX = 0, MaxY = 0;
	for (int unsigned i = 0; i < Triangulos.size(); i++) {

		if (Triangulos[i].P1.x > MaxX) MaxX = Triangulos[i].P1.x;
		if (Triangulos[i].P2.x > MaxX) MaxX = Triangulos[i].P2.x;
		if (Triangulos[i].P3.x > MaxX) MaxX = Triangulos[i].P3.x;

		if (Triangulos[i].P1.y > MaxY) MaxY = Triangulos[i].P1.y;
		if (Triangulos[i].P2.y > MaxY) MaxY = Triangulos[i].P2.y;
		if (Triangulos[i].P3.y > MaxY) MaxY = Triangulos[i].P3.y;

		if (Triangulos[i].P1.x < MinX) MinX = Triangulos[i].P1.x;
		if (Triangulos[i].P2.x < MinX) MinX = Triangulos[i].P2.x;
		if (Triangulos[i].P3.x < MinX) MinX = Triangulos[i].P3.x;

		if (Triangulos[i].P1.y < MinY) MinY = Triangulos[i].P1.y;
		if (Triangulos[i].P2.y < MinY) MinY = Triangulos[i].P2.y;
		if (Triangulos[i].P3.y < MinY) MinY = Triangulos[i].P3.y;
	}

	double centroX = 0, centroY = 0, NewCentroX = 0, NewCentroY = 0, AnchoX = 0, AltoY = 0;
	
	// Ancho y Alto de la escena
	AnchoX = MaxX - MinX;
	AltoY = MaxY - MinY;

	// Escala la escena al 100% de tamaño de la ventana
	double FactorZoom = 1.0;
	if (AnchoX > AltoY)
		FactorZoom = (ANCHO * 1.0) / AnchoX;
	else
		FactorZoom = (ALTO * 1.0) / AltoY;

	// Centroide de la escena
	centroX = (MaxX - MinX) / 2.0 + MinX;
	centroY = (MaxY - MinY) / 2.0 + MinY;

	// NUEVO CENTRO
	// Escala en el Mismo sitio
	if (Pos == 'M') {
		NewCentroX = centroX;
		NewCentroY = centroY;
	}
	// escala y mueve al centro de la ventana
	else if (Pos == 'C') {
		NewCentroX = ANCHO / 2.0;
		NewCentroY = ALTO / 2.0;
	}
	// escala al 70% de la ventana y mueve a la izquierda - centro de la ventana
	else if (Pos == 'I') {
		FactorEscala = FactorZoom * 0.7;
		NewCentroX = (centroX - MinX);
		NewCentroY = ALTO / 2.0;
	}
	// escala al 70% de la ventana y mueve a la derecha - centro de la ventana
	else if (Pos == 'D') {
		FactorEscala = FactorZoom * 0.7;
		NewCentroX = (ANCHO - centroX + MinX);
		NewCentroY = ALTO / 2.0;
	}
	// escala al 70% de la ventana y mueve a centro y arriba de la ventana
	else if (Pos == 'A') {
		FactorEscala = FactorZoom * 0.7;
		NewCentroX = ANCHO / 2.0;
		NewCentroY = (ALTO - centroY + MinY);
	}
	// escala al 70% de la ventana y mueve a centro y debajo de la ventana
	else if (Pos == 'J') {
		FactorEscala = FactorZoom * 0.7;
		NewCentroX = ANCHO / 2.0;
		NewCentroY = (centroY - MinY);
	}

	// MUEVE CENTROIDE A (0,0)
	for (int unsigned i = 0; i < Triangulos.size(); i++) {
		Triangulos[i].P1.x = Triangulos[i].P1.x - centroX;
		Triangulos[i].P1.y = Triangulos[i].P1.y - centroY;

		Triangulos[i].P2.x = Triangulos[i].P2.x - centroX;
		Triangulos[i].P2.y = Triangulos[i].P2.y - centroY;

		Triangulos[i].P3.x = Triangulos[i].P3.x - centroX;
		Triangulos[i].P3.y = Triangulos[i].P3.y - centroY;
	}

	// ESCALAR ESCENA
	for (int unsigned i = 0; i < Triangulos.size(); i++) {
		Triangulos[i].P1.x = Triangulos[i].P1.x * FactorEscala;
		Triangulos[i].P1.y = Triangulos[i].P1.y * FactorEscala;

		Triangulos[i].P2.x = Triangulos[i].P2.x * FactorEscala;
		Triangulos[i].P2.y = Triangulos[i].P2.y * FactorEscala;

		Triangulos[i].P3.x = Triangulos[i].P3.x * FactorEscala;
		Triangulos[i].P3.y = Triangulos[i].P3.y * FactorEscala;
	}

	// RETORNAR CENTROIDE A POSICION ORIGINAL
	for (int unsigned i = 0; i < Triangulos.size(); i++) {
		Triangulos[i].P1.x = Triangulos[i].P1.x + NewCentroX;
		Triangulos[i].P1.y = Triangulos[i].P1.y + NewCentroY;

		Triangulos[i].P2.x = Triangulos[i].P2.x + NewCentroX;
		Triangulos[i].P2.y = Triangulos[i].P2.y + NewCentroY;

		Triangulos[i].P3.x = Triangulos[i].P3.x + NewCentroX;
		Triangulos[i].P3.y = Triangulos[i].P3.y + NewCentroY;
	}

	// RECALCULAR CENTROIDES DE PRIMITIVAS
	for (int unsigned i = 0; i < Triangulos.size(); i++) {
		Triangulos[i].BC.x = (Triangulos[i].P1.x + Triangulos[i].P2.x + Triangulos[i].P3.x) / 3;
		Triangulos[i].BC.y = (Triangulos[i].P1.y + Triangulos[i].P2.y + Triangulos[i].P3.y) / 3;
	}

	DibujaTriangulos();
}

//============================================================================
//			ANIMACION
//============================================================================
// Realiza animación de fotograma clave (Triangulo Seleccionado)
// Aplica rotación, traslación y escalado
// Los parametros de animación son generador aleatoriamente
//============================================================================
void Animacion(void) {

	Tiempo = Tiempo + VEL_TIEMPO;
	if (TriSel > -1) {

		// Rotacion, Escalado, Traslación
		if (R == 1 && E == 2 && T == 3) {
			if (Tiempo <= TiempoRotacion)
				RotaTriangulo(angulo, TriSel, Giro);
			else if (Tiempo <= (TiempoRotacion + TiempoEscalado))
				EscalaTriangulo(TriSel, 1 + (0.01 * EscaladoAD));
			else if (Tiempo <= (TiempoRotacion + TiempoEscalado + TiempoEscalado))
				EscalaTriangulo(TriSel, 1 - (0.01 * EscaladoAD));
			else if (Tiempo <= (TiempoRotacion + TiempoEscalado + TiempoEscalado + TiempoTraslacion))
				TrasladaTriangulo(TriSel, TrasX * SentidoX, TrasY * SentidoY);
			else if (Tiempo <= (TiempoRotacion + TiempoEscalado + TiempoEscalado + TiempoTraslacion + TiempoTraslacion))
				TrasladaTriangulo(TriSel, TrasX * (-SentidoX), TrasY * (-SentidoY));
		}
		// Rotacion, Traslación, Escalado
		else if (R == 1 && E == 3 && T == 2) {
			if (Tiempo <= TiempoRotacion)
				RotaTriangulo(angulo, TriSel, Giro);
			else if (Tiempo <= (TiempoRotacion + TiempoTraslacion))
				TrasladaTriangulo(TriSel, TrasX * SentidoX, TrasY * SentidoY);
			else if (Tiempo <= (TiempoRotacion + TiempoTraslacion + TiempoTraslacion))
				TrasladaTriangulo(TriSel, TrasX * (-SentidoX), TrasY * (-SentidoY));
			else if (Tiempo <= (TiempoRotacion + TiempoTraslacion + TiempoTraslacion + TiempoEscalado))
				EscalaTriangulo(TriSel, 1 + (0.01 * EscaladoAD));
			else if (Tiempo <= (TiempoRotacion + TiempoTraslacion + TiempoTraslacion + TiempoEscalado + TiempoEscalado))
				EscalaTriangulo(TriSel, 1 - (0.01 * EscaladoAD));
		}
		// Escalado, Rotación, Traslación
		else if (R == 2 && E == 1 && T == 3) {
			if (Tiempo <= TiempoEscalado)
				EscalaTriangulo(TriSel, 1 + (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoEscalado + TiempoEscalado)
				EscalaTriangulo(TriSel, 1 - (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoEscalado + TiempoEscalado + TiempoRotacion)
				RotaTriangulo(angulo, TriSel, Giro);
			else if (Tiempo <= TiempoEscalado + TiempoEscalado + TiempoRotacion + TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * SentidoX, TrasY * SentidoY);
			else if (Tiempo <= TiempoEscalado + TiempoEscalado + TiempoRotacion + TiempoTraslacion + TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * (-SentidoX), TrasY * (-SentidoY));
		}
		// Escalado, Traslación, Rotación 
		else if (R == 3 && E == 1 && T == 2) {
			if (Tiempo <= TiempoEscalado)
				EscalaTriangulo(TriSel, 1 + (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoEscalado + TiempoEscalado)
				EscalaTriangulo(TriSel, 1 - (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoEscalado + TiempoEscalado + TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * SentidoX, TrasY * SentidoY);
			else if (Tiempo <= TiempoEscalado + TiempoEscalado + TiempoTraslacion + TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * (-SentidoX), TrasY * (-SentidoY));
			else if (Tiempo <= TiempoEscalado + TiempoEscalado + TiempoTraslacion + TiempoTraslacion + TiempoRotacion)
				RotaTriangulo(angulo, TriSel, Giro);
		}
		// Traslación, Rotación, Escalado
		else if (R == 2 && E == 3 && T == 1) {
			if (Tiempo <= TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * SentidoX, TrasY * SentidoY);
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * (-SentidoX), TrasY * (-SentidoY));
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion + TiempoRotacion)
				RotaTriangulo(angulo, TriSel, Giro);
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion + TiempoRotacion + TiempoEscalado)
				EscalaTriangulo(TriSel, 1 + (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion + TiempoRotacion + TiempoEscalado + TiempoEscalado)
				EscalaTriangulo(TriSel, 1 - (0.01 * EscaladoAD));
		}
		// Traslación, Escalado, Rotación
		else if (R == 3 && E == 2 && T == 1) {
			if (Tiempo <= TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * SentidoX, TrasY * SentidoY);
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion)
				TrasladaTriangulo(TriSel, TrasX * (-SentidoX), TrasY * (-SentidoY));
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion + TiempoEscalado)
				EscalaTriangulo(TriSel, 1 + (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion + TiempoEscalado + TiempoEscalado)
				EscalaTriangulo(TriSel, 1 - (0.01 * EscaladoAD));
			else if (Tiempo <= TiempoTraslacion + TiempoTraslacion + TiempoEscalado + TiempoEscalado + TiempoRotacion)
				RotaTriangulo(angulo, TriSel, Giro);
		}
	}
}

//============================================================================
//			TECLA ESPECIAL
//============================================================================
// Evento de teclado: pulsación de teclas especiales
// Este método es utilizado con 4 propósitos
// 1. En caso de presionar Flechas del Cursor
//    mover el triángulo seleccionado según la orientación de la flechas
// 2. En caso de prersionar Av.Pág o Ré.Pág se escala el triangulo seleccionado
// 3. En caso de prersionar Inicio o Fin se rota el triángulo seleccionado
//    en sentido horario o antihorario
// 4. Inicializacion de parametros de animación de forma aleatoria
//============================================================================
void TeclaEspecial(int key, int x, int y) {
	switch (key) {

		// Mueve triangulo seleccionado con flechas del cursor
	case GLUT_KEY_RIGHT:
		if (Tecla == 'S') TrasladaTriangulo(TriSel, avance, 0);
		if (Tecla == 'Z') Zoom('D');
		break;
	case GLUT_KEY_LEFT:
		if (Tecla == 'S') TrasladaTriangulo(TriSel, -avance, 0);
		if (Tecla == 'Z') Zoom('I');
		break;
	case GLUT_KEY_UP:
		if (Tecla == 'S') TrasladaTriangulo(TriSel, 0, avance);
		if (Tecla == 'Z') Zoom('A');
		break;
	case GLUT_KEY_DOWN:
		if (Tecla == 'S') TrasladaTriangulo(TriSel, 0, -avance);
		if (Tecla == 'Z') Zoom('J');
		break;

		// Escala triangulo seleccionado
	case GLUT_KEY_PAGE_DOWN:
		if (Tecla == 'S') EscalaTriangulo(TriSel, 1.0 + escala);
		break;
	case GLUT_KEY_PAGE_UP:
		if (Tecla == 'S') EscalaTriangulo(TriSel, 1.0 - escala);
		break;

		// Rota triangulo seleccionado
	case GLUT_KEY_HOME:
		if (Tecla == 'S') RotaTriangulo(angulo, TriSel, 1);
		break;
	case GLUT_KEY_END:
		if (Tecla == 'S') RotaTriangulo(angulo, TriSel, -1);
		break;

		// Inicializa parametros de ANIMACIÓN de fotograma: CTRL-IZQUIERDO
	case GLUT_KEY_CTRL_L:
		if (Tecla == 'F' && TriSel > -1) {
			// Determina tiempo de animación para rotar, trasladar y escalar
			TiempoRotacion = rand() % 200;
			TiempoTraslacion = rand() % 50;
			TiempoEscalado = rand() % 25;
			// Sentido de giro +1 / -1
			Giro = rand() % 2; if (Giro == 0) Giro = -1;
			// sentido de avance en EJE X:  +1 / -1
			SentidoX = rand() % 2; if (SentidoX == 0) SentidoX = -1;
			// sentido de avance en EJE Y:  +1 / -1
			SentidoY = rand() % 2; if (SentidoY == 0) SentidoY = -1;
			// Cantidad de pixeles a avanzar en cada iteración en EJE X
			TrasX = rand() % 5 + 1;
			// Cantidad de pixeles a avanzar en cada iteración en EJE Y
			TrasY = rand() % 5 + 1;
			// Orden escalado AUMENTO o DISMINUCIÓN:  +1 / -1
			EscaladoAD = rand() % 2; if (EscaladoAD == 0) EscaladoAD = -1; 
			// Secuencia de animación: ROTA, ESCALA, TRASLADA
			// R, E, T pertenece {1, 2, 3}
			// Si R = 3, E = 1, T = 2, el orden sera: Escalar, Trasladar, Rotar
			R = rand() % 3 + 1;
			int RET = rand() % 3 + 1;
			while (R == RET) RET = rand() % 3 + 1;
			E = RET;
			RET = rand() % 3 + 1;
			while (R == RET || E == RET) RET = rand() % 3 + 1;
			T = RET;

			// ANIMACIÓN
			while (Tiempo < TiempoRotacion + TiempoEscalado + TiempoEscalado + TiempoTraslacion + TiempoTraslacion)
				Animacion();

			// REINICIAR PARAMETROS DE ANIMACIÓN
			Tiempo = 0.0;
			Triangulos[TriSel].Sel = false;
			TriSel = -1;

			DibujaTriangulos();
		}
	}
}

//============================================================================
//			TECLA NORMAL
//============================================================================
// Evento de pulsación de teclas normales
// Se utiliza para 5 propósitos
// 1. Seleccionar la opcion Dibujar triángulo     TECLA D
// 2. Seleccionar la opcion Seleccionar triángulo TECLA S
// 3. Seleccionar la opcion Repintar triángulo    TECLA R
// 4. Seleccionar la opcion Zoom triángulo        TECLA Z
// 5. Seleccionar la opcion Fotograma triangulo   TECLA F
// 6. Cancela el trazado de triangulo             TECLA ESCAPE
// 7. Borrar el triangulo seleccionado            TECLA SUPRIMIR
// 8. Seleccionar color                           TECLA 0, 1, 2, ... ó 9
// 9. Aumenta el Zoom en 5%                       TECLA +
// 10. Disminuye el Zoom en 5%                    TECLA -
//============================================================================
void TeclaNormal(unsigned char key, int x, int y) {

	// Dibujar triangulo
	if (key == 'd' || key == 'D') {
		Tecla = 'D'; Opcion = 0;
	}
	// Seleccionar triangulo
	else if (key == 's' || key == 'S') {
		Tecla = 'S'; Opcion = 1;
	}
	// Repintado triangulo
	else if (key == 'r' || key == 'R') {
		Tecla = 'R'; Opcion = 2;
	}
	// Zoom de escena
	else if (key == 'z' || key == 'Z') {
		Tecla = 'Z'; Opcion = 3;
	}
	// Animación de Fotograma
	else if (key == 'f' || key == 'F') {
		Tecla = 'F'; Opcion = 4;
	}
	// Deshace acciones de trazado de triangulo
	else if (key == 27) {
		Tecla = 'D'; Opcion = 0; TriSel = -1;
		P1.x = x; P1.y = 0;
		P2.x = x; P2.y = 0;
		band = false; p1b = false; p2b = false;
	}
	// Borrar triangulo tecla <SUPRIMIR>
	else if (key == 127) {
		EliminaTriangulo(TriSel);
		Tecla = 'S'; Opcion = 1;
		TriSel = -1;
	}
	// Teclas de números del 0 al 9 (Selecciona color)
	else if (key >= 48 && key <= 57) {
		ColorSel = Colores[(int)(key - 48)];
	}
	// Aumenta zoom 5% 
	else if (key == '+') {
		FactorEscala = 1.05;
		Zoom('C');
	}
	// Disminuye zoom 5% 
	else if (key == '-') {
		FactorEscala = 0.95;
		Zoom('C');
	}
	// Volver a mostrar ventana, para repintar
	glutPostRedisplay();
}


//============================================================================
//			INICIALIZA GLUT
//============================================================================
// Inicializa Glut 
//============================================================================
void InicializaGlut() {

	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ANCHO, 0, ALTO);
}

//============================================================================
//			MAIN
//============================================================================
// Programa principal
// Posee la lógica del progama principal
//============================================================================
void main(int argc, char** argv) {

	// Inicializa vector de colores de pintado de triángulos
	InicializaColores();

	// Inicializa entorno de ventana
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(ANCHO, ALTO);
	glutInitWindowPosition(0, 0);

	// Crea ventana 
	glutCreateWindow("  EDITOR DE GRAFICOS VECTORIALES 2D               ( LUIS  BELTRAN  PALMA  TTITO )               DOCTORADO EN CIENCIAS DE LA COMPUTACION               UNA-PUNO");
	// Lamada a eventos
	InicializaGlut();
	glutMouseFunc(ClicMouse);
	glutPassiveMotionFunc(MoverMouse);
	glutMotionFunc(ArrastrarMouse);
	glutSpecialFunc(TeclaEspecial);
	glutKeyboardFunc(TeclaNormal);
	glutDisplayFunc(DibujaTriangulos);
	// Entra en un bucle de eventos
	glutMainLoop();
}