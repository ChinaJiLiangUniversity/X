#include "MeWidget.h"

#include <QDebug>
#include <QtWidgets\qvboxlayout>
#include <QtWidgets\qhboxlayout>
#include <MeGlWindow.h>
#include <QtWidgets\QSlider>

MeWidget::MeWidget()
{
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	QVBoxLayout* controlsLayout;
	mainLayout->addLayout(controlsLayout = new QVBoxLayout);
	mainLayout->addWidget(meGlWindow = new MeGlWindow(&theModel));

	QHBoxLayout* lightPositionLayout;
	controlsLayout->addLayout(lightPositionLayout = new QHBoxLayout);
	lightPositionLayout->addWidget(lightXSlider = new QSlider);
	lightPositionLayout->addWidget(lightYSlider = new QSlider());
	lightPositionLayout->addWidget(lightZSlider = new QSlider);

	connect(lightXSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(lightYSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(lightZSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));

	sliderValueChanged();
}

void MeWidget::sliderValueChanged()
{
	theModel.lightPosition.x = lightXSlider->value();
	theModel.lightPosition.y = lightYSlider->value();
	theModel.lightPosition.z = lightZSlider->value();
	meGlWindow->repaint();
}