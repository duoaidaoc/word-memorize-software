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
#include "resource_manager.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    SetUp();
    actionSet();
    auto man = resource_manager::getInstance();
    auto glob_font = man->get_glob_font();
    glob_font.setPointSize(30);
    ui->time_label->setFont(glob_font);

    glob_font.setPointSize(20);
    ui->deco_label->setFont(glob_font);

    glob_font.setPointSize(20);
    ui->hello_label->setFont(glob_font);
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
    auto man = resource_manager::getInstance();
    auto path = man->bg_pic_randomselect();

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
    ui->hello_label->setText(man->get_glob_hello());
}
void login::label_change(int x){
    if(rand()%100 >= x){
        auto man = resource_manager::getInstance();
        label_path = man->pf_pic_randomselect();
        ui->login_label->setStyleSheet(QString("#login_label {"
                                               "border-image: url(%1) 0 0 0 0;"
                                               "}").arg(label_path));
    }
}

void login::set_curr_time()
{
    ui->time_label->setText(QTime::currentTime().toString());
}

void login::actionSet()
{
    QObject::connect(ui->close_button, &QPushButton::clicked, [](){
        QApplication::quit(); // 点击按钮时退出应用程序
    });
    QObject::connect(ui->reg_button, &QPushButton::clicked, this , [&](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    QObject::connect(ui->ret_button, &QPushButton::clicked, this , [&](){
        ui->stackedWidget->setCurrentIndex(0);
    });

    int rate = 70;
    connect(ui->acc_edit, &QLineEdit::textChanged, this,[this, rate](){
        label_change(rate);
    });

    QObject::connect(timer, &QTimer::timeout, this, &login::set_curr_time);
    timer->start(1000); // 间隔为一秒
}


