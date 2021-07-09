#include <QtWidgets>

#include "screenshot.h"

Screenshot::Screenshot() :  screenshotLabel(new QLabel(this))
{
    screen = QGuiApplication::primaryScreen();

    grabScreenTimer = new QTimer(this);
    connect(grabScreenTimer, &QTimer::timeout, this, &Screenshot::shootScreen);

    screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    screenshotLabel->setMinimumSize(screenGeometry.width() / 8, screenGeometry.height() / 8);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(screenshotLabel);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    
    gameCaptureButton = new QPushButton(tr("Start"), this);
    connect(gameCaptureButton, &QPushButton::clicked, this, &Screenshot::gameCapture);
    buttonsLayout->addWidget(gameCaptureButton);

    QPushButton *saveScreenshotButton = new QPushButton(tr("Save Screenshot"), this);
    connect(saveScreenshotButton, &QPushButton::clicked, this, &Screenshot::saveScreenshot);
    buttonsLayout->addWidget(saveScreenshotButton);

    QPushButton *stopGameCapture = new QPushButton(tr("Stop"), this);
    stopGameCapture->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(stopGameCapture, &QPushButton::clicked, this, &Screenshot::stopGameCapture);

    buttonsLayout->addWidget(stopGameCapture);
    buttonsLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    shootScreen();

    setWindowTitle(tr("TAS"));
    resize(300, 200);
}

void Screenshot::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = originalPixmap.size();
    scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
    if (!screenshotLabel->pixmap() || scaledSize != screenshotLabel->pixmap()->size())
        updateScreenshotLabel();
}

void Screenshot::gameCapture()
{
    gameCaptureButton->setDisabled(true);
    
    //Place this window on the screen that your game is running on
    //then click capture and move the window to a different screen.
    //During the click the screen is chosen and remembered
    screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();    

    grabScreenTimer->start( 250 );
}

void Screenshot::saveScreenshot()
{
    const QString format = "png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);
    QStringList mimeTypes;
    
    foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
        mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fileName = fileDialog.selectedFiles().first();
    if (!originalPixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}

void Screenshot::shootScreen()
{
    //if no screen handle is found then do nothing!
    if (!screen)
        return;

    originalPixmap = screen->grabWindow(0);
    
    //Game logic processing
    gameLogic(&originalPixmap);

    updateScreenshotLabel();
}

void Screenshot::stopGameCapture(){
    gameCaptureButton->setDisabled(false);
    grabScreenTimer->stop();
}

void Screenshot::updateScreenshotLabel()
{
   screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(), Qt::KeepAspectRatio,  Qt::SmoothTransformation));
}
