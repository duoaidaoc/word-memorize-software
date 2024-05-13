#include "game.h"
#include "ui_game.h"

#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <algorithm>

#include "resource_manager.h"
#include "Image_processing.h"

game::game(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::game)
{
  ui->setupUi(this);

  setup_ui();
  setup_connection();
  setup_words();
}

game::~game()
{
  delete ui;
}

void game::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath path;
  path.addRoundedRect(rect(), 20, 20);
  painter.setClipPath(path);
  painter.drawImage(rect(),*bg);

  QWidget::paintEvent(evt);
}

void game::setup_ui()
{
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  this->setProperty("canMove",true);

  auto man = resource_manager::getInstance();
  QString back_ground_path = man->bg_pic_randomselect();
  bg = new QImage;
  bg->load(back_ground_path);
  ImageProcesser::GaussiamBlur(20 ,20 ,*bg);

  timer = new QTimer(this);
//  TODO(): 设置计时器初始时间
   // 初始时间设置为100


  layout = new QVBoxLayout(ui->game_rank_scrollArea_contents);
  layout->setAlignment(Qt::AlignTop|Qt::AlignLeft);

  Tip = new tip();
  Tip->set_content("游戏结束", "本局游戏得分: xxx");

  ui->game_rank_scrollArea_contents->setStyleSheet("background-color: transparent;");
  ui->game_edit_frame->setStyleSheet("border: 2px solid white; border-radius: 5px; font-size: 30px;");
  ui->game_word_label->setStyleSheet("font-size:40px;");

}

void game::setup_connection()
{
  QObject::connect(ui->game_start_btn,&QPushButton::clicked,[&](){
    ui->stackedWidget_start->setCurrentIndex(1);
  });
  QObject::connect(ui->game_name_lineedit,&QLineEdit::returnPressed,[&](){
    name = ui->game_name_lineedit->text();
    ui->game_name_lineedit->clear();
    ui->stackedWidget_start->setCurrentIndex(0);
    //游戏开始
    nowTime = initialTime;
    now_score = 0;
    ui->game_score_label->setText(QString::number(now_score));
    ui->game_word_label->setText(words[idx].english);
    idx = (idx + 1) % words.size();
    ui->game_line_edit->clear();

    ui->stackedWidget->setCurrentIndex(2);
    timer->start(1000); // 每1000毫秒触发一次定时器
  });
  QObject::connect(ui->game_rule_btn,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(1);
  });
  //游戏正常结束
  QObject::connect(ui->game_rank_btn,&QPushButton::clicked,[&](){
    clearlayout(layout);
    int pos = qMin(5, rank.size()) - 1; // 4
    for(int i = pos; i >=0 ; i--){
      QLabel* label = new QLabel();
      label->setText(rank[i].second.arg(pos - i + 1).arg(rank[i].first));
      layout->addWidget(label);
    }
    ui->stackedWidget->setCurrentIndex(3);
  });
  QObject::connect(ui->game_play_ret_btn,&QPushButton::clicked,[&](){
    //强制停止
    timer->stop();
    ui->stackedWidget->setCurrentIndex(0);
  });
  QObject::connect(ui->game_rank_ret_btn,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(0);
  });
  QObject::connect(ui->game_rule_ret_btn,&QPushButton::clicked,[&](){
    ui->stackedWidget->setCurrentIndex(0);
  });
  QObject::connect(ui->game_exi_btn,&QPushButton::clicked,[&](){
    hide();
  });
  QObject::connect(timer, &QTimer::timeout, this, [=]() mutable {
    // mutable关键字使Lambda表达式可变
    nowTime--; // 每次定时器超时，时间减少一秒
    ui->game_timer_label->setText(QString::number(nowTime) + "s");
    // 如果需要在时间为0时停止定时器，可以添加以下逻辑
    if (nowTime == 0) {
      timer->stop();
      //游戏结束
      Tip->set_content("游戏结束",QString("本局游戏得分: %1 分").arg(now_score));
      Tip->show();
      rank.append({now_score,QString("%1. ") + QString("%2 得分 %3 分").arg(name)});
      std::sort(rank.begin(),rank.end());
      ui->stackedWidget->setCurrentIndex(0);
    }
  });
  QObject::connect(ui->game_line_edit,&QLineEdit::returnPressed,[&](){
    auto input = ui->game_line_edit->text();
    ui->game_line_edit->clear();
    if(input.toLower() == ui->game_word_label->text().toLower()){
      now_score += input.length();
      ui->game_score_label->setText(QString::number(now_score));
      ui->game_word_label->setText(words[idx].english);
      idx = (idx + 1) % words.size();
    }
    else{
      now_score -= 4;
      ui->game_score_label->setText(QString::number(now_score));
    }
  });
}

void game::setup_words()
{
  auto sys = resource_manager::getInstance()->get_system();
  auto student = resource_manager::getInstance()->get_student();
  words = sys.generateWords(student.GetId(), 0, 20);
  // 若没有单词则无法开始游戏。
}

void game::clearlayout(QBoxLayout *lo)
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


