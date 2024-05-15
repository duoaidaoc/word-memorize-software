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

    setup();
    setaction();
    data_setup();
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
    ui->progressBar->setMinimum(0); // 设置最小值
    ui->progressBar->setMaximum(100); // 设置最大值

    Tip = new tip();

    signaltip_1 = new tip();
    signaltip_1->set_content("warning","当前进行其它任务中,旧的任务会被丢弃,确定再开启新任务吗?");
    QObject::connect(signaltip_1, &tip::confirm,[&](){
      clearNowWord();
      setNowWord(signaltip_1->tid);
    });
    signaltip_2 = new tip();
    signaltip_2->set_content("warning","确定退出课堂吗?!!!!");
    QObject::connect(signaltip_2, &tip::confirm,[&](){
      auto man = resource_manager::getInstance();
      auto student = man->get_student();
      student.leaveClass(cls_.id);
      cls_.id = -1;
      update_class();
      ui->stackedWidget_2->setCurrentIndex(3);
    });

    game_widget = new game();
    settings = new Settings();
    profile_pic = new QImage();


}

void student_main::setaction()
{
    QObject::connect(ui->learn_btn,&QPushButton::clicked,[&](){
      // TODO() 切换系统单词
      if(que_widget->get_status() == question::free){
        clearNowWord();
        setNowWord(-1);
      }
      else if(que_widget->get_task_id() != -1){
        signaltip_1->tid = -1;
        signaltip_1->show();
      }
      else{
        ui->stackedWidget_2->setCurrentIndex(1);
      }
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
      //得到单词列表应该在wordframes中
      if(que_widget->get_status() == question::free){
        std::vector<db::WordInfo>words;
        words.reserve(word_frames.size());
        auto man = resource_manager::getInstance();
        for(const auto &wd : word_frames){
          words.push_back(wd->get_content());
        }
        if(words.empty()){
          Tip->set_content("warning","没有单词");
          Tip->show();
          return;
        }
        que_widget->set_ques(words);
        que_widget->start();
      }
      que_widget->show();
    });
    QObject::connect(Ccue_frame,&classcue_frame::UpdateClass,[&](){
      update_class();
    });
    QObject::connect(ui->exi_class_btn,&QPushButton::clicked,[&](){
      signaltip_2->show();
    });
    auto &tmp = ui->lineEdit_2;
    QObject::connect(tmp,&QLineEdit::returnPressed,tmp,[this,tmp](){
      bool ok;
      qint64 plan = tmp->text().toLongLong(&ok);
      auto man = resource_manager::getInstance();
      auto &student = man->get_student();
      student.updatePlan(plan);
      ui->stackedWidget_2->setCurrentIndex(7);
    });
    QObject::connect(ui->plan_btn,&QPushButton::clicked,[&](){
      ui->stackedWidget_2->setCurrentIndex(0);
    });
    QObject::connect(que_widget,&question::finish,[&](){
      Tip->set_content("","做题完毕");
      Tip->show();
      // 刷新任务列表
      auto man = resource_manager::getInstance();
      auto student = man->get_student();
      auto sys = man->get_system();

      int ll =  student.returnStudentBank().toLongLong();
      if(ll != -1){
        double rate = sys.returnLearnedRateForWordBank(student.GetId(), ll);
        ui->progressBar->setValue(int(rate * 100));
      }

      clearNowTask();
      setNowTask();

      ui->stackedWidget_2->setCurrentIndex(7);

    });
    QObject::connect(que_widget,&question::word_learnt,[&](qint64 word_id, qint64 task_id){
      auto &student = resource_manager::getInstance()->get_student();
      qDebug()<< "学习了task";
      student.learnWordRecord(word_id, task_id);
    });
    QObject::connect(que_widget,&question::word_learnt_sys,[&](qint64 word_id){
      auto &student = resource_manager::getInstance()->get_student();
      qDebug()<< "学习了sys";
      student.learnSysWordRecord(word_id);
    });
    QObject::connect(ui->battle_btn,&QPushButton::clicked,[&](){
      game_widget->show();
    });
    // 打开设置界面
    QObject::connect(ui->setting_btn,&QPushButton::clicked,[&](){
      detailed_info info = {
          ui->detail_nick_label->text().mid(QString("昵称：").length()),
          ui->detail_age_label->text().mid(QString("年龄：").length()),
          ui->detail_school_label->text().mid(QString("学校：").length()),
          ui->detail_phone_label->text().mid(QString("电话号码：").length()),
          ui->detail_say_label->text().mid(QString("备注：").length()),
          profile_pic,
          profile_pic_url
      };
      settings->get_info(info);
      settings->show();
    });
    // 确认了更改
    QObject::connect(settings,&Settings::set_outer_info,[&](detailed_info info){
      ui->detail_nick_label->setText(QString("昵称：") + info.nickname);
      ui->detail_age_label->setText(QString("年龄：") + info.age);
      ui->detail_school_label->setText(QString("学校：") + info.school);
      ui->detail_phone_label->setText(QString("电话号码：") + info.phone);
      ui->detail_say_label->setText(QString("备注：") + info.say);
      *profile_pic = *info.img;
      ui->label_user_icon->setPixmap(QPixmap::fromImage(*profile_pic));
      ui->detail_pic_label->setPixmap(QPixmap::fromImage(*profile_pic));
      profile_pic_url = info.img_url;
      // TODO(): 修改数据库
      auto &student = resource_manager::getInstance()->get_student();
      student.storeSettingsForStudent(info.img_url,info.age.toLongLong(),info.phone,info.school,student.GetId(),info.say);

    });
}


void student_main::clearlayout(QBoxLayout *lo)
{
    if(!lo)
        return;
    // 删除所有原先存在的frame
    while (QLayoutItem *item = lo->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
          // 从布局中移除部件
          lo->removeWidget(widget);
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

void student_main::clearNowWord()
{
    clearlayout(word_layout);
    word_frames.clear();
    if(que_widget->get_status() == question::busy){
        que_widget->shutdown();
    }
}

void student_main::setNowWord(qint64 tid)
{
    if(que_widget->get_status() == question::busy){
        qDebug() << "question frame Busying!";
        return;
    }
    que_widget->set_task_id(tid);
    QList<db::WordInfo>words;
    auto man = resource_manager::getInstance();
    auto student = man->get_student();
    if(tid != -1){
      words = student.infoWordsInTask(tid);
    }
    else{
      auto sys = man->get_system();
      QVariant tmp = student.returnStudentBank();
      if(tmp.isNull()) {
        Tip->set_content("warning","没有单词！");
        Tip->show();
        return;
      }
      words = sys.generateWords(student.GetId(), student.returnStudentBank().toLongLong(), student.infoPlan().toLongLong());
    }
    for(const auto& word: words){
        word_frame* wd = new word_frame(ui->word_contents);
        word_frames.push_back(wd);
        wd->set_content(word, word_frames.size() - 1);
        word_layout->addWidget(wd);
        QObject::connect(wd, &word_frame::set_display_content, this, &student_main::show_display);
        QObject::connect(w_display, &word_display::upd_page, this, &student_main::update_display);
    }
    ui->stackedWidget_2->setCurrentIndex(1);
}

void student_main::clearNowTask()
{
    clearlayout(task_layout);
    task_frames.clear();
}

void student_main::setNowTask()
{
    if(!task_frames.empty()){
        qDebug() << "在set前需要clear task";
        abort();
    }
    auto man = resource_manager::getInstance();
    auto student = man->get_student();
    for(const auto& class_frame: class_frames){
        auto cls = class_frame->getclass();
        auto tasks = student.infoTaskInClass(cls.id);
        for(const auto& tsk: tasks){
          //过滤已学习的任务
          if(student.infoTaskCondition(student.GetId(),tsk.taskId) > 0.99999){
            return;
          }

          task_frame* tf = new task_frame(ui->task_contents);
          tf->settask(cls, tsk);
          task_frames.push_back(tf);
          task_layout->addWidget(tf);
          QObject::connect(tf, &task_frame::set_display_content, [&](db::TaskInfo tsk){
            // 把Tsk的单词列表拿出来放在word_frames里面
            if(que_widget->get_status() == question::free){
              clearNowWord();
              setNowWord(tsk.taskId);
            }
            else if(que_widget->get_task_id() != tsk.taskId){
              signaltip_1->tid = tsk.taskId;
              signaltip_1->show(); // 选择确定则开启当前任务
            }
            else{
              ui->stackedWidget_2->setCurrentIndex(1);
            }
          });
        }
    }
}

void student_main::data_setup()
{
    auto &student = resource_manager::getInstance()->get_student();
    ui->label_user_id->setText(QString("账号 :") + QString::number(student.GetId()));
    ui->label_user_name->setText(QString("姓名 :") + student.GetName());
    update_class();
    // TODO():头像没完成
    // 设置个人信息
    const auto &info = student.retrieveSettingsForStudent(student.GetId());
    // nickname
    ui->detail_age_label->setText(QString("年龄：") + QString::number(info.age));
    ui->detail_school_label->setText(QString("学校：") + info.school);
    ui->detail_phone_label->setText(QString("电话号码：") + info.phone);
    ui->detail_say_label->setText(QString("备注：") + info.message);
    profile_pic->load(info.filepath);
    profile_pic_url = info.filepath;
    ui->label_user_icon->setPixmap(QPixmap::fromImage(*profile_pic));
    ui->detail_pic_label->setPixmap(QPixmap::fromImage(*profile_pic));

    // 设置个人信息结束
    //词库只需要在开始的时候初始化
    auto man = resource_manager::getInstance();
    auto sys = man->get_system();

    int ll =  student.returnStudentBank().toLongLong();
    if(ll != -1){
        double rate = sys.returnLearnedRateForWordBank(student.GetId(), ll);
        ui->progressBar->setValue(int(rate * 100));
    }
    const auto &idlist = sys.returnWordBankInfo();
    QHBoxLayout *hbox = new QHBoxLayout(ui->wordlib_display);
    for (int i = 0; i < idlist.size(); ++i) { // 添加 QFrame
      QFrame *itemFrame = new QFrame;
      itemFrame->setFixedSize(80, 100); // 固定大小为 80*100
      itemFrame->setAttribute(Qt::WA_TranslucentBackground, true);

      QLabel *icon = new QLabel(itemFrame);
      icon->move(0, 0);
      icon->setFixedSize(80,80);
      // TODO(): 改成正经的 图标。
      icon->setStyleSheet("border-image: url(../word-memorize-software/pics/man.png) 0 0 0 0;");

      QPushButton *name = new QPushButton(itemFrame);
      name->move(0,80);
      name->setFixedSize(80,20);
      name->setStyleSheet("color: black");
      name->setText(idlist[i].name);
      hbox->addWidget(itemFrame);

      QVariant tmp = QVariant::fromValue(idlist[i]);
      name->setProperty("wordBankInfo", tmp);

      QObject::connect(name,&QPushButton::clicked,name,[this, name]{
        QVariant tmp = name->property("wordBankInfo");
        auto man = resource_manager::getInstance();
        auto student = man->get_student();
        if(tmp.canConvert<db::WordBankInfo>()){
          db::WordBankInfo info = tmp.value<db::WordBankInfo>();
          if(name->text() == "") ui->n_label->setText(QString("当前没有正在学习的词库"));
          else ui->n_label->setText(QString("当前正在学习的词库") + name->text());
          student.learnWordBanks(info.id);
          auto system = man->get_system();

          int ll =  student.returnStudentBank().toLongLong();
          if(ll != -1){
            double rate = system.returnLearnedRateForWordBank(student.GetId(), ll);
            ui->progressBar->setValue(int(rate * 100));
          }

        }
        else{
          qDebug()<< "frame 没有 wordBankInfo属性";
        }
      });
    }
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
    clearlayout(class_layout);
    class_frames.clear();

    auto &student = resource_manager::getInstance()->get_student();
    auto list = student.infoStudentClass();
    for(const auto& [id, name] : list){
        CClass cls = {id ,name};
        class_frame* cf = new class_frame(ui->class_contents);
        class_frames.push_back(cf);
        class_layout->addWidget(cf);
        cf->setclass(cls);
        auto connect = QObject::connect(cf, &class_frame::set_display_content, [&](CClass _cls){
          cls_ = _cls;
          // 重置ui->student_vlayout中的frame
          for(auto &layout : student_appending_layout){
            clearlayout(layout);
          }
          // 通过数据库获取 班级老师的信息(id, name)
          ui->label_class_name->setText(QString("班级名: ") + _cls.name);
          const auto &class_detail = student.infoClassDetails(_cls.id);
          if(!class_detail.size()){
            throw std::runtime_error("class他没有老师，这合理吗？");
          }
          ui->label_teacher_name->setText(QString("教师: ") + class_detail[0].teacherName);

          // 通过数据库获取 班级学生的信息
          const auto &class_members = student.infoClassMembers(_cls.id);
          int manba_size = class_members.size();
          ui->label_student_number->setText(QString("学生人数:\n %1 人").arg(manba_size));

          // 创建并添加 QFrame 到布局
          for (int i = 0; i < manba_size; ++i) {
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
    }
    clearNowTask();
    setNowTask();
}

void student_main::show_display(const db::WordInfo &wd,int seq)
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
