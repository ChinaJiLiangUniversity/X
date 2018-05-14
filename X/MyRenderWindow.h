#ifndef X_H
#define X_H


#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "MOpenglWidget.h"
#include "DeferredShading.h"
#include "MyNormalMapping.h"
#include "MybaseRenderOp.h"
class MyRenderWindow : public QMainWindow
{
	Q_OBJECT

public:
	MyRenderWindow(QWidget *parent = 0);
	~MyRenderWindow();
	void setupUi();
	void slot_test_dialog();
	QWidget *centralwidget;
	MOpenglWidget *openGLWidget;
	QMenuBar *menubar;
	QStatusBar *statusbar;
	QAction *fit_all_;  //   ∫œÀ˘”–
	QAction *defferedshading_act_; 
private:
	
public slots:
	void fitAllSolt();
    void defferedshading_act_slot();
};

#endif // X_H
