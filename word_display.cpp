#include "word_display.h"
#include "qevent.h"
#include "qpainter.h"
#include "ui_word_display.h"
#include "resource_manager.h"
#include <QGraphicsBlurEffect>
#include <QPainterPath>
#include "Image_processing.h"

word_display::word_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::word_display)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    auto man = resource_manager::getInstance();
    auto path = man->bg_pic_randomselect();
    img = new QImage(path);
    ImageProcesser::GaussiamBlur(100,100,*img);
    animation = new QPropertyAnimation(this, "windowOpacity");
    moveAnimation = new QPropertyAnimation(this, "pos");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    animation->setDuration(1000); // 1秒钟
    animation->setStartValue(0.0); // 完全透明
    animation->setEndValue(1.0); // 完全不透明
    moveAnimation->setDuration(1000);
    paint_cnt = 0;
}

word_display::~word_display()
{
    delete ui;
}

void word_display::set_content(const Word &word)
{
    wd = word;
}

void word_display::enter()
{
    if(!isVisible()){
        animation->start();
        moveAnimation->setStartValue(QPoint(pos().x() - 30, pos().y()));
        moveAnimation->setEndValue(pos());
        moveAnimation->start();
    }
}

void word_display::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect(), 20, 20);
    painter.setClipPath(clipPath);
    painter.drawImage(rect(), *img);
}

QPixmap word_display::screen_shot()
{
    QRect widgetRect = this->geometry();
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QPixmap screenshot = primaryScreen->grabWindow(0,
                                                   widgetRect.x(),
                                                   widgetRect.y(),
                                                   widgetRect.width(),
                                                   widgetRect.height());
    paint_cnt++;
    if(paint_cnt >= 10){
        paint_cnt = 0;
        screenshot.save("save.png");
    }

    qDebug() << "shoted";
    return screenshot;
}


