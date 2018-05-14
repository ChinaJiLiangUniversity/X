#include "USSlider.h"


USSlinder::USSlinder(QWidget *parent, int _type /*= 0*/, int _style /*= 0*/)
{
	if ( _style == slide_style::Horizontal)
	{
		this->resize(401, 39);
		gridLayout = new QGridLayout(this);
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		label = new QLabel(this);
		label->setObjectName(QStringLiteral("label"));
		gridLayout->addWidget(label, 0, 0, 1, 1);


		horizontalSlider = new QSlider(this);
		horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
		horizontalSlider->setOrientation(Qt::Horizontal);


		gridLayout->addWidget(horizontalSlider, 0, 1, 1, 1);
		lineEdit = new QLineEdit(this);
		lineEdit->setObjectName(QStringLiteral("lineEdit"));
		gridLayout->addWidget(lineEdit, 0, 2, 1, 1);
		lineEdit->setMaximumSize(QSize(80, 16777215));
	}
	else
	{
		if (this->objectName().isEmpty())
			this->setObjectName(QStringLiteral("Form"));
		this->resize(306, 63);
		gridLayout = new QGridLayout(this);
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		label = new QLabel(this);
		label->setObjectName(QStringLiteral("label"));

		gridLayout->addWidget(label, 0, 0, 1, 1);

		lineEdit = new QLineEdit(this);
		lineEdit->setObjectName(QStringLiteral("lineEdit"));
		lineEdit->setMaximumSize(QSize(80, 16777215));

		gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

		horizontalSlider = new QSlider(this);
		horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
		horizontalSlider->setOrientation(Qt::Horizontal);
		gridLayout->addWidget(horizontalSlider, 1, 0, 1, 2);
	}
}

USSlinder::~USSlinder()
{

}
