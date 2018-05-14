#include "USTestDialog.h"

USTestDialog::USTestDialog(QWidget *parent)
{
	if (this->objectName().isEmpty())
		this->setObjectName(QStringLiteral("Dialog"));
	this->resize(400, 300);
	gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QStringLiteral("gridLayout"));
	us_slider_ = new USSlinder(this,1,1);
	

	gridLayout->addWidget(us_slider_, 0, 0, 1, 1);

	buttonBox = new QDialogButtonBox(this);
	buttonBox->setObjectName(QStringLiteral("buttonBox"));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

	gridLayout->addWidget(buttonBox, 1, 0, 1, 1);



}

USTestDialog::~USTestDialog()
{

}
