# LWC (language written crappily or anything else that fits in LWC)
LWC is a scripting language that is written entirely from scratch with an interpreter written in standard C++11. It does not use LLVM, just an interpreter written by me. It is a complete rewrite of an earlier version which was quite slow. It only uses longs as a datatype, though other datatypes will eventually be virtualized. 

It is my second attempt at designing a scripting language. The first language worked entirely on top of the input string and was very slow. This new version has been designed with run-time performance deeply in mind. It is very much a work in progress.

At the time of writing the language currently has only assignment, arithmetic, conditionals and looping implemented. Functions are coming very soon, followed by arrays and then other datatypes. Class-like structures would be nice but it will depend how viable it is, and I am a student doing this for fun with not much time on my hands.