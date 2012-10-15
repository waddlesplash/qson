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
        Variant = 2
    };

    explicit QtFastJsonObject(QtFastJsonObject *parent = 0,QVariant key = "", bool isFJO = true);
    explicit QtFastJsonObject(QObject *parent = 0,QVariant key = "");
    inline QtFastJsonObject* jsonParent() { return myParent; }

    inline QtFastJsonObject* at(const QVariant key) { return childItems.value(key.toString()); }

    inline QVariant key() { return myKey; }
    inline void setKey(QVariant newKey) { myKey = newKey; }

    /* Creates and adds a child item and then returns it. */
    QtFastJsonObject* addChild(QVariant key, QVariant value = "");

    void removeChild(QVariant key);
    void removeChild(QtFastJsonObject* object);

    inline QList<QtFastJsonObject*> getChildItems() { return childItems_ordOfIns; }

    inline int childCount() { return childItems.size(); }

    /* The JsonValType indicates what type the value is: an array, map, or string. */
    inline JsonValType type() { return myValType; }
    inline void setType(JsonValType newType) { myValType = newType; }

    /* These simply call variantValue.to...() and are only here for convenience */
    inline QString toStr() { return myVariantValue.toString(); }
    inline int toInt() { return myVariantValue.toInt(); }

    inline QVariant variant() { return myVariantValue; }
    inline void setVariantValue(QVariant value) { myVariantValue = value; }

private:
    QHash<QString,QtFastJsonObject*> childItems;
    QList<QtFastJsonObject*> childItems_ordOfIns;

    QVariant myKey;
    QVariant myVariantValue;
    JsonValType myValType;

    QtFastJsonObject* myParent;
};

#endif // QTFASTJSONOBJECT_H
