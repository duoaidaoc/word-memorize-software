#ifndef QUESTION_H
  #define QUESTION_H

#include <QWidget>
#include "util.h"
#include "word.h"

#include <queue>
#include <map>

namespace Ui {
class question;
}

// 单词列表传入，经过客户端的一系列交互后，传出单词的学习情况 bool (学会/未学会)

class question : public QWidget
{
  Q_OBJECT
public:
  enum state{
    free, // 没有为question窗口设置题目列表
    busy // question窗口的答题动作进行中
  };
  explicit question(QWidget *parent = nullptr);
  ~question();
  // 将窗口设置成选择题，传入英文，a选项，b选项，c选项，正确的选项序号(1表示a，2表示b，3表示c)
  void set_selection_question(const QString &english, const QString &A, const QString &B, const QString &C, int _correctitem);
  // 将窗口设置成填空题, 传入中文，正确的英文
  void set_fillword_question(const QString &english, const QString &chinese);
  // 返回question 窗口的状态
  state get_status();
  // 外部设置question窗口状态用于强行终止
  void shutdown();

  void set_task_id(qint64 task_id);

  qint64 get_task_id();
protected:
  // 窗口绘制事件重写,ui相关
  void paintEvent(QPaintEvent* evt)override;
private:
  Ui::question *ui; // ui框架
  QImage* bg; // 背景图片
  int correctitem; // 正确选项
  QString correct_english; // 英文

  std::queue<ques> questions;         // 问题列表
  std::map<qint64, int> finished_cnt; // 完成计数
  state st;                           // 窗口忙或空闲。
  qint64 task_id;                     // 任务id

  void set_connection(); // 初始化

signals:
  // 题目做完时触发finish()信号
  void finish();
  // 一个单词学会时发出word_learnt()信号
  void word_learnt(qint64 word_id,qint64 task_id);
  void word_learnt_sys(qint64 word_id);

public:
  // 设置问题列表
  void set_ques(const std::vector<db::WordInfo> &_words);
  // 从题目列表中拿出一题来设置窗口的题目
  void start();

private:
  // 答题函数在回答完毕时自动调用done
  void done(bool ok);
};

#endif // QUESTION_H
