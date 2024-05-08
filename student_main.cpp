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

    auto man = resource_manager::getInstance();
    auto &database = man->get_database();
    auto &student = man->get_student();


    Word wd = {7,"abandon","abandon.txt ","v. 抛弃、放弃"};
    setup();
    setaction();
    data_setup();
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


    Ccue_frame = new classcue_frame();
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
      Ccue_frame->show();
    });
    QObject::connect(ui->change_btn,&QPushButton::clicked,[&](){
      ui->stackedWidget_2->setCurrentIndex(6);
    });
    QObject::connect(ui->test_btn,&QPushButton::clicked,[&](){
      // TODO():
      // 得到单词列表   若为空则不打开页面并报错！！！

      // 生成题目列表
      // 将题目列表放置到que_widget中
      // que_widget将题目准备好
      que_widget->show();
    });
    QObject::connect(Ccue_frame,&classcue_frame::UpdateClass,[&](){
      update_class();
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


    task_frame* tf = new task_frame(ui->task_contents);
    task_frames.push_back(tf);
    task_layout->addWidget(tf);

    QObject::connect(tf, &task_frame::set_display_content, [&](Task tsk){
      if(que_widget->get_status() == question::free){
        std::vector<Word>words;
        //得到单词列表
        words.reserve(word_frames.size());
        for(const auto &frm : word_frames){
          words.push_back(frm->get_content());
        }
        if(words.empty()){
          //TODO():报错
          return;
        }
        // TODO(): 当外围单词计划改变的时候，que_widget进度丢失并重新设置状态为free
        // TODO(): 其它情况不变
        que_widget->set_ques(words);
        que_widget->start();
      }
      que_widget->show();
    });

    ui->class_label->hide();
    ui->task_label->hide();

    QHBoxLayout *hbox = new QHBoxLayout(ui->wordlib_display);
    for (int i = 0; i < 10; ++i) { // 添加九个 QFrame 作为示例
        QFrame *itemFrame = new QFrame;
        itemFrame->setFixedSize(80, 100); // 固定大小为 80*100
        itemFrame->setStyleSheet("background-color: lightblue"); // 设置背景色
        hbox->addWidget(itemFrame);
    }
}

void student_main::data_setup()
{
    auto &student = resource_manager::getInstance()->get_student();
    ui->label_user_id->setText(QString::number(student.GetId()));
    ui->label_user_name->setText(student.GetName());
    update_class();
    // TODO():头像没完成
}

void student_main::update_display(int seq_)
{
    int n = word_frames.size();
    seq_ = (seq_ + n) % n;
    w_display->set_content(word_frames[seq_]->get_content(), seq_);
}

void student_main::update_class()
{
    // 非常重要，不能删掉，yo!

    // 删除所有原先存在的frame
    while (QLayoutItem *item = class_layout->takeAt(0)) {
      if (QWidget *widget = item->widget()) {
        // 从布局中移除部件
        class_layout->removeWidget(widget);
        // 删除部件并释放内存
        delete widget;
      }
      else{
        throw std::runtime_error("布局中不是widget");
      }
      // 删除布局项并释放内存
      delete item;
    }

    auto &student = resource_manager::getInstance()->get_student();
    auto list = student.infoStudentClass();
    for(const auto& [id, name] : list){
        CClass cls = {id ,name};
        class_frame* cf = new class_frame(ui->class_contents);
        class_frames.push_back(cf);
        class_layout->addWidget(cf);
        cf->setclass(cls);
        auto connect = QObject::connect(cf, &class_frame::set_display_content, [&](CClass cls){
          // 重置ui->student_vlayout中的frame
          for(auto &layout : student_appending_layout){
            if (!layout)
              return;
            while (QLayoutItem *item = layout->takeAt(0)) {
              if (QWidget *widget = item->widget()) {
                // 从布局中移除部件
                layout->removeWidget(widget);
                // 删除部件并释放内存
                delete widget;
              }
              else{
                throw std::runtime_error("布局中不是widget");
              }
              // 删除布局项并释放内存
              delete item;
            }
          }
          // 通过数据库获取 班级老师的信息(id, name)
          ui->label_class_name->setText(QString("班级名: ") + cls.name);
          const auto &class_detail = student.infoClassDetails(cls.id);
          if(!class_detail.size()){
            throw std::runtime_error("class他没有老师，这合理吗？");
          }
          ui->label_teacher_name->setText(QString("教师: ") + class_detail[0].teacherName);

          // 通过数据库获取 班级学生的信息
          const auto &class_members = student.infoClassMembers(cls.id);
          int manba_size = class_members.size();
          ui->label_student_number->setText(QString("学生人数:\n %1 人").arg(manba_size));

          // 创建并添加 QFrame 到布局
          for (int i = 0; i < manba_size; ++i) { // 添加九个 QFrame 作为示例
            QFrame *itemFrame = new QFrame(ui->student_display);
            itemFrame->setFixedSize(80, 100); // 固定大小为 80*100
            itemFrame->setAttribute(Qt::WA_TranslucentBackground, true);

            QLabel *icon = new QLabel(itemFrame);
            icon->move(0, 0);
            icon->setFixedSize(80,80);
            // TODO(): 改成正经的 头像。
            icon->setStyleSheet("border-image: url(../word-memorize-software/pics/man.png) 0 0 0 0;");

            QLabel *name = new QLabel(itemFrame);
            name->move(0,80);
            name->setFixedSize(80,20);
            name->setStyleSheet("color: black");
            name->setText(class_members[i].studentName);
            name->setAlignment(Qt::AlignCenter);


            student_appending_layout[i % 2]->addWidget(itemFrame);
          }

          ui->stackedWidget_2->setCurrentIndex(5);
          // 根据cls设置新的ui->student_vlayout中的frame
        });
        cf->add_connect(connect);
    }
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
