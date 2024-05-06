#include "QString"
#ifndef WORD_H
#define WORD_H

typedef struct Word{
  QString eng;
  QString sound_url;
  QString info;
}Word;

typedef struct CClass{
  qint64 id;
  QString name;
  QString cue;
}CClass;

typedef struct Task{
  QString name;
}Task;


#endif // WORD_H
