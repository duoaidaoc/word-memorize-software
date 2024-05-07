#include "teacher_main.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "task_frame.h"
#include "ui_teacher_main.h"

#include "resource_manager.h"
#include "Image_processing.h"
#include "class_frame.h"
teacher_main::teacher_main(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::teacher_main)
{
  ui->setupUi(this);

  ui_setup();
  connection_setup();
  data_setup();
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

  //分界线绘画
  QColor bgColor = man->get_reversed_color();
  QFrame* v_line = new QFrame(ui->teacher_frame);
  v_line->setGeometry(0,ui->stackedWidget->y(),this->width(),3);
  v_line->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha()));
  QFrame* h_line = new QFrame(ui->teacher_frame);
  h_line->setGeometry(ui->label_teacher_main_pic->x() + ui->label_teacher_main_pic->width() + 20,0,3,this->height());
  h_line->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha()));


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
    // TODO():
    // 弹出一个窗口
    // 设置班级信息(班级id,班级名,班级码)
    // 关闭窗口自动添加(或添加失败处理错误信息)
    // 添加后刷新班级信息
  });
  QObject::connect(ui->btn_add_word,&QPushButton::clicked,[&](){
    //
  });

}

void teacher_main::data_setup()
{

}

void teacher_main::test()
{
  class_frame* cf = new class_frame();
  class_layout->addWidget(cf);
  QObject::connect(cf,&class_frame::set_display_content,[&](CClass cls){
    ui->stackedWidget->setCurrentIndex(2);
  });

  task_frame* tf = new task_frame();
  task_layout->addWidget(tf);
  QObject::connect(tf,&task_frame::set_display_content,[&](Task tsk){
    ui->stackedWidget->setCurrentIndex(6);
  });
}
