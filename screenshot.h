#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QPixmap>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;
QT_END_NAMESPACE

//! [0]
class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void gameCapture();
    void saveScreenshot();
    void shootScreen();
    void stopGameCapture();

private:
    void updateScreenshotLabel();

    QPixmap originalPixmap;
    QTimer *grabScreenTimer;
    QScreen *screen; 

    QLabel *screenshotLabel;        
    QPushButton *gameCaptureButton;
};
//! [0]

#endif // SCREENSHOT_H