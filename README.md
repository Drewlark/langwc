# LWC (Language Without Compassion) or any other acronym that fits in LangWC)
LWC is a scripting language that is written entirely from scratch with an interpreter written in standard C++14 (though I am considering opening up the gates to everything in C++17). It is a fairly naive approach, as I have somewhat intentionally not researched implementations of any other scripting languages. It is a complete rewrite of an earlier version which was extremely slow and even more naive. It only uses longs (will probably move to 64 bit integers soon) as a datatype, though other datatypes will eventually be virtualized (this is already the case on some level)

It is my second attempt at designing a scripting language. The first language worked entirely on top of the input string and was very slow. This new version has been designed with run-time performance deeply in mind. It is very much a work in progress.

At the time of writing the language currently has only assignment, arithmetic, loops, and conditional statements. Some features may need manual activation (check lwc_builtins.h and lwc_Parser.h) for the time being.

I've been using VS2019 to write this, but there is no reason it can't be compiled with basically any other compiler on any OS, though I haven't bothered to try gcc or anything yet. Regardless, the code base is pretty simple, and requires no external dependencies, just the STL.

My speed goal is somewhere faster than python. Right now in a very basic speed test (thousands of small assignments and arithmetic) the language is marginally faster (10-20%). I'll try to keep it here, even as more features are added.

I don't have an exact use case in mind, it really is more of a challenge for myself. It has maybe one cool feature so far, and that really is kind of a weird intregated macro system I guess.

Oh, and currently there is **no garbage collection**, that is next on my agenda however.
