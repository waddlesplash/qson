#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QByteArray>

#include <iostream>

QByteArray qFileGetContents(QString fileName)
{
    QFile f(fileName);
    if(!f.open(QFile::ReadOnly)) {
        return "";
    }
    QByteArray ret = f.readAll();
    f.close();
    return ret;
}

bool outResult(bool result, QString type, QString name)
{
    if(result) {
        std::cout << "\033[1;32mPASS\033[0m ";
    } else {
        std::cout << "\033[1;31mFAIL\033[0m ";
    }
    std::cout << type.toUtf8().constData();
    std::cout << " ";
    std::cout << name.toUtf8().constData();
    std::cout << "\n";
    return result;
}

bool writeTest(QString fileName)
{
    return false;
}

bool readTest(QString fileName)
{
    return true;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    bool res = true;
    QStringList items = QString::fromUtf8(qFileGetContents("testcases.lst")).split("\n");
    foreach(QString item, items) {
        res = res && outResult(readTest(item), "read", item);
        res = res && outResult(writeTest(item), "write", item);
    }

    if(res) { return 0; }
    else { qWarning("\nOHNOES! Some tests failed!"); return 1; }
}
