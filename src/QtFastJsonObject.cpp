/*
 * qtfastjson (QtFastJsonObject.cpp)
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

#include "QtFastJsonObject.h"

QtFastJsonObject::QtFastJsonObject(QObject *parent, QVariant key) :
    QObject(parent)
{
    myKey = key;
    childItems = new QHash<QString,QtFastJsonObject*>();
    myVariantValue = "";
}
QtFastJsonObject::QtFastJsonObject(QtFastJsonObject *parent, QVariant key, bool isFJO) :
    QObject(parent)
{
    myKey = key;
    myParent = parent;
    childItems = new QHash<QString,QtFastJsonObject*>();
    myVariantValue = "";
}

QtFastJsonObject* QtFastJsonObject::addChild(QVariant key, QVariant value)
{
    QtFastJsonObject* newObj = new QtFastJsonObject(this,key,true);
    newObj->setVariantValue(value);

    childItems->insert(key.toString(),newObj);
    childItems_ordOfIns.insert(childItems_ordOfIns.count(),newObj);

    return newObj;
}

void QtFastJsonObject::removeChild(QVariant key)
{
    QtFastJsonObject *objToRem = childItems->value(key.toString());
    childItems->remove(key.toString());
    childItems_ordOfIns.removeOne(objToRem);
}
void QtFastJsonObject::removeChild(QtFastJsonObject *object)
{
    childItems->remove(childItems->key(object));
    childItems_ordOfIns.removeOne(object);
}
