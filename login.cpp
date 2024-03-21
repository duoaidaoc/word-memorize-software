#include "login.h"
#include "ui_login.h"
#include <QDir>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QFontDatabase>
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    SetUp();
    actionSet();
    set_font(ui->time_label,30);
    set_font(ui->deco_label,20);
    ui->login_label->setProperty("randomly_update",true);
}

login::~login()
{
    delete ui;
}

void login::SetUp()
{
    // 无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // 透明背景
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(16);

    ui->login_frame->setGraphicsEffect(shadow);
    QString path = "../database_crouse_design/pics/night_bg";
    path = randomselect(path);
    // 设置样式表，将背景图片作为背景
    ui->login_frame->setStyleSheet(QString("#login_frame {"
                                                "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                           "}").arg(path));
    label_change(0);
    ui->login_label->setFixedSize(80, 80);
    ui->close_button->setFixedSize(20, 20);

    // 创建一个 QTimer 定时器，间隔为一秒
    timer = new QTimer(this);
    ui->time_label->setFocusPolicy(Qt::NoFocus);
}

QString login::randomselect(QString path)
{
    QDir dir(path);
    QStringList filters;
    filters << "*.jpg" << "*.png"; // 只筛选 jpg 和 png 格式的图片
    QStringList imageFiles = dir.entryList(filters, QDir::Files);

    // 随机选择一张图片
    if (!imageFiles.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(imageFiles.size());
        path = dir.filePath(imageFiles[randomIndex]);
    }
    return path;
}

void login::label_change(int x){
    if(rand()%100 >= x){
        QString path = "../database_crouse_design/pics/login_label";
        path = randomselect(path);
        ui->login_label->setStyleSheet(QString("#login_label {"
                                               "border-image: url(%1) 0 0 0 0;"
                                               "}").arg(path));
    }
}

void login::set_curr_time()
{
    ui->time_label->setText(QTime::currentTime().toString());
}

void login::set_font(QLabel* obj,int size)
{
    QString strPath = "../database_crouse_design/ttf/No.233-上首怪兽体.ttf";
    QFile dFontFile(strPath);
    if(!dFontFile.open(QIODevice::ReadOnly))
    {
        //说明打开字体文件失败了
        return;
    }

    int nFontId = QFontDatabase::addApplicationFontFromData(dFontFile.readAll());
    if(nFontId == -1)
    {
        //说明加载字体文件失败了，该字体不可用
        return;
    }

    QStringList lFontFamily = QFontDatabase::applicationFontFamilies(nFontId);
    if(lFontFamily.empty())
    {
        //说明从字体中获取字体簇失败了
        return;
    }

    QFont font(lFontFamily.at(0),size);
    obj->setFont(font);
    return ;
}

void login::actionSet()
{
    QObject::connect(ui->close_button, &QPushButton::clicked, [](){
        QApplication::quit(); // 点击按钮时退出应用程序
    });

    int rate = 70;
    connect(ui->acc_edit, &QLineEdit::textChanged, this,[this, rate](){
        label_change(rate);
    });

    QObject::connect(timer, &QTimer::timeout, this, &login::set_curr_time);
    timer->start(1000); // 间隔为一秒
}


