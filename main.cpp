#include "login.h"
#include "appinit.h"
#include <QApplication>
#include "QFile"
#include "student_main.h"
#include <QDir>
#include "resource_manager.h"
#include "role.h"
#include "system.h"
#include "teacher_main.h"
#include "tip.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  auto man = resource_manager::getInstance();

//  ========================================== 数据库测试 ==========================================//
//   创建数据库对象
  man->init_database();
  auto &database = man->get_database();
//  qDebug() << "======== 开始数据库测试 ========\n";
//  qDebug() << "-------- 创建数据库 --------\n";

  // 初始化服务器系统，服务器端。
  man->init_system();
  auto &system = man->get_system();
  qDebug() << "-------- 创建教师信息 --------\n";
  db::Teacher teacher(database);
  teacher.SetId(3);
  teacher.SetName("John");
  teacher.SetPassword("abcdefg");
  teacher.SetProfilePhotoUrl("djdjdjjdjd");
  teacher.registerRole();

  db::Teacher teacher2(database);
  teacher2.SetId(9);
  teacher2.SetName("Alic");
  teacher2.SetPassword("ieieie");
  teacher2.SetProfilePhotoUrl("cncnncnc");
  teacher2.registerRole();

  teacher.cancelRole();
  teacher2.displayInfo();

  qDebug() << "-------- 创建学生信息 --------\n";
  // 创建学生对象并注册
  db::Student student(database);
  student.SetId(4);
  student.SetName("Alice");
  student.SetPassword("djdjdjjdjddddddd");
  student.SetProfilePhotoUrl("eeueueuL");
  student.registerRole();

  db::Student student2(database);
  student2.SetId(89);
  student2.SetName("MIKE");
  student2.SetPassword("eieiiedjjcncn");
  student2.SetProfilePhotoUrl("llwlwl");
  student2.registerRole();

  student.cancelRole();
  student2.displayInfo();

  db::Student student3(database);
  student3.SetId(90);
  student3.SetName("Leon");
  student3.SetPassword("eieiiedjjcncn");
  student3.SetProfilePhotoUrl("llwlwl");
  student3.registerRole();
  student3.displayInfo();

  qDebug() << "-------- 测试老师创建班级 --------\n";
  teacher2.createClass(2023, "english1", "nnnnnnn");
  teacher2.createClass(2021, "english2", "mmmmmmm");
  teacher2.createClass(2024, "english3", "mmmmmmm");
  teacher2.createClass(2026, "english4", "mmmmmmm");
  teacher2.createClass(2027, "english5", "mmmmmmm");
  teacher2.createClass(2028, "english6", "mmmmmmm");
  teacher2.createClass(2029, "english7", "mmmmmmm");
  teacher2.createClass(2020, "english8", "mmmmmmm");
  teacher2.createClass(100000, "english10", "mmmmmmm");

  qDebug() << "-------- 测试学生加入班级 --------\n";
  student2.joinClass(2023);                 // todo: 不能重复添加，是在db出解决还是逻辑中解决。
  student2.joinClass(2021);
  student2.joinClass(2020);
  student2.joinClass(2028);
  student2.joinClass(2029);
  student3.joinClass(2020);
  student2.joinClass(2020);               // 会报错，并且导致崩溃

  qDebug() << "-------- 测试老师删除班级 --------\n";
  // 需要支持级联删除。
  // 老师删除对应如下操作：
  // 1. class表格里面删除
  // 2. teacherclass表格里面删除
  // 3. studentclass表格里面删除
  teacher2.deleteClass(2028);

  qDebug() << "-------- 测试学生退出班级 --------\n";
  // student2.leaveClass(2021);
  teacher2.deleteClass(2029);
  teacher.deleteClass(2020);

  qDebug() << "-------- 测试老师给班级添加计划 --------\n";
  //teacher2.createTask(2, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

  qDebug() << "-------- 测试老师给班级去除计划 --------\n";
  //teacher2.deleteTask(2, 2020);
  //teacher2.createTask(3, 2024, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());
  //teacher2.createTask(6, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());
  //teacher2.createTask(5, 2020, QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

  qDebug() << "-------- 测试老师给计划添加单词 --------\n";
  /*
  teacher2.createTaskWord(5, 11, "abandon", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(6, 12, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(5, 13, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(6, 14, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(5, 15, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(6, 11, "abandon", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(5, 12, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(6, 13, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(6, 14, "accccc", "抛弃", "aaaaa", "bbbbb");
  teacher2.createTaskWord(5, 15, "accccc", "抛弃", "aaaaa", "bbbbb");
  */

  qDebug() <<"-------- 测试系统生成自带的题库 --------\n";
  //system.createWordBank(1, "牛津", "djdjdjjdjdjdjdj");
  //system.importWordBank(1, 16, "oxford", "牛津", "ddddd", "dddddddd");
  //system.importWordBank(1, 11, "abandon", "抛弃", "aaaaa", "bbbbb");
  system.importLocalWords("cet4");
  qint64 id = system.checkAlreadyInWords("absolutely");

  qDebug() << "-------- 教师import进入task单词 --------\n";
  QList<QString> englishList;
  englishList.append("abandon");
  englishList.append("axe");
  englishList.append("arouse");
  englishList.append("bb");
  englishList.append("ax");

  QList<QString> englishList2;
  englishList2.append("abandon");
  englishList2.append("bbbbb");
  englishList2.append("bbb");
  englishList2.append("bb");

  qint64 task_id = teacher2.importTaskWordBank(englishList);
  qDebug() << ">>>>>>>>>>>>>>>>>>>> " <<task_id;

  if(task_id >= 0)
    teacher2.createTask(task_id, 2020, "aaaa", QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());

  task_id = teacher2.importTaskWordBank(englishList2);
  qDebug() << ">>>>>>>>>>>>>>>>>>>> " <<task_id;
  if(task_id >= 0)
    teacher2.createTask(task_id, 2020, "aaaa", QDateTime::currentDateTime(), QDateTime::currentDateTime(), QTime::currentTime());


  qDebug() << "-------- 教师学生登录 --------\n";
  qDebug() << system.returnTeacherPassword(9) << "\n";
  qDebug() << system.returnTeacherPassword(3) << "\n";
  qDebug() << system.returnStudentPassword(89) << "\n";
  qDebug() << system.returnStudentPassword(90) << "\n";

  qDebug() << "-------- 测试学生显示所在的班级 --------\n";
  // 返回的是QList<QPair<qint64, QString>>
  student2.infoStudentClass();

  qDebug() << "-------- 测试学生显示所在的班级的详细信息 --------\n";
  // param1: Class ID
  // 返回的是该class的老师信息。QList<TeacherInfo>
  student2.infoClassDetails(2020);
  // 返回的是该class的学生成员信息。QList<StudentInfo>
  student2.infoClassMembers(2020);

  qDebug() << "-------- ================================ --------\n";
  teacher2.infoTeacherClass();
  teacher2.infoClassDetails(2020);
  // 返回的是该class的学生成员信息。QList<StudentInfo>
  teacher2.infoClassMembers(2020);

  qDebug() << "-------- 测试学生背任务里面的单词 --------\n";
  qDebug() << "***** 班级查任务 *****\n";
  // ToDo: （1）班级查任务
  // 返回QList<TaskInfo>
  student2.infoTaskInClass(2020);
  // ToDo: （2）显示所有指定任务里面的单词
  // 返回QList<WordInfo>
  qDebug() << "***** 任务查单词 *****\n";
  student2.infoWordsInTask(6);
  student2.infoWordsInTask(5);
  // ToDo: （3）学生学习单词（单词id）
  student2.learnWordRecord(0);
  student2.learnWordRecord(92);

  qDebug() << "******************** 测试学生退出班级又加入班级 ***************************";
  student2.leaveClass(2020);
  student2.joinClass(2020);

  qDebug() << "******************** 输出2020班级的task ***************************";
  student2.infoTaskInClass(2020);
  qDebug() << "******************** 输出2020班级的task ***************************";

  // ToDo: （4）显示总体完成情况
  double result = student2.infoTaskCondition(student2.GetId(), task_id);
  qDebug() << "******************** " << result << " ***************************";

  qDebug() << "-------- 测试老师查看学习情况 --------\n";
  // ToDo: （1）班级查任务
  teacher2.infoTaskInClass(2020);
  // ToDo: （2）显示所有单词
  teacher2.infoWordsInTask(6);
  teacher2.infoWordsInTask(5);
  // ToDo: （3）显示总体完成情况

  qDebug() << "-------- 学生学习系统单词 -------\n";
  student2.learnSysWordRecord(6);
  student2.learnSysWordRecord(7);
  student2.learnSysWordRecord(8);
  student2.learnSysWordRecord(9);
  student2.learnSysWordRecord(10);
  student2.learnSysWordRecord(11);
  student2.learnSysWordRecord(12);
  student2.learnSysWordRecord(13);

  QList<db::WordInfo> wordList = system.generateWords(student2.GetId(), 0);
  for (const auto& wordInfo : wordList) {
    qDebug() << "Word ID:" << wordInfo.word_id;
    qDebug() << "English:" << wordInfo.english;
    qDebug() << "Chinese:" << wordInfo.chinese;
    qDebug() << "Phonetic:" << wordInfo.phonetic;
    qDebug() << "Audio URL:" << wordInfo.audio_url;
    qDebug() << "-------------------------";
  }

  qDebug() << "11111111111111111111111111111111111111111111111111111111111111111111111111";
  wordList = system.generateWords(student2.GetId(), 0);
  for (const auto& wordInfo : wordList) {
    qDebug() << "Word ID:" << wordInfo.word_id;
    qDebug() << "English:" << wordInfo.english;
    qDebug() << "Chinese:" << wordInfo.chinese;
    qDebug() << "Phonetic:" << wordInfo.phonetic;
    qDebug() << "Audio URL:" << wordInfo.audio_url;
    qDebug() << "-------------------------";
  }

  qDebug() << system.returnLearnedRate(student2.GetId());
  qDebug() << system.returnLearnedRateForWordBank(student2.GetId(), 1);

  QList<db::WordBankInfo> banklist = system.returnWordBankInfo();
  qDebug() << "-------- bankList测试 -------\n";
  for(const auto &bank : banklist) {
    student2.learnWordBanks(bank.id);
    qDebug() << student2.returnStudentBank();
    qDebug() << bank.id << bank.name << bank.picture_url;
  }

  wordList = system.returnWordBankList(0);
  for(const auto &word : wordList) {
    qDebug() << word.word_id << word.chinese;
  }
  qDebug() << "-------- 数据库测试结束 -------\n";

//========================================== 前端测试 ==========================================//
  a.setFont(man->get_glob_font());

  QFile file(man->get_glob_stylecss_path());
  if (file.open(QFile::ReadOnly)) {
      QString qss = QLatin1String(file.readAll());
      qApp->setStyleSheet(qss);
      file.close();
  }

  man->init_init_student();
  man->init_init_teacher();

  login* loginpage = new login();
  student_main* stm = new student_main();
  teacher_main* tem = new teacher_main();
  loginpage->show();
  //  tem->show();
  //  stm->show();
  QObject::connect(loginpage,&login::turn_to,[&](bool interface){
    if(interface){
      tem->data_setup();
      tem->show();
    }
    else{
      stm->data_setup();
      stm->show();
    }
  });

  AppInit::Instance()->start();
  return a.exec();
}
