/*
 * qtfastjson (QtFastJsonObject.h)
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

#ifndef QTFASTJSONOBJECT_H
#define QTFASTJSONOBJECT_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QVariant>

class QtFastJsonObject : public QObject
{
    Q_OBJECT
public:
    enum JsonValType {
        List = 0,
        Array = 1,
        Num = 2,
        Str = 3,
        Bool = 4,
        Null = 5
    };

    explicit QtFastJsonObject(QtFastJsonObject *parent = 0,QVariant key = "", bool isFJO = true);
    explicit QtFastJsonObject(QObject *parent = 0,QVariant key = "");
    inline QtFastJsonObject* jsonParent() { return myParent; }

    inline QtFastJsonObject* at(const QString key) { return childItems->value(key); }
    inline QtFastJsonObject* at(const int key) { return childItems->value(QString::number(key)); }

    inline QVariant key() { return myKey; }
    inline void setKey(QVariant newKey) { myKey = newKey; }

    inline void insertChild(QString key, QtFastJsonObject* object) { childItems->insert(key,object); }
    inline void insertChild(int key, QtFastJsonObject* object) { childItems->insert(QString::number(key),object); }

    inline void removeChild(QString key) { childItems->remove(key); }
    inline void removeChild(int key) { childItems->remove(QString::number(key)); }
    inline void removeChild(QtFastJsonObject* object) { childItems->remove(childItems->key(object)); }

    inline QList<QtFastJsonObject*> getChildItems() { return childItems->values(); }
    /* TODO: The above returns the items in a sorted manner, NOT the inserted manner. */

    inline int childCount() { return childItems->size(); }

    inline void setVariantValue(QVariant value) { myVariantValue = value; }
    inline QVariant variant() { return myVariantValue; }

    inline JsonValType type() { return myValType; }
    inline void setType(JsonValType newType) { myValType = newType; }

    inline QString toStr() { return myVariantValue.toString(); }
    inline int toInt() { return myVariantValue.toInt(); }

private:
    QHash<QString,QtFastJsonObject*>* childItems;

    QVariant myKey;
    QVariant myVariantValue;
    JsonValType myValType;

    QtFastJsonObject* myParent;
};

#endif // QTFASTJSONOBJECT_H
