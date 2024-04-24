#include "student_main.h"
#include "ui_student_main.h"
#include <QGraphicsDropShadowEffect>
#include "resource_manager.h"
#include "word_display.h"



student_main::student_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::student_main)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    Word wd = {"abandon","abandon.txt ","v. 抛弃、放弃"};
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
                                             "}").arg("../word-memorize-software/pics/tmp.png"));
    display = new word_display();
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
        word_frames.push_back(wd);
        wd->set_content(word, word_frames.size() - 1);
        layout->addWidget(wd);
        QObject::connect(wd, &word_frame::set_display_content, this, &student_main::show_display);
        QObject::connect(display, &word_display::upd_page, this, &student_main::update_display);
    }
}

void student_main::update_display(int seq_)
{
    int n = word_frames.size();
    seq_ = (seq_ + n) % n;
    display->set_content(word_frames[seq_]->get_content(), seq_);
}

void student_main::show_display(const Word &wd,int seq)
{
    if(seq < 0){
        throw std::runtime_error("seq isn't initialized.");
    }

    if(!display->isVisible()){
        movedisplay();
    }
    display->set_content(wd, seq);
    display->enter();
    display->show();

}

void student_main::movedisplay()
{
    QPoint p = pos() + ui->student_frame->pos();
    display->set_nexpos({p.x() + ui->student_frame->width() + 10,p.y()});
}
