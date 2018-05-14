#include "MyRenderWindow.h"
#include "USSlider.h"
#include "USTestDialog.h"
MyRenderWindow::MyRenderWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi();
}

MyRenderWindow::~MyRenderWindow()
{

}

void MyRenderWindow::setupUi()
{
	
	if (objectName().isEmpty())
		setObjectName(QStringLiteral("MainWindow"));
	resize(800, 600);
	centralwidget = new QWidget();
	centralwidget->setObjectName(QStringLiteral("centralwidget"));
	openGLWidget = new MOpenglWidget(centralwidget);
	openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
	openGLWidget->setGeometry(QRect(20, 20, 721, 501));
	setCentralWidget(centralwidget);
	
	

	fit_all_ = new QAction("Fit ALL",this);
	defferedshading_act_ = new QAction("Deffered Render",this);
	connect(defferedshading_act_, &QAction::triggered, this, &MyRenderWindow::defferedshading_act_slot);
	connect(fit_all_, &QAction::triggered, this, &MyRenderWindow::fitAllSolt);
	menubar = new QMenuBar();
	menubar->setObjectName(QStringLiteral("menubar"));
	menubar->setGeometry(QRect(0, 0, 800, 23));
	setMenuBar(menubar);
	QAction *temp_act = new QAction(nullptr);
	menubar->addAction(temp_act);
	menubar->addAction(fit_all_);
	menubar->addAction(defferedshading_act_);
	temp_act->setText("New_Dialog");
	connect(temp_act, &QAction::triggered, this, &MyRenderWindow::slot_test_dialog);
	statusbar = new QStatusBar();
	statusbar->setObjectName(QStringLiteral("statusbar"));
	setStatusBar(statusbar);
}

void MyRenderWindow::slot_test_dialog()
{
	USTestDialog dlg(this);
	dlg.show();
	if (dlg.exec() == USTestDialog::Accepted)
	{

	}


}

void MyRenderWindow::fitAllSolt()
{
	if (openGLWidget != nullptr)
	{
		openGLWidget->ptr_camera = new MyCamera();
		openGLWidget->update();
	}

}

void MyRenderWindow::defferedshading_act_slot()
{
	
	if ( openGLWidget->renderoperation_ !=nullptr)
	{
		openGLWidget->renderoperation_ = nullptr;
		delete openGLWidget->renderoperation_;
		openGLWidget->renderoperation_ = new MyNormalMapping();
	}
	else
	{
		openGLWidget->renderoperation_ = new MyNormalMapping();
	}
	openGLWidget->update();

}