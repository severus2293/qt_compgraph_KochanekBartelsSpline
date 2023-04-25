#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QGLWidget>
#include <vector>
#include <QTime>
//#include "vector3.h"
using namespace std;
class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    double   segments = 10; // кол-во линий для построения сплайна между двумя точками
    double   tension = 0; // длина касательного вектора [-1,1]
    double   continuity = 0; // резкость перехода между касательными [-1,1]
    double   bias = 0; //  направление касательного вектора [-1,1]
    double   width = 1.0; // ширина линий
    vector <double> x; // координаты точек по оси x
    vector <double> y;  // координаты точек по оси y
    vector <double> DSx;
    vector <double> DSy;
    vector <double> DDx;
    vector <double> DDy;
    void clearBuffers();
    explicit GLWidget(QWidget* parent = 0);
    ~GLWidget();
    void clear(); //очистка холста
    void initializeGL() override;
    void paintGL() override; // отрисовка примитивов
    void resizeGL(int w, int h) override;
    void drawSpline();
    HDC hDC;
private:
    QTimer timer;
protected:
    void mousePressEvent(QMouseEvent * event) override; // эвент нажатия мыши по холсту
};

#endif // GLWIDGET_H
