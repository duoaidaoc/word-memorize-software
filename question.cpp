#include <QEvent>
#include <QPainter>
#include <QPainterpath>

#include "question.h"
#include "ui_question.h"
#include "resource_manager.h"
#include "Image_processing.h"

#include <cstdlib>

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
  QObject::connect(ui->eng_edit,&QLineEdit::returnPressed,[&](){
    if(ui->eng_edit->text() != ""){
      done(ui->eng_edit->text() == correct_english);
      ui->eng_edit->setText("");
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

void question::shutdown()
{
  // 清空问题队列
  std::queue<ques> empty_ques_que;
  std::swap(questions, empty_ques_que);
  // 清空完成计数
  this->finished_cnt.clear();

  st = state::free;
  this->hide();
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

// 生成0-n中，与pre不同的随机数
// 供set_ques调用
static int get_diff_rand(int n, int pre) {
  int r = rand()%n;
  while (r == pre)
    r = rand()%n;
  return r;
}

// 使用 words 生成 questions
// 参数：const std::vector<db::WordInfo> &_words 用于生成问题队列的单词列表
// 功能：初始化私有变量this->questions
// 算法：设_words长度为n，则qeustions由n个选择题和n个填空题组成
//      其中每个选择题在ABC三个选项中随机选择一个作为正确选项，填写第i个单词的中文
//      另外两个干扰项从_words中随机选择其它的单词的中文，因此可能会有两个选项内容相同
void question::set_ques(const std::vector<db::WordInfo> &_words)
{
  int n = _words.size();
  // 生成n个选择题
  for (int i=0; i<n; i++) {
    ques cur_ques;
    cur_ques.word_id = _words[i].word_id;
    cur_ques.is_sel = true;
    cur_ques.is_fill = false;
    cur_ques.chn = _words[i].chinese;
    cur_ques.eng = _words[i].english;

    // 在ABC三个选项中随机选择一个作为正确选项，填写第i个单词的中文
    // 另外两个干扰项从_words中随机选择其它的单词的中文，因此可能会有两个选项内容相同
    int right = rand()%3+1;
    cur_ques.right = right;
    if (right == 1) {
      cur_ques.A = _words[i].chinese;
      cur_ques.B = _words[get_diff_rand(n,i)].chinese;
      cur_ques.C = _words[get_diff_rand(n,i)].chinese;
    } else if (right == 2) {
      cur_ques.A = _words[get_diff_rand(n,i)].chinese;
      cur_ques.B = _words[i].chinese;
      cur_ques.C = _words[get_diff_rand(n,i)].chinese;
    } else {
      cur_ques.A = _words[get_diff_rand(n,i)].chinese;
      cur_ques.B = _words[get_diff_rand(n,i)].chinese;
      cur_ques.C = _words[i].chinese;
    }

    this->questions.push(cur_ques);
  }

  // 生成n个填空题
  for (int i=0; i<n; i++) {
    ques cur_ques;
    cur_ques.word_id = _words[i].word_id;
    cur_ques.is_sel = false;
    cur_ques.is_fill = true;
    cur_ques.chn = _words[i].chinese;
    cur_ques.eng = _words[i].english;
    this->questions.push(cur_ques);
  }
}

// 从题目列表中拿出一题开始写
// 功能：根据题目类型
//      调用set_selection_question()选择题窗口
//      或set_fillword_question()填空题窗口
// 算法：根据this->questions的队首元素
//      如果是选择题，调用set_selection_question生成选择题
//      如果是填空题，调用set_fillword_question生成填空题
void question::start()
{
  if(st == state::free)
    st = state::busy;

  ques curr_question = this->questions.front();

  // 如果是选择题，调用set_selection_question生成选择题
  if (curr_question.is_sel) {
    set_selection_question(curr_question.eng, curr_question.A, curr_question.B, curr_question.C, curr_question.right);
  // 如果是填空题，调用set_fillword_question生成填空题
  } else if (curr_question.is_fill) {
    set_fillword_question(curr_question.eng, curr_question.chn);
  } else {
    exit(-1);
  }
}

// 答题函数在回答完毕时自动调用done
// 参数：bool ok 问题回答正确与否
// 功能：根据当前问题答题情况修改问题队列questions
//      根据当前问题答题情况决定修改数据库word_learnt与否
//      根据问题队列情况决定finish与否
// 算法：根据问题回答情况，做出相应的动作
//      如果问题答对即ok == true，当前问题的当前单词的完成计数加1，如果达到2，向数据库发出完成信号
//      如果问题答对即ok == false，将当前问题加到问题队列队尾
//      如果问题队列为空，说明已经做完，发出finish信号，状态置为空闲
//      如果问题队列非空，调用start()取出下一题
void question::done(bool ok)
{
  // 取出当前问题队列队首的问题
  ques curr_question = this->questions.front();
  this->questions.pop();
  qint64 curr_id = this->questions.front().word_id;

  // 如果问题答对即ok == true，当前问题的当前单词的完成计数加1，如果达到2，向数据库发出完成信号
  if (ok) {
    this->finished_cnt[curr_id]++;
    if (this->finished_cnt[curr_id] == 2) {
      emit word_learnt(curr_id);
    }
  // 如果问题答对即ok == false，将当前问题加到问题队列队尾
  } else {
    this->questions.push(curr_question);
  }

  // 如果问题队列为空，说明已经做完，发出finish信号，状态置为空闲
  if (this->questions.empty()) {
    emit finish();
    st = state::free;
  // 如果问题队列非空，调用start()取出下一题
  } else {
    start();
  }
}








