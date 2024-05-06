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
    word_layout = new QVBoxLayout(ui->word_contents);
    class_layout = new QVBoxLayout(ui->class_contents);
    task_layout = new QVBoxLayout(ui->task_contents);
    word_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    class_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    task_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    // 无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // 透明背景
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    auto man = resource_manager::getInstance();
    auto path = man->bg_pic_randomselect();
    ui->student_frame->setStyleSheet(QString("#student_frame {"
                                           "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                           "}").arg(path));
    QColor bgColor = man->get_reversed_color();
    v_line = new QFrame(ui->student_frame);
    v_line->setGeometry(0,ui->stackedWidget->y(),ui->student_frame->width(),3);
    v_line->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha()));
    h_line = new QFrame(ui->student_frame);
    h_line->setGeometry(ui->stackedWidget->x() + ui->stackedWidget->width(),0,3,ui->student_frame->height());
    h_line->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha()));

    ui->lexicon_label->setStyleSheet(QString("#lexicon_label {"
                                             "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                             "}").arg("../word-memorize-software/pics/tmp.png"));


    c_frame = new classcue_frame();
    w_display = new word_display();
    que_widget = new question();

    student_fixed_layout = new QVBoxLayout(ui->student_display);
    student_appending_layout[0] = new QHBoxLayout;
    student_appending_layout[1] = new QHBoxLayout;
    student_fixed_layout->addLayout(student_appending_layout[0]);
    student_fixed_layout->addLayout(student_appending_layout[1]);
    ui->student_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->student_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->wordlib_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->wordlib_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    QObject::connect(ui->info_btn,&QPushButton::clicked,[&](){
      ui->stackedWidget_2->setCurrentIndex(2);
    });
    QObject::connect(ui->class_btn,&QPushButton::clicked,[&](){
      ui->stackedWidget_2->setCurrentIndex(3);
    });
    QObject::connect(ui->task_btn,&QPushButton::clicked,[&](){
      ui->stackedWidget_2->setCurrentIndex(4);
    });
    QObject::connect(ui->class_add_btn,&QPushButton::clicked,[&](){
      c_frame->show();
    });
    QObject::connect(ui->change_btn,&QPushButton::clicked,[&](){
      ui->stackedWidget_2->setCurrentIndex(6);
    });
    QObject::connect(ui->test_btn,&QPushButton::clicked,[&](){
      // TODO():
      // 得到单词列表
      // 生成题目列表
      // 将题目列表放置到que_widget中
      // que_widget将题目准备好
      que_widget->show();
    });
}

void student_main::addword(const Word &word)
{
    //初始化demo
    for(int i = 1;i<=10;i++){
        word_frame* wd = new word_frame(ui->word_contents);
        word_frames.push_back(wd);
        wd->set_content(word, word_frames.size() - 1);
        word_layout->addWidget(wd);
        QObject::connect(wd, &word_frame::set_display_content, this, &student_main::show_display);
        QObject::connect(w_display, &word_display::upd_page, this, &student_main::update_display);
    }
    class_frame* cf = new class_frame(ui->class_contents);
    class_frames.push_back(cf);
    class_layout->addWidget(cf);
    QObject::connect(cf, &class_frame::set_display_content, [&](CClass cls){
      // TODO():
      // 重置ui->student_vlayout中的frame
      ui->stackedWidget_2->setCurrentIndex(5);
      // 根据cls设置新的ui->student_vlayout中的frame
    });

    task_frame* tf = new task_frame(ui->task_contents);
    task_frames.push_back(tf);
    task_layout->addWidget(tf);

    QObject::connect(tf, &task_frame::set_display_content, [&](Task tsk){
      // TODO():
      // 得到单词列表
      // 生成题目列表
      // 将题目列表放置到que_widget中
      // que_widget将题目准备好
      // (和上文可以打包复用)
      que_widget->show();
    });

    ui->class_label->hide();
    ui->task_label->hide();



    // 创建并添加 QFrame 到布局
    for (int i = 0; i < 20; ++i) { // 添加九个 QFrame 作为示例
        QFrame *itemFrame = new QFrame;
        itemFrame->setFixedSize(80, 100); // 固定大小为 80*100
        itemFrame->setStyleSheet("background-color: lightblue"); // 设置背景色
        student_appending_layout[i % 2]->addWidget(itemFrame);
    }
    QHBoxLayout *hbox = new QHBoxLayout(ui->wordlib_display);
    for (int i = 0; i < 10; ++i) { // 添加九个 QFrame 作为示例
        QFrame *itemFrame = new QFrame;
        itemFrame->setFixedSize(80, 100); // 固定大小为 80*100
        itemFrame->setStyleSheet("background-color: lightblue"); // 设置背景色
        hbox->addWidget(itemFrame);
    }
}

void student_main::update_display(int seq_)
{
    int n = word_frames.size();
    seq_ = (seq_ + n) % n;
    w_display->set_content(word_frames[seq_]->get_content(), seq_);
}

void student_main::show_display(const Word &wd,int seq)
{
    if(seq < 0){
        throw std::runtime_error("seq isn't initialized.");
    }

    if(!w_display->isVisible()){
        move_display();
    }
    w_display->set_content(wd, seq);
    w_display->enter();
    w_display->show();

}

void student_main::move_display()
{
    QPoint p = pos() + ui->student_frame->pos();
    w_display->set_nexpos({p.x() + ui->student_frame->width() + 10,p.y()});
}
