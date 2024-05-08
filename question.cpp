#include <QEvent>
#include <QPainter>
#include <QPainterpath>

#include "question.h"
#include "ui_question.h"
#include "resource_manager.h"
#include "Image_processing.h"
question::question(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::question)
{
  ui->setupUi(this);

  auto man = resource_manager::getInstance();

  this->setAttribute(Qt::WA_TranslucentBackground,true);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setProperty("canMove",true);
  QFont font;
  font.setPointSize(20);
  ui->eng_edit->setFont(font);

  bg = new QImage(man->bg_pic_randomselect());
  ImageProcesser::GaussiamBlur(20, 20, *bg);

  QObject::connect(ui->close_btn,&QPushButton::clicked,[&](){
    ui->eng_edit->clear();
    this->hide();
  });
  QObject::connect(ui->close_btn_2,&QPushButton::clicked,[&](){
    this->hide();
  });
  QObject::connect(ui->eng_edit,&QLineEdit::editingFinished,[&](){
    if(ui->eng_edit->text() != ""){

    }
  });

  st = state::free;
  task_id = -2; // -1 表示被每日任务占用，-2表示未被占用
}

question::~question()
{
  delete ui;
}

void question::set_selection_question(const QString &english, const QString &A, const QString &B, const QString &C, int _correctitem)
{
  ui->eng_label->setText(english);
  ui->A_btn->setText(QString("A: ") + A);
  ui->B_btn->setText(QString("B: ") + B);
  ui->C_btn->setText(QString("C: ") + C);
  correctitem = _correctitem;
  ui->stackedWidget->setCurrentIndex(1);
}

void question::set_fillword_question(const QString &english, const QString &chinese)
{
  ui->chn_label->setText(chinese);
  correct_english = english;
  ui->stackedWidget->setCurrentIndex(0);
}

question::state question::get_status()
{
  return st;
}


void question::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath path;
  path.addRoundedRect(rect(),20,20);
  painter.setClipPath(path);
  painter.drawImage(rect(), *bg);

  QWidget::paintEvent(evt);
}

void question::set_connection()
{
  QObject::connect(ui->A_btn,&QPushButton::clicked,[&](){
    done(correctitem == 1);
  });
  QObject::connect(ui->B_btn,&QPushButton::clicked,[&](){
    done(correctitem == 2);
  });
  QObject::connect(ui->B_btn,&QPushButton::clicked,[&](){
    done(correctitem == 3);
  });
  QObject::connect(ui->eng_edit,&QLineEdit::editingFinished,[&](){
    if(ui->eng_edit->text() != ""){
      done(ui->eng_edit->text() == correct_english);
    }
  });
  QObject::connect(ui->close_btn,&QPushButton::clicked,[&](){
    done(correctitem == 3);
  });
  QObject::connect(ui->close_btn_2,&QPushButton::clicked,[&](){
    done(correctitem == 3);
  });
}

void question::set_task_id(qint64 _task_id)
{
  task_id = _task_id;
}

qint64 question::get_task_id()
{
  return task_id;
}

void question::set_ques(const std::vector<db::WordInfo> &_words)
{

  // TODO(): 使用 words 生成 questions
}

void question::start()
{
  if(st == state::free)
    st = state::busy;
  // TODO(): 从题目列表中拿出一题开始写。
  // 本类中的private接口: set_selection_question() 注释见头文件
  // 本类中的private接口: set_fillword_question() 注释见头文件

}
//  答题函数在回答完毕时自动调用done
void question::done(bool ok)
{
  // TODO():
  // ok == 1 答对 从问题序列中删除。
  // ok == 0 答错 之后的某个时刻重新做。

  // 做没做完？
  // 没做完则下一题。
  // 做完了 发出finish信号 qt语句: emit finish();
  // 且 state = state::free
}








