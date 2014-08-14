#include "ReadTester.h"

#include <iostream>

#include "TestDriver.h"

void ReadTester::handleObject(QsonObj* obj, QStringList* data, int indentLevel)
{
    QList<QsonObj*> children = obj->children();
    for (int i = 0; i < children.size(); i++) {
        QsonObj* obj = children.at(i);
        if (obj->type() == QsonObj::Variant)
            data->append(QString("%1" "Variant: %2, %3").arg(indents(indentLevel)).arg(obj->key().toString()).arg(obj->toStr()));
        else if (obj->type() == QsonObj::Array) {
            data->append(QString("%1" "Array: %2").arg(indents(indentLevel)).arg(obj->key().toString()));
            handleObject(obj, data, indentLevel + 1);
        } else if (obj->type() == QsonObj::List) {
            data->append(QString("%1" "Map: %2").arg(indents(indentLevel)).arg(obj->key().toString()));
            handleObject(obj, data, indentLevel + 1);
        }
    }
}

bool ReadTester::Run(QString fileName)
{
    QByteArray testcase = qFileGetContents("testcases/" + fileName + ".json");
    QStringList expected = QString::fromUtf8(qFileGetContents("testresults/" + fileName + ".log")).split("\n");

    QsonDoc doc;
    doc.readJSON(QString::fromUtf8(testcase));

    QStringList data;
    data.append("Map: <root>");
    handleObject(&doc, &data, 1);
    bool result = compareStringLists(expected, data);

#ifdef UPDATE_RESULTS
    qFilePutContents("testresults/" + fileName + ".log", data.join("\n"));
#endif

    outputResult(result, "read", fileName);
    return result;
}
