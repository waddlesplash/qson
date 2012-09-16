#Qt Fast JSON
A fast way to manipulate JSON data from Qt under the MIT license.
Currently, it only *reads* JSON.

This is still in alpha stage, so don't use it without testing it first.
Please report any bugs!

Parsing JSON in Qt previously had to be done using QJson or a C/C++ JSON library.
In order to read the following JSON...
```json
{
"encoding" : "UTF-8",
"plug-ins" : [
	"python",
	"c++",
	"ruby"
	],
"indent" : { "length" : 3, "use_space" : true }
}
```
...while using QJson, you would have to do something like this at the least:
```cpp
QJson::Parser parser;
bool* ok;
QVariantMap result = parser.parse("{ ...jsondata... }", &ok).toMap();
int length = result["indent"].toMap()["length"].toInt();
```
but if you use QtFastJson you only have to do the following:
```cpp
QtFastJsonDoc* jsonDoc = new QtFastJsonDoc();
jsonDoc->readJSON("json_file.json",true); // Returns a bool of how it made out.
int length = jsonDoc->at("indent")->at("length")->toInt();
```
