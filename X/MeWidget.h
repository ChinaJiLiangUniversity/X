#pragma once
#include <QtWidgets\qwidget.h>
#include <MyModel.h>
class QSlider;
class MeGlWindow;

class MeWidget : public QWidget
{
	Q_OBJECT

	QSlider* lightXSlider;
	QSlider* lightYSlider;
	QSlider* lightZSlider;

	MeGlWindow* meGlWindow;

	MyModel theModel;

private slots:
	void sliderValueChanged();

public:
	MeWidget();
};

