#Qt Fast JSON
A fast way to manipulate JSON data from Qt under the MIT license.
Currently, it only *reads* JSON.

This is still in alpha stage, so don't use it without testing it first.
Please report any bugs!

Parsing JSON in Qt previously had to be done using QJson or a C/C++ JSON library.
In order to read the following JSON...
<pre>
	{
	   "encoding" : "UTF-8",
	   "plug-ins" : [
		   "python",
		   "c++",
		   "ruby"
		  ],
	   "indent" : { "length" : 3, "use_space" : true }
	}
</pre>
...while using QJson, you would have to do something like this at the least:
<pre>
	QJson::Parser parser;
	bool* ok;
	QVariantMap result = parser.parse("{ ...jsondata... }", &ok).toMap();
	int length = result["indent"].toMap()["length"].toInt();
</pre>
but if you use QtFastJson you only have to do the following:
	QtFastJsonDoc* jsonDoc = new QtFastJsonDoc();
    jsonDoc->readJSON("json_file.json",true); // Returns a bool of OK.
    int length = jsonDoc->at("indent")->at("length")->toInt();
</pre>
I intend to add support for using [] instead of ->at() soon.
