
#include "resource_manager.h"

QString resource_manager::bg_pic_randomselect(){
    return randomselect(glob_bg_path);
}
QString resource_manager::pf_pic_randomselect(){
    return randomselect(glob_profile_picture_path);
}
QString resource_manager::audio_select(const QString &eng){
    return glob_audio_path + "/" + eng + ".mp3";
}

QString resource_manager::get_noise_pic()
{
    return QString("../word-memorize-software/pics/noise.png");
}

QString resource_manager::get_glob_stylecss_path()
{
    return glob_stylecss_path;
}

QString resource_manager::get_glob_bg_path()
{
    return glob_bg_path;
}

QString resource_manager::get_glob_hello()
{
    return glob_hello;
}

QString resource_manager::get_glob_profile_picture_path()
{
    return glob_profile_picture_path;
}

QString resource_manager::get_glob_audio_path()
{
    return glob_audio_path;
}

QFont resource_manager::get_glob_font()
{
    return glob_font;
}

QColor resource_manager::get_color()
{
    if (sel < 6 || sel >= 19){
      return QColor(0,0,0,63);
    }
    else{
      return QColor(255,255,255,63);
    }
}
resource_manager* resource_manager::getInstance() {
    if(!instance){
        instance = new resource_manager();
    }
    return instance;
}
QString resource_manager::randomselect(QString path)
{
    QDir dir(path);
    QStringList filters;
    filters << "*.jpg" << "*.png"; // 只筛选 jpg 和 png 格式的图片
    QStringList imageFiles = dir.entryList(filters, QDir::Files);

    if (!imageFiles.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(imageFiles.size());
        path = dir.filePath(imageFiles[randomIndex]);
    }
    return path;
}

resource_manager::resource_manager()
{
    QTime current_time = QTime::currentTime();
    sel = current_time.hour();
    sel = 20;
    if (sel < 6 || sel >= 19) {
        //晚上
        glob_stylecss_path = "../word-memorize-software/night.css";
        glob_bg_path = "../word-memorize-software/pics/night_bg";
        glob_hello = "晚上好啊！";
    } else if (sel < 14) {
        //上午
        glob_stylecss_path = "../word-memorize-software/day.css";
        glob_bg_path = "../word-memorize-software/pics/morning_bg";
        glob_hello = "上午好啊！";
    } else if (sel < 19) {
        //下午
        glob_stylecss_path = "../word-memorize-software/day.css";
        glob_bg_path = "../word-memorize-software/pics/afternoon_bg";
        glob_hello = "下午好啊！";
    }
    QFile dFontFile("../word-memorize-software/ttf/shangshou-monster.ttf");
    dFontFile.open(QIODevice::ReadOnly);
    int nFontId = QFontDatabase::addApplicationFontFromData(dFontFile.readAll());
    QStringList lFontFamily = QFontDatabase::applicationFontFamilies(nFontId);
    glob_font = QFont(lFontFamily.at(0), 15);

    glob_profile_picture_path = "../word-memorize-software/pics/login_label";
    glob_audio_path = "../word-memorize-software/audios";
}

resource_manager* resource_manager::instance = NULL;

