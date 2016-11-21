# fcat
An experiment to file discovery from within a given root folder.

The program is enumerating the sub-directories using a stack algorythm instead of a recursive one.

This is only an experiment. The DirWorm class can be used but it has a lot of comments/uncleared/unused functions etc.

There is the <a href="https://github.com/easylogging/easyloggingpp">easyloggingpp</a> header which is a very useful logger framework.

Use -std=c++11 and -pthread when building.

I had issues building it in Eclipse neon1.a (4.6.1) using the MingW g++. It found _wstat function as undeclared. But building it outside of Eclipse with
<pre>g++ DirWorm.cpp main.cpp FileProperties.cpp ExportBehavior.cpp ExportToPlainText.cpp Utf8Tool.cpp -o fcat.exe</pre>
everything was built successfully.