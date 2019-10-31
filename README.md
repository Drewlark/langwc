# LWC (Language Without Compassion) or any other acronym that fits in LangWC)
LangWC is a scripting language that is written entirely from scratch with an interpreter written in standard C++14 (though I am considering opening up the gates to everything in C++17 - the barrier is embedded compatibility). It is a fairly naive approach, as I have somewhat intentionally not researched implementations of any other scripting languages. It is a complete rewrite of an earlier version which was extremely slow and even more naive. Currently only ints and elastics (a strange variety of lambas of my own invention) are available as datatypes. However datatypes are very easy to add, it just has not been my priority.

It is my second attempt at designing a scripting language. The first language worked entirely on top of the input string and was very slow. This new version has been designed with run-time performance deeply in mind. It is very much a work in progress.

At the time of writing the language currently has only assignment, arithmetic, loops, and conditional statements. Some features may need manual activation (check lwc_builtins.h and lwc_Parser.h) for the time being.

It is 3.5 times faster than equivalent Python3 code when compiled with gcc. It is 50% faster than Python3 when compiled with MSVC. To compile with GCC, I recommend the following: `g++ main.cpp builtins.cpp typedefs.cpp lwc_Parser.cpp lwc_Lexing.cpp -o a.out -O3 -pthread`

I don't have an exact use case in mind, it really is more of a challenge for myself. It has maybe one cool feature so far, and that really is kind of a weird intregated macro system I guess.
