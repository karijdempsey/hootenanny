/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ValueAggregatorJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ValueAggregatorJs)

Persistent<Function> ValueAggregatorJs::_constructor;

ValueAggregatorJs::ValueAggregatorJs()
{
}

ValueAggregatorJs::~ValueAggregatorJs()
{
}

void ValueAggregatorJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(ValueAggregator::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol(opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
        FunctionTemplate::New(toString)->GetFunction());
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  String::New(ValueAggregator::className().data()));

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol(n), constructor);
  }
}

Handle<Value> ValueAggregatorJs::New(const Arguments& args) {
  HandleScope scope;

  QString className = str(args.This()->GetConstructorName());

  ValueAggregator* c = Factory::getInstance().constructObject<ValueAggregator>(className);
  ValueAggregatorJs* obj = new ValueAggregatorJs(ValueAggregatorPtr(c));
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<ValueAggregator>(c, args);

  return args.This();
}

Handle<Value> ValueAggregatorJs::toString(const Arguments& args)
{
  HandleScope scope;

  ValueAggregatorPtr sd = toCpp<ValueAggregatorPtr>(args.This());

  return scope.Close(toV8(sd->toString()));
}

}

