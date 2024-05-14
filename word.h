#include "QString"
#include "qimage.h"
#ifndef WORD_H
#define WORD_H

typedef struct Word{
  qint64 word_id;
  QString eng;
  QString sound_url;
  QString chn;
  QString info;
}Word;

typedef struct CClass{
  qint64 id;
  QString name;
  QString cue;
}CClass;

typedef struct Task{
  QString name;
  qint64 tid;
}Task;

typedef struct Student{
  qint64 id;
  QString Password;
  QString name;
}Student;

typedef struct Teacher{
  qint64 id;
  QString Password;
  QString name;
}Teacher;

typedef struct ques{
  qint64 word_id;   // 单词id
  bool is_sel;      // 是选择题吗？
  bool is_fill;     // 是填空题吗？
  QString A,B,C;    // 选择题的a,b,c选项
  int right;        // 选择题的正确选项(1-a,2-b,3-c)
  QString eng,chn;  // 填空题的中英文
}ques;

typedef struct detailed_info{
  QString nickname;
  QString age;
  QString school;
  QString phone;
  QString say;
  QImage* img;
}detailed_info;

#endif // WORD_H
