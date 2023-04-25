#include "glwidget.h"
#include <QTextStream>
#include <QMouseEvent>
#include <cmath>
#include <QLabel>
GLWidget::GLWidget(QWidget * parent):QGLWidget(parent) // инициализируем базовые параметры
{
connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
timer.start(100);
}


void GLWidget::clear() //очистить холст
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->paintGL();
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL() //задать базовый цвет для холста
{

    glClearColor(0.2,0.2,0.2,1);
}








void GLWidget::drawSpline(){
    if (segments<=0)
        segments = 1;
            double nextX;
            double nextY;
            double prevX;
            double prevY;
            double point_x;
            double point_y;

            for(int i = 0; i<x.size(); i++) {
                point_x = x[i];
                point_y = y[i];
                glEnable(GL_POINT_SMOOTH);
                glPointSize(10.0);
                glColor3f(1.0,0.0,0.0);
                glBegin(GL_POINTS);

                glVertex2d(x[i],y[i]);
                glEnd();

                //устанавливаем граничные условия для следующей и предыдущей точки
                if (i == 0) {
                    nextX = x[i+1];
                    nextY = x[i+1];
                    prevX = x[i];
                    prevY = y[i];
                }
                else if (i == x.size()-1) {
                    nextX = x[i];
                    nextY = y[i];
                    prevX = x[i-1];
                    prevY = y[i-1];
                }
                else {
                    nextX = x[i+1];
                    nextY = y[i+1];
                    prevX = x[i-1];
                    prevY = y[i-1];
                }
                // уравнения для начальной касательной
                double d0a = ((1-tension)*(1-continuity)*(1+bias))/2.0;
                double d0b = ((1- tension)*(1+ continuity)*(1- bias))/2.0;
                // уравнения для конечной касательной
                double d1a = ((1- tension)*(1+ continuity)*(1+ bias))/2.0;
                double d1b = ((1- tension)*(1- continuity)*(1- bias))/2.0;
                // добавляем начальную касательную
                DSx.push_back(d0a*(point_x-prevX) + d0b*(nextX-point_x));
                DSy.push_back(d0a*(point_y-prevY) + d0b*(nextY-point_y));
                // добавляем конечную касательную
                DDx.push_back(d1a*(point_x-prevX) + d1b*(nextX-point_x));
                DDy.push_back(d1a*(point_y-prevY) + d1b*(nextY-point_y));
            }
            // начинаем отрисовку
            glLineWidth((GLfloat) width); // задаём ширину линий
            glBegin(GL_LINE_STRIP);
                    for(int i = 0; i<x.size()-1; i++) {

                        point_x = x[i];
                        point_y = y[i];
                        nextX = x[i+1];
                        nextY = y[i+1];
                        // получаем начальную касательную
                        double xd0 = DDx[i];
                        double yd0 = DDy[i];
                        // получаем конечную касательную
                        double xd1 = DSx[i+1];
                        double yd1 = DSy[i+1];;
                        double ax, bx, cx, dx;
                        double ay, by, cy, dy;
                        // коэффициенты для x составляющей кубического полинома
                        ax = 2.0*point_x - 2.0*nextX + xd0 + xd1;
                        bx = -3.0*point_x + 3.0*nextX - 2.0*xd0 - xd1;
                        cx = xd0;
                        dx = point_x;
                        // коэффициенты для y составляющей кубического полинома
                        ay = 2.0*point_y - 2.0*nextY + yd0 + yd1;
                        by = -3.0*point_y + 3.0*nextY - 2.0*yd0 - yd1;
                        cy = yd0;
                        dy = point_y;

                        //разбиваем 2 точки на сегменты и отстраиваем каждый сегмент
                        double s=0, s2=0, s3=0;
                        double increment = 1.0/(segments-1);
                        for (int j = 0; j<segments; j++) {
                            // A tiny bit of time saving
                            if (s!=0) {
                                s2 = s*s; // squared
                                s3 = s2*s; // cubed
                            }
                            glColor3f(0,1.0,0.0); // задаём цвет линий
                            glVertex2d((GLfloat)(ax*s3 + bx*s2 + cx*s + dx),
                                       (GLfloat)(ay*s3 + by*s2 + cy*s + dy));
                            s=s+increment;
                        }
                    }
                    glEnd();
                    clearBuffers(); // очищаем векторы касательных для возможности изменить параметры кривой
}

void GLWidget::clearBuffers(){
    DSx.clear();
    DSy.clear();
    DDx.clear();
    DDy.clear();
}

void GLWidget::mousePressEvent(QMouseEvent * event) //событие нажатия на холст для отрисовки точки
{

        // границы окна OpenGL
        double x0 = -1.3;
        double y0 =  1.0;
        double x1 =  1.3;
        double y1 = -1.0;

        // переводим координаты окна в координаты окна OpenGL
        double xAtPress = double(event->x()) / this->geometry().size().width() * (x1 - x0) + x0;
        double yAtPress = double(event->y()) / this->geometry().size().height() * (y1 - y0) + y0;
        x.push_back(xAtPress);
        y.push_back(yAtPress);
        QTextStream out(stdout);

    out << xAtPress << endl;
    out << yAtPress  << endl;

}













void GLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // очистка буфера
    glColor3f(1.0,0,0.0);

    if(x.size() > 0){
       drawSpline();
    }

}

void GLWidget::resizeGL(int w, int h)
{
   glViewport(0,0,w,h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   updateGL();
}
