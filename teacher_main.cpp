#include "teacher_main.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "task_frame.h"
#include "ui_teacher_main.h"

#include "resource_manager.h"
#include "class_frame.h"
#include "newclass_frame.h"
#include "qfiledialog.h"
#include "QStandardItemModel"
teacher_main::teacher_main(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::teacher_main)
{
  ui->setupUi(this);

  ui_setup();
  connection_setup();
  //data_setup();
  test();
}

teacher_main::~teacher_main()
{
  delete ui;
}

void teacher_main::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath clipPath;
  clipPath.addRoundedRect(rect(),20,20);
  painter.setClipPath(clipPath);
  painter.drawImage(rect(), img);

  QWidget::paintEvent(evt);
}

void teacher_main::ui_setup()
{
  this->setProperty("canMove",true);
  this->setAttribute(Qt::WA_TranslucentBackground,true);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

  //背景图选取
  auto man = resource_manager::getInstance();
  img.load(man->bg_pic_randomselect());
  // 高斯模糊 ImageProcesser::GaussiamBlur(20,20,img);

  //班级展示列表绘画
  class_layout = new QVBoxLayout(ui->class_contents);
  class_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
  task_layout = new QVBoxLayout(ui->task_contents);
  task_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
  word_layout = new QVBoxLayout(ui->word_contents);
  word_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
  task_view_layout = new QVBoxLayout(ui->task_view_contents);
  task_view_layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);


  //分界线绘画
  QColor bgColor = man->get_reversed_color();
  QFrame* v_line = new QFrame(ui->teacher_frame);
  v_line->setGeometry(0,ui->stackedWidget->y(),this->width(),3);
  v_line->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha()));
  QFrame* h_line = new QFrame(ui->teacher_frame);
  h_line->setGeometry(ui->label_teacher_main_pic->x() + ui->label_teacher_main_pic->width() + 20,0,3,this->height());
  h_line->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha()));


  student_fixed_layout = new QVBoxLayout(ui->student_display);
  student_appending_layout[0] = new QHBoxLayout;
  student_appending_layout[1] = new QHBoxLayout;
  student_fixed_layout->addLayout(student_appending_layout[0]);
  student_fixed_layout->addLayout(student_appending_layout[1]);
  ui->student_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->student_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->word_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->word_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->scrollArea_task_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->scrollArea_task_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  ui->tableView->setFocusPolicy(Qt::NoFocus);
  ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);

  Tip = new tip();
  nc_frame = new newclass_frame(this);
  model = new QStandardItemModel();
  ui->tableView->setModel(model);
  stu_model = new QStandardItemModel();
  ui->tableView_student->setModel(stu_model);
}

void teacher_main::connection_setup()
{
  // 点击退出按钮退出程序
  QObject::connect(ui->close_button,&QPushButton::clicked,[&](){
    QApplication::quit();
  });
  // 点击最小化按钮最小化窗口
  QObject::connect(ui->min_button,&QPushButton::clicked,[&](){
    this->showMinimized();
  });
  // 点击显示个人信息
  QObject::connect(ui->info_btn,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(0);
  });
  // 点击显示班级总览
  QObject::connect(ui->class_btn,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(1);
  });
  // 班级内点击管理任务列表
  QObject::connect(ui->btn_view_task,&QPushButton::clicked,[&](){
    ui->label_class_of_task->setText(QString("班级: %1的任务列表").arg(current_cls.name));
    ui->stackedWidget->setCurrentIndex(3);
  });
  // 任务管理内点击创建任务进入任务创建界面
  QObject::connect(ui->btn_add_task,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(4);
  });
  // 任务管理界面点击返回
  QObject::connect(ui->btn_return_to_detail,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(2);
  });
  // 任务创建界面点击返回
  QObject::connect(ui->btn_create_to_task,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(3);
  });
  // 任务完成情况界面返回任务管理界面
  QObject::connect(ui->btn_condition_to_task,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(3);
  });
  // 任务内容界面返回任务管理界面
  QObject::connect(ui->btn_content_to_task,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(3);
  });
  // 任务完成情况界面跳转任务内容
  QObject::connect(ui->btn_condition_to_content,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(6);
  });
  // 任务内容界面跳转任务完成情况
  QObject::connect(ui->btn_content_to_condition,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(5);
  });
  // 创建班级
  QObject::connect(ui->class_add_btn,&QPushButton::clicked,[&](){
    nc_frame->show();
  });
  // 为任务添加单词,跳转到单词添加页面
  QObject::connect(ui->btn_add_word,&QPushButton::clicked,[&](){
    model->clear();
    ui->stackedWidget->setCurrentIndex(7);
  });
  // 加入班级马上刷新
  QObject::connect(nc_frame,&newclass_frame::UpdateClass,[&](){
    update_class();
  });
  // 导入txt
  QObject::connect(ui->btn_loadtxt,&QPushButton::clicked,[&](){
    QString filter = "Text Files (*.txt);;All Files (*.*)";
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", filter);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "无法打开文件：" << fileName;
      return;
    }

    // 2. 使用 QTextStream 读取文件
    QTextStream in(&file);
    QStringList wordList;

    // 3. 读取文本数据并拆分成单词
    while (!in.atEnd()) {
      QString line = in.readLine();
      QStringList wordsInLine = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
      wordList.append(wordsInLine);
    }

    file.close(); // 记得关闭文件

    // 4. 创建一个 QStandardItemModel，并设置行和列的数量
    model->setRowCount(wordList.size());
    model->setColumnCount(1);

    // 5. 将单词添加到模型中
    for (int row = 0; row < wordList.count(); ++row) {
      const QString& word = wordList.at(row);
      QStandardItem *item = new QStandardItem(word);
      model->setItem(row, 0, item);
    }

    // 6. 创建 QTableView，并设置模型

  });
  // 清空导入内容
  QObject::connect(ui->btn_discard,&QPushButton::clicked,[&](){
    model->clear();
  });
  QObject::connect(ui->btn_ret,&QPushButton::clicked,[&](){
    model->clear();
    ui->stackedWidget->setCurrentIndex(4);
  });
  QObject::connect(ui->btn_addtotask,&QPushButton::clicked,[&](){
    int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row) {
      // 获取第一列数据
      QModelIndex index = model->index(row, 0); // 第一个参数是行，第二个参数是列
      QVariant data = model->data(index, Qt::DisplayRole); // 获取数据
      QString value = data.toString(); // 转换为 QString

      // 在这里对第一列的数据进行操作，比如输出到控制台
      word_frame* wf = new word_frame(ui->word_contents);
      wf->set_btn_disabled();
      db::WordInfo wd;
      wd.english = value;
      wf->set_content(wd);
      word_layout->addWidget(wf);
      word_frames.push_back(wf);
    }
    ui->stackedWidget->setCurrentIndex(4);
  });
  // 任务完成创建
  QObject::connect(ui->btn_commit,&QPushButton::clicked,[&](){
    clearlayout(word_layout);
    word_frames.clear();

    auto man = resource_manager::getInstance();
    auto teacher = man->get_teacher();
    QList<QString> words;
    for(const auto &wf: word_frames){
      words.push_back(wf->get_content().english);
    }
    int tid = teacher.importTaskWordBank(words) ;
    if(tid >= 0){
      Tip->set_content("","创建成功");
      Tip->show();
      teacher.createTask(tid, current_cls.id, "jdjdjdj", QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());
      clearlayout(word_layout);
      word_frames.clear();
    }
    else{
      Tip->set_content("warning", "创建失败");
      Tip->show();
    }
    clearNowTask();
    setNowTask();
    ui->stackedWidget->setCurrentIndex(3);
  });

}

void teacher_main::data_setup()
{
  auto man = resource_manager::getInstance();
  auto &teacher = man->get_teacher();
  ui->label_user_id->setText(QString::number(teacher.GetId()));
  ui->label_user_name->setText(teacher.GetName());
  update_class();
}

void teacher_main::update_class()
{
    // 非常重要，不能删掉，yo!

    // 删除所有原先存在的frame
    clearlayout(class_layout);
    auto &teacher = resource_manager::getInstance()->get_teacher();
    auto list = teacher.infoTeacherClass();
    if(list.size() > 0){
      ui->class_label->hide();
    }
    for(const auto& [id, name] : list){
        CClass cls = {id ,name};
        class_frame* cf = new class_frame(ui->class_contents);
        class_frames.push_back(cf);
        class_layout->addWidget(cf);
        cf->setclass(cls);
        auto connect = QObject::connect(cf, &class_frame::set_display_content, [&](CClass cls){

          current_cls = cls;
//          qDebug() << "1Q1111111111111111111111111111111111111111111111111\n";
//          qDebug() << QString::number(cls.id) + ' ' + cls.name + ' ' + cls.cue + ' ';
          // 重置ui->student_vlayout中的frame
          for(auto &layout : student_appending_layout){
            clearlayout(layout);
          }
          // 通过数据库获取 班级老师的信息(id, name)
          ui->label_class_name->setText(QString("班级名: ") + cls.name);
          const auto &class_detail = teacher.infoClassDetails(cls.id);
          if(!class_detail.size()){
            throw std::runtime_error("class他没有老师，这合理吗？");
          }
          ui->label_teacher_name->setText(QString("教师: ") + class_detail[0].teacherName);

          // 通过数据库获取 班级学生的信息
          const auto &class_members = teacher.infoClassMembers(cls.id);
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
          ui->stackedWidget->setCurrentIndex(2);
          // 根据cls设置新的ui->student_vlayout中的frame
        });
    }
    clearNowTask();
    setNowTask();
}

void teacher_main::clearlayout(QBoxLayout *layout)
{
    if(!layout)
        return;
    // 删除所有原先存在的frame
    while (QLayoutItem *item = layout->takeAt(0)) {
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
}

void teacher_main::clearNowTask()
{
    clearlayout(task_layout);
    task_frames.clear();
}

void teacher_main::setNowTask()
{
    if(!task_frames.empty()){
        qDebug() << "在set前需要clear task";
        abort();
    }
    auto man = resource_manager::getInstance();
    auto teacher = man->get_teacher();
    for(const auto& class_frame: class_frames){
        auto cls = class_frame->getclass();
        auto tasks = teacher.infoTaskInClass(cls.id);
        for(const auto& tsk: tasks){
          task_frame* tf = new task_frame(ui->task_contents);
          tf->settask(cls, tsk);
          task_frames.push_back(tf);
          task_layout->addWidget(tf);
          QObject::connect(tf, &task_frame::set_display_content, [&](db::TaskInfo tsk){
            auto man = resource_manager::getInstance();
            auto teacher = man->get_teacher();
            // 单词页清空
            clearlayout(task_view_layout);
            task_view_frames.clear();
            // 学生页清空
            stu_model->clear();
            // 将tsk的单词添加到单词页面
            const auto &words = teacher.infoWordsInTask(tsk.taskId);
            for(const auto &word : words){
              word_frame* wf = new word_frame();
              wf->set_btn_disabled();
              wf->set_content(word);
              task_view_frames.push_back(wf);
              task_view_layout->addWidget(wf);
            }
            // 将tsk的学生按完成情况排序
            const auto &students = teacher.infoClassMembers(current_cls.id);

            stu_model->setRowCount(students.size());
            stu_model->setColumnCount(3);
            stu_model->setHorizontalHeaderItem(0, new QStandardItem("学生学号"));
            stu_model->setHorizontalHeaderItem(1, new QStandardItem("姓名"));
            stu_model->setHorizontalHeaderItem(2, new QStandardItem("完成率"));

            for(int i = 0; i < students.size();i++){
              QStandardItem *item = new QStandardItem(QString::number(students[i].studentId));
              stu_model->setItem(i, 0, item);
              QStandardItem *item_1 = new QStandardItem(students[i].studentName);
              stu_model->setItem(i, 1, item_1);
              QStandardItem *item_2 = new QStandardItem(QString::number(teacher.infoTaskCondition(students[i].studentId,tsk.taskId)));
              stu_model->setItem(i, 2, item_2);
            }
            ui->stackedWidget->setCurrentIndex(6);
          });
        }
    }
}

void teacher_main::test()
{
//  class_frame* cf = new class_frame();
//  class_layout->addWidget(cf);
//  QObject::connect(cf,&class_frame::set_display_content,[&](CClass cls){
//    ui->stackedWidget->setCurrentIndex(2);
//  });

//  task_frame* tf = new task_frame();
//  task_layout->addWidget(tf);
//  QObject::connect(tf,&task_frame::set_display_content,[&](Task tsk){
//    ui->stackedWidget->setCurrentIndex(6);
//  });
}
