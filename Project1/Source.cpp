/*
"Disofenix 117"
Diego Esteban Suarez C.
Universidad Militar Nueva Granada
2018
*/
#include <stdlib.h>
#include <math.h>
#include<stdio.h>

#ifdef GL
#include <GL\glut.h>
#else
#include "glut.h"
#endif // GL
/*
PARA DIBUJAR LINEA BRESENHAM SE USA LA TECLA "L".
PARA DIBUJAR CIRCULO CON ANALISIS DE ORDEN 2 SE USA LA LETRA "C".
PARA DIBUJAR UNA ELIPSE CON ANALISIS SE USA LA LETRA "E".
PARA EL RELLENO DE LAS FIGURAS "R".
*/
//variables globales
int H, W;
int Xo, Yo;
int Xf, Yf;
int R;
bool interruptor;
void Inicio(void);
void ConfigureWindow(int newW, int newH);
void OnResizeWindow(int width, int height);
void OnRender(void);

enum DrawTipe//funciones de tipos de variables para las funciones a dibujar
{
	DLine,
	DCircle,
	DElipse,
};

enum Mouse//funciones de tipos de variables para el mouse
{
	MouseElipse,
	MouseCircle,
	MouseLine,
};
DrawTipe draw; //variable que define el tipo de dibujo a realizar
Mouse mouse;//variable que define el tipo de mouse a usar

void Inicio(void)//iniciacion de variables
{
	H = 1000, W = 1000;
	Xo = 0;
	Yo = 0;
	Xf = 0;
	Yf = 0;
	R = 0;
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glMatrixMode(GL_PROJECTION);

} 
//////////////////////////////////////////////////////////////////////////
//FUNCIONES BASICAS PARA OPENGL
//////////////////////////////////////////////////////////////////////////
void OnMouse(int button, int state, int x, int y)//funcion que captura clics
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			Xo = x;
			Yo = y;
		}
	}
}
void DrawCursor(int Xc, int Yc)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	switch (draw)
	{
	case DElipse:
		glBegin(GL_QUADS);
		glVertex2i(Xc - 5, Yc - 5);
		glVertex2i(Xc - 5, Yc + 5);
		glVertex2i(Xc + 5, Yc + 5);
		glVertex2i(Xc + 5, Yc - 5);
		glEnd();
		glBegin(GL_QUADS);
		glVertex2i(Xc - 1, Yc - 1);
		glVertex2i(Xc - 1, Yc + 1);
		glVertex2i(Xc + 1, Yc + 1);
		glVertex2i(Xc + 1, Yc - 1);
		glEnd();
		break;
	case DLine:
		glBegin(GL_LINES);
		glVertex2i(Xc - 5, Yc);
		glVertex2i(Xc + 5, Yc);
		glVertex2i(Xc, Yc - 5);
		glVertex2i(Xc, Yc + 5);
		glEnd();
		break;
	}
}
void OnKeyboard(unsigned char key, int x, int y)// Se configura el modo de graficación en el teclado
{
	switch (key)
	{
	case 'x':
	case 'X':
		exit(0);
		break;
	case 'l':
	case 'L':
		draw = DLine;
		break;
	case 'e':
	case 'E':
		draw = DElipse;
		break;
	case 'c':
	case 'C':
		draw = DCircle;
		break;
	case 'r':
	case 'R':
		interruptor = !interruptor;
		break;

	}

	glutPostRedisplay();
}
void OnMouseMove(int x, int y)//funcion que captura el movimiento del mouse
{
	if (mouse == MouseElipse||mouse==MouseLine)
	{
		Xf = x;
		Yf = y;
	}
	else if (mouse==MouseCircle)
	{
		R = x;
	}
	

	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////
//DIBUJAR LINEAS BASICAS
//////////////////////////////////////////////////////////////////////////
void DibujarLineaHorizontal(int Xi, int Xe, int Y)//ahi bien lo dice; dibuja una linea horizontal
{
	glBegin(GL_POINTS);
	int x;
	for (x = Xi; x <= Xe; x++)
	{
		glVertex2i(x, Y);
	}
	glEnd();
}
//////////////////////////////////////////////////////////////////////////
//SIMETRIAS
//////////////////////////////////////////////////////////////////////////
void simetriaCirculo(int xc, int yc, int x, int y)//simetria para dibujar el resto de los octantes
{
	glBegin(GL_POINTS);
	glVertex2i(x + xc, y + yc);
	glVertex2i(x + xc, -y + yc);
	glVertex2i(-x + xc, y + yc);
	glVertex2i(-x + xc, -y + yc);
	glVertex2i(y + xc, x + yc);
	glVertex2i(y + xc, -x + yc);
	glVertex2i(-y + xc, x + yc);
	glVertex2i(-y + xc, -x + yc);
	glEnd();
}
void simetriaElipse(int xc, int yc, int x, int y)//simetria para dibujar el resto de los cuadrantes
{
	glBegin(GL_POINTS);
	glVertex2i(x + xc, y + yc);
	glVertex2i(x + xc, -y + yc);
	glVertex2i(-x + xc, y + yc);
	glVertex2i(-x + xc, -y + yc);
	glEnd();
}
//////////////////////////////////////////////////////////////////////////
//RELLENOS POR SIMETRIA DE LA FIGURA
//////////////////////////////////////////////////////////////////////////
void rellenoSimetriaCirculo(int x, int y, int xc, int yc)
{
	glBegin(GL_POINTS);
		DibujarLineaHorizontal(-x + xc, x + xc, y + yc);
		DibujarLineaHorizontal(-x + xc, x + xc, -y + yc);
		DibujarLineaHorizontal(-y + xc, y + xc, x + yc);
		DibujarLineaHorizontal(-y + xc, y + xc, -x + yc);

	glEnd();
}
void rellenoSimetriaElipse(int x, int y, int xc, int yc)
{
	glBegin(GL_POINTS);
	DibujarLineaHorizontal(-x + xc, x + xc, y + yc);
	DibujarLineaHorizontal(-x + xc,x + xc, -y + yc);
	glEnd();
}
//////////////////////////////////////////////////////////////////////////
//FUNCIONES DE FIGURAS 
//////////////////////////////////////////////////////////////////////////
void dibujarCirculo()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	int xc = Xo, yc = Yo;
	int x = 0, y = R;
	int D = (5 / 4) - R;
	int inc1 = 5 - 2 * R, inc2 = 3;
	while (x <= y)
	{
		if (D > 0)
		{
			x = x + 1;
			y = y - 1;
			D = D + inc1;
			inc1 = inc1 + 4;
			inc2 = inc2 + 2;
		}
		else
		{
			x = x + 1;
			y = y;
			D = D + inc2;
			inc1 = inc1 + 2;
			inc2 = inc2 + 2;
		}

		simetriaCirculo(xc, yc, x, y);
	}

}
void RellenoCirculo()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	int xc = Xo, yc = Yo;
	int x, y;
	y = R;
	x = 0;
	int D = (5 / 4) - R;
	int inc1 = 5 - 2 * R, inc2 = 3;
	DibujarLineaHorizontal(-R + xc, R + xc, yc); //linea del centro del circulo
	glBegin(GL_POINTS);

	while (x <= y)
	{
		if (D < 0)
		{
			x = x + 1;
			y = y;
			D = D + inc2;
			inc1 = inc1 + 2;
			inc2 = inc2 + 2;
		}
		else
		{
			x = x + 1;
			y = y - 1;
			D = D + inc1;
			inc1 = inc1 + 4;
			inc2 = inc2 + 2;
		}
		rellenoSimetriaCirculo(x, y, xc, yc);
	}
	glEnd();

}
void dibujarElipse() 
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	int xc = Xo, yc = Yo, rx = Xf, ry = Yf;
	int x = 0, y = ry;
	if (rx == 0 || ry == 0)
	{
		return;
	}
	simetriaElipse(xc, yc, x, y);
	int rx2 = rx * rx, ry2 = ry * ry;
	int DosRx2 = 2 * rx2, Dosry2 = 2 * ry2;
	float p1 = ry2 - (rx2*ry) + ((1 / 4)*rx2);
	float p2 = (ry2*(x + 1/2)*(x + 1/2)) + (rx2*(y - 1)*(y - 1)) - rx2 * ry2;
	int px = 0, py = DosRx2 * y;
	while (2 * ry2*x <= 2 * rx2*y)
	{
		simetriaElipse(xc, yc, x, y);
		x++;
		if (p1 < 0) 
		{
			p1 += (2 * x*ry2) + ry2;
		}
		else
		{
			y--;
			p1 += (2 * ry2*x) - (2 * rx2*y) + ry2;

		}
	}
	simetriaElipse(xc, yc, x, y);
	while (y > 0) 
	{

		y--;
		if (p2 > 0) 
		{
			p2 += rx2 - (2 * rx2*y);
		}
		else
		{
			x++;
			p2 += (2 * ry2*x) - (2 * rx2*y) + rx2;
		}
		simetriaElipse(xc, yc, x, y);

	}
	glFlush();

}
void rellenoElipse()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	int xc = Xo, yc = Yo, rx = Xf, ry = Yf;
	int x = 0, y = ry;
	if (rx == 0 || ry == 0)
	{
		return;
	}
	int rx2 = rx * rx, ry2 = ry * ry;
	int DosRx2 = 2 * rx2, Dosry2 = 2 * ry2;
	float p1 = ry2 - (rx2*ry) + ((1 / 4)*rx2);
	float p2 = (ry2*(x + 1 / 2)*(x + 1 / 2)) + (rx2*(y - 1)*(y - 1)) - rx2 * ry2;
	int px = 0, py = DosRx2 * y;
	while (2 * ry2*x <= 2 * rx2*y)
	{

		x++;
		if (p1 < 0)
		{
			p1 += (2 * x*ry2) + ry2;
		}
		else
		{
			y--;
			p1 += (2 * ry2*x) - (2 * rx2*y) + ry2;

		}
		rellenoSimetriaElipse(x, y, xc, yc);
	}
	simetriaElipse(xc, yc, x, y);
	while (y > 0)
	{

		y--;
		if (p2 > 0)
		{
			p2 += rx2 - (2 * rx2*y);
		}
		else
		{
			x++;
			p2 += (2 * ry2*x) - (2 * rx2*y) + rx2;
		}
		rellenoSimetriaElipse(x, y, xc, yc);
	}
	glFlush();

}

void dibujarLineaBresenham()
{
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	/*Recta forma cartesiana y=mx+b
	m=dy/dx
	Recta forma general f(x,y)=Ax+By+C=0
	donde m= -A/B   y  b= -C/B*/
	int dx, dy, dE, dNE, De, Dne, d, D, x, y, f, mv;
	dx = abs(Xf - Xo);
	dy = abs(Yf - Yo);
	//Si (dx > dy) entonces se toma que el negativo será B en lugar de A por ende A= dy, B= -dx y C= -bdx
	//Si (dx < dy) entonces se toma que el negativo será A en lugar de B por ende A= -dy, B= dx y C= bdx
	d = 2 * dy - dx; // Es el dk cuando (dx > dy) = (dy -1/2 dx) se multiplicó x2 para quitar los racionales.
	D = 2 * dx - dy; // Es el dk cuando (dx < dy) = (dx -1/2 dy) se multiplicó x2 para queitar los racionales
	dE = 2 * dy; // dE= dk+1 cuando d<0 y cuando (dx > dy)
	De = 2 * dx; // De= dk+1 cuando d<0 y cuando (dx < dy)
	dNE = 2 * (dy - dx); // dNe = dk+1 cuando d>=0 y cuando (dx > dy)
	Dne = 2 * (dx - dy); //Dne = dk+1 cuando d>=0 y cuando (dx < dy)
	x = Xo;
	y = Yo;
	mv = 1; // Dependiendo del octante en que se encuentre indica si la variable que lleva el punto medio aumenta o disminuye.
	if (dx > dy) // Se ubica desde (pi/4 , 7pi/4) u (3pi/4 , 5pi/4 )
	{  
		if (Xo > Xf)
		{
			x = Xf;
			y = Yf;
			f = Xo; // El punto f siempre se ubicará hacia la derecha por organización y cumplimiento de la condición
					//while dentro de un determinado octante
			if (Yf>Yo)
				mv = -1;
		}
		else 
		{
			x = Xo;
			y = Yo;
			f = Xf;
			if (Yo>Yf)
				mv = -1;
		}
		glVertex2i(x, y);
		while (x < f)
		{
			x++;
			if (d < 0)
				d = d + dE;
			else {
				y = y + mv;
				d = d + dNE;
			}
			glVertex2i(x, y);
		}
	}
	else
	{
		if (Yo > Yf)
		{
			x = Xf;
			y = Yf;
			f = Yo;
			if (Xf>Xo)
				mv = -1;
		}
		else 
		{
			x = Xo;
			y = Yo;
			f = Yf;
			if (Xo>Xf)
				mv = -1;
		}
		glVertex2i(x, y);
		while (y < f)
		{
			y++;
			if (D < 0)
			{
		
			D = D + De;
		}
			else
			{
				x = x + mv;
				D = D + Dne;
			}
			glVertex2i(x, y);
		}
	}
	glEnd();

}

//////////////////////////////////////////////////////////////////////////
//FUNCIONES BASICAS PARA OPENGL
//////////////////////////////////////////////////////////////////////////
void ConfigureWindow(int newW, int newH)//Funcion de configuracion de la ventana de graficacion el puerto de vista

{
	glViewport(0, 0, newW, newH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, newW, newH, 0);
}
void OnResizeWindow(int width, int height)//Funcion cuando la ventana de la GUI cambia de tamaño
{
	ConfigureWindow(width, height);

	W = width;
	H = height;
}
void OnRender(void) //funcion de graficacion
{
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	ConfigureWindow(W, H);
	if (draw == DElipse)
	{
		mouse = MouseElipse;
		
		if (interruptor == true)
		{
			rellenoElipse();
		}
		else
		{
			dibujarElipse();
		}
	}
	
	else if (draw ==DLine)
	{
		mouse = MouseLine;
		dibujarLineaBresenham();
	}
	else if (draw == DCircle)
	{
		mouse = MouseCircle;
		if (interruptor == true)
		{
			RellenoCirculo();
		}
		else
		{
			dibujarCirculo();
		}

	}

	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
//FUNCION PRINCIPAL
//////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	Inicio();
	glutCreateWindow("Disofenix 117 - 2do Corte");
	glutReshapeWindow(W, H);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResizeWindow);
	glutKeyboardFunc(OnKeyboard);
	glutMouseFunc(OnMouse);
	glutMotionFunc(OnMouseMove);
	glutMainLoop();


	return 0;
}