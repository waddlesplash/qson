/*
 * qtfastjson (QtFastJson.h)
 *  Part of QtFastJSON (http://github.com/waddlesplash/qtfastjson/).
 *
 * Copyright (c) 2012 WaddleSplash
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
*/

#ifndef QTFASTJSON_H
#define QTFASTJSON_H

#include <QObject>
#include <QString>
#include "QtFastJsonObject.h"

class QtFastJsonDoc : public QtFastJsonObject
{
    Q_OBJECT
public:
    explicit QtFastJsonDoc(QObject *parent = 0);
    bool readJSON(QString fileName, bool isFile);
    bool readJSON(const QString data);

private:
    bool parser_lexer();
    QString parser_readStr();
    QVariant parser_readNum();

    QChar json_curChar;
    int json_loc;
    int json_length;
    QString jsonDat;
};

#endif // QTFASTJSON_H
