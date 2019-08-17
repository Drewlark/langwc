# LWC (language W___ C___ or any other acronym that fits in LWC)
LWC is a scripting language that is written entirely from scratch with an interpreter written in standard C14. It is a fairly naive approach, as I have somewhat intentionally not researched implementations of any other scripting languages. It is a complete rewrite of an earlier version which was extremely slow and even more naive. It only uses longs (will probably move to 64 bit integers soon) as a datatype, though other datatypes will eventually be virtualized (this is already the case on some level)

It is my second attempt at designing a scripting language. The first language worked entirely on top of the input string and was very slow. This new version has been designed with run-time performance deeply in mind. It is very much a work in progress.

At the time of writing the language currently has only assignment, arithmetic, loops, and conditional statements. Some features may need manual activation (check lwc_builtins.h and lwc_Parser.h) for the time being.

I've been using VS2019 to write this, but there is no reason it can't be compiled with basically any other compiler on any OS, though I haven't bothered to try g++ or anything yet. Regardless, the code base is pretty simple, and requires no external dependencies.

My speed goal is somewhere faster than python. I'd like to be quite a bit faster, but at the moment my malloc calls are pretty ridiculous so it runs very roughly around 0-20% slower than python 3.7.

I don't have an exact use case in mind, it really is more of a challenge for myself. It has maybe one cool feature so far, and that really is kind of a weird intregated macro system I guess.
