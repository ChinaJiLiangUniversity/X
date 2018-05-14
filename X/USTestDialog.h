#ifndef US_TEST_DIALOG
#define US_TEST_DIALOG

#include <QDialog>
#include <USSlider.h>
#include <QDialogButtonBox>
#include <QLineEdit>

class USTestDialog :public QDialog
{
public:
	USTestDialog(QWidget *parent);
	~USTestDialog();

	QDialogButtonBox *buttonBox;
	USSlinder  *us_slider_;
	QGridLayout *gridLayout;
	QLineEdit *new_line_;

};

#endif // !US_TEST_DIALOG
