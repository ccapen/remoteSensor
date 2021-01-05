#include "mainw.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{//解决汉字乱码问题
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    MainW w;
    w.show();
    return a.exec();
}
