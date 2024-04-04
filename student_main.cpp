#include "student_main.h"
#include "ui_student_main.h"
#include <QGraphicsDropShadowEffect>
#include "resource_manager.h"


student_main::student_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::student_main)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    Word wd = {"abandon","abandon.txt","v. 抛弃、放弃"};
    setup();
    setaction();
    addword(wd);
}

student_main::~student_main()
{
    delete ui;
}

void student_main::setup()
{
    layout = new QVBoxLayout(ui->Word_Contents);
    layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    // 无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // 透明背景
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    auto man = resource_manager::getInstance();
    auto path = man->bg_pic_randomselect();
    ui->student_frame->setStyleSheet(QString("#student_frame {"
                                           "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                           "}").arg(path));
    v_line = new QFrame(ui->student_frame);
    v_line->setGeometry(0,ui->stackedWidget->y(),ui->student_frame->width(),3);
    v_line->setStyleSheet("background-color: rgba(255,255,255,0.6);");
    h_line = new QFrame(ui->student_frame);
    h_line->setGeometry(ui->stackedWidget->x() + ui->stackedWidget->width(),0,3,ui->student_frame->height());
    h_line->setStyleSheet("background-color: rgba(255,255,255,0.6);");

    ui->lexicon_label->setStyleSheet(QString("#lexicon_label {"
                                             "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                             "}").arg("../database_crouse_design/pics/tmp.png"));
}

void student_main::setaction()
{
    QObject::connect(ui->learn_btn,&QPushButton::clicked,[&](){
        ui->stackedWidget_2->setCurrentIndex(1);
    });
    QObject::connect(ui->close_button,&QPushButton::clicked,[&](){
        QApplication::quit();
    });
    QObject::connect(ui->min_button,&QPushButton::clicked,[&](){
        this->showMinimized();
    });
    QObject::connect(ui->up_btn,&QPushButton::clicked,[&](){
        ui->stackedWidget_3->setCurrentIndex(ui->stackedWidget_3->currentIndex() ^ 1);
    });
    QObject::connect(ui->down_btn,&QPushButton::clicked,[&](){
        ui->stackedWidget_3->setCurrentIndex(ui->stackedWidget_3->currentIndex() ^ 1);
    });
}

void student_main::addword(const Word &word)
{
    for(int i = 1;i<=10;i++){
        word_frame* wd = new word_frame(ui->Word_Contents);
        wd->set_content(word);
        layout->addWidget(wd);
    }
}

void student_main::setfollower(QWidget *f)
{
    follower = f;
    movefollower();
}

void student_main::movefollower()
{
    if(!follower->isVisible()){
      QPoint p = pos() + ui->student_frame->pos();
      follower->move(p.x() + ui->student_frame->width() + 10,p.y());
      //follower->move(pos());
    }
}
