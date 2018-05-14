#ifndef US_SLIDER_H
#define US_SLIDER_H
#include <QSlider>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
class USSlinder : public QWidget
{
	Q_OBJECT
	enum slide_style
	{
		Horizontal,
		Vertical,
	};
public:
	USSlinder(QWidget *parent, int _type = 0, int _style = 0);
	~USSlinder();
	public:
		QLabel *label;
		QSlider *horizontalSlider;
		QLineEdit *lineEdit;
		QGridLayout *gridLayout;
};


#endif