#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <iostream>

#include <Qson.h>

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

bool qFilePutContents(QString fileName, QString data)
{
    QFile f(fileName);
    if(!f.open(QFile::WriteOnly)) {
        return false;
    }
    f.write(data.toUtf8());
    f.close();
    return true;
}

bool qFileExists(QString filename)
{
    QFile f(filename);
    return f.exists();
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

bool compareStringLists(QStringList a, QStringList b)
{
    if(a.size() != b.size()) { return false; }
    for(int i = 0; i < a.size(); i++) {
        if(a.at(i) != b.at(i)) { return false; }
    }
    return true;
}

class TestDriver : public QObject
{
    Q_OBJECT
public:
   TestDriver() : QObject() { }

    bool runReadTest(QString file)
    {
        QByteArray testcase = qFileGetContents("testcases/"+file+".json");
        QStringList expected = QString::fromUtf8(qFileGetContents("testresults/"+file+".log")).split("\n");
        testResult.clear();

        QsonDoc d; connect(&d,SIGNAL(logEvent(QString)),this,SLOT(logEvent(QString)));
        d.readJSON(QString::fromUtf8(testcase));
        QCoreApplication::processEvents(); // So there aren't any stray signals
#ifdef SAVE_RESULTS
        qFilePutContents("testresults/"+file+".log", testResult.join('\n'));
#endif

        return outResult(compareStringLists(testResult, expected),"read",file);
    }

    bool runWriteTest(QString file)
    {
        return true;
    }

public slots:
    void logEvent(QString val) { testResult.append(val); }

private:
    QStringList testResult;
};


int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    TestDriver t;

    int failed = 0, passed = 0;
    QStringList items = QString::fromUtf8(qFileGetContents("testcases.lst")).split("\n");
    foreach(QString item, items) {
        if(!qFileExists("testcases/"+item+".json")) { continue; }
        if(t.runReadTest(item)) { passed++; } else { failed++; }
        if(t.runWriteTest(item)) { passed++; } else { failed++; }
    }

    if(failed == 0) { std::cout << "\nSUCCESS! All tests passed!"; return 0; }
    else { std::cout << "\nOHNOES! Some tests failed!"; return 1; }
}

#include "testdriver.moc"
