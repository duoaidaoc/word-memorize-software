#include "word_display.h"
#include "ui_word_display.h"
#include "resource_manager.h"

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
    qDebug() << path;
    this->setStyleSheet(QString("word_display #frame{"
                                "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                "}").arg(path));
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000); // 1秒钟
    animation->setStartValue(0.0); // 完全透明
    animation->setEndValue(1.0); // 完全不透明
    moveAnimation = new QPropertyAnimation(this, "pos");
    moveAnimation->setDuration(1000);
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
