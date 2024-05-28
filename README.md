# BiBS-Auto-Binder-for-Godot  

This is a header only library that provides a better interface for C++ users to bind against godot to achieve feature parity with GdScript. We accomplish this through a mixture of hopes, prayers, templates, dark sacrifices, and coffee.

## Supported Use Cases

&emsp; 1. Export any data member in one line of code.  
&emsp; &emsp; a. No more writing getters or setters.  
&emsp; &emsp; b. No fussing around with variant enumerations  
&emsp; &emsp; c. Automatically chooses the best property hints for your member type.  
&emsp; 2. Export methods  
&emsp;&emsp; a. No backreferencing of strings names - ever.  
&emsp; 3. Integrate seamlessly to godot's signal system  
&emsp; &emsp; a. Compile-time for correctness on all callable types.  
&emsp; &emsp; b. Compile-time check for correctness on method callback name.  
&emsp; &emsp; c. Connect signals in one line without any messy strings.  

BiBS officially stands for *Binding is Baby Stuff*. However, don't allow the offical definition constrain your imagination in completing the acronym in other ways.

While it is absolutely my intention to show off my l33tcode, I recognize that imperfection is intrinsic to the nature of open source software (and software in general), but because of the collaborative and enduring nature of OS software, it can eventually surpass commercial software in quality. This project is my contribution to the godot community which I believe moves in that direction.

## Installation

BiBS is header only. You simply need to set your include path to the top level of this repository.  
    BsExample.cpp is provided as learning material and may not compile on your version of godot.  

For SCons, this would mean adding `CPP_VPATH_LST += Glob("/path/to/godot-BiBS")` to your SConstruct.  
For CMake, I don't know. You are better than me and now that's your problem.

## Requirements
1. BiBS uses the interface provided by GdExtension. It should also be compatible with the native interface. Either way, one of these need to be acquired separately. Please thank the maintainers at godot for all their hard work.  
    GdExtension Users: https://github.com/godotengine/godot-cpp  
    Native Users:  https://github.com/godotengine/godot

2. BiBS needs C++20 or newer.

3. Godot 4.2+ is recommended.

### Addendum on compiler support
 &emsp; BiBS has been tested with with GCC and MSVC.  
 &emsp; Ideally, I want to support all C++20 compilers.
 &emsp; Please let me know if you think your compiler isn't compatible.  

## Troubleshooting

#### Help! The compiler is screaming template code at me and I don't know what to do!

Step 1. Calm down and take a DEEP breath.  
 &emsp; Please trust me, things aren't as dire as they look.  
 &emsp; You probably made a tiny syntax error.  
 &emsp; C++ Compilers are hilariously bad at logging template errors.  
 &emsp; &emsp; The compiler is telling you every interpretation of your code it tried, and what was wrong with each one.  
 &emsp; &emsp; About 95% of this logging should be ignored  
Step 2. Scroll up the top of the debug log.  
 &emsp; The compiler will lie to you and say the problem is in BiBS.  
 &emsp; While that may be morally true, on a technical basis, the problem is most likely in your code.  
Step 3. Look for line numbers in the debug log that actually refer to your class.  
 &emsp; This will be a bit like an easter hunt.  
 &emsp; The line numbers referring to #include statements will most likely lead you astray.  
Step 4. Once you have a line number, compare your code with one of the examples.  
 &emsp; Did you do anything differently?  
 &emsp; Did you use a slightly different syntax, or declare things in a different way?  
 &emsp; &emsp; BiBS prioritizes composability with C++, so there are many alternate ways to do things.  
 &emsp; &emsp; If you are new to C++, stick to the syntax in the example until you learn the language better.  

#### Note regarding MSVC debugging  
 &emsp; MSVC is very bad at giving useful error logging when templates and macros are involved.  
 &emsp; Unfortunately, both are heavily used in BiBS.  
 &emsp;  
 &emsp; If you have frequent issues with template/macro compile errors, consider switching to GCC(MinGW for Windows users) or Clang.  
 

You may see certain statements that are, in fact, BiBS trying to tell you that you have made an error.  
 &emsp;  1. `error: static assertion failed: Could not locate index of tag member.`  
 &emsp; &emsp; Most likely on a signal, BiBS tried to lookup an exported member by pointer and could not find it.  
 &emsp; &emsp; This implies that you either forgot to export it, or exported the wrong member.  
 &emsp;  2.  `error: static assertion failed: "Parameter types for callback do not match the signal.`  
 &emsp; &emsp;  This message implies that the types on your signal handler do not match the signal it is bound to.  

These are some other compiler errors to look out for, and what they mean  
 &emsp; 1. error: incomplete type 'MyFirstBiBSClass::EXPORT<18446744073709551615, MyFirstBiBSClass>' used in nested name specifier  
 &emsp;  &emsp; Unlike attributes in C#, these attributes must appear AFTER the declaration that they are referencing.  
 &emsp;  &emsp; If you are seeing this message, it most likely means that that you did something like this..  
 &emsp;  &emsp; &emsp;  
 &emsp;  &emsp; &emsp; // ERROR - You need to move 'myVariable' above 'BS_EXPORT'.  
 &emsp;  &emsp; &emsp; BS_EXPORT(&MyFirstBiBSClass::myVariable,.name="myVariable");   
 &emsp;  &emsp; &emsp; int myVariable = 0;   
 &emsp;  &emsp;  

### Understanding the Syntax Guide  

#### Placeholders
 Names appearing between verical bars are placeholders.  
 &emsp; For instance, `void |FUNCTION_NAME|()` may resolve to either `int foo()` or `int bar()`, but not `void bar()` or `int foo`.

#### Syntax Declaration
 `Syntax |DESIGNATOR|` declares `|DESIGNATOR|` as a symbol that may resolve to one or more expressions.  
  &emsp; Every expression following a syntax declaration is a possible completion of the designator.

#### Patterns
 The first member of a pattern will appear as `|PLACEHOLDER[_0]|`.  
 The following occurence of `|OPERATOR| ...[PLACEHOLDER] |OPERATOR|` denotes an expansion of that pattern.  
 &emsp; The `|OPERATOR|` is usally a comma, but can be any other binary operator.  
 &emsp; This includes, but is not limited to, arithmetic, logical, or bitwise operators.  
 &emsp; When the bitwise-or operator is encountered, it will be substituted with `bitor`, for readability.  
 The occurence of `|PLACEHOLDER [_N]|` terminates the pattern.  
 &emsp; The symbol 'N' is not the only allowed name here.  
 &emsp; &emsp; It may be some other value like 'M'.  
 &emsp; &emsp; This is needed if the expression is a nested pattern or N is in use within the expression.  

 For example, `int arr = {1,3,52,24};` may also be represented as `int arr = {|NUM[_0]|,...[NUM],|NUM[_N]|}`

#### Multi-line statements
 All expressions containing at least one placeholder are assumed to be on a single line.
 If there is a line break continuing a statement, it will be denoted by an escape character (`\`).

### Syntax Guide

```
 Syntax |EXPORT_METHOD_DECLARATION|
    #For a fully anonymous method
    BS_EXPORT(|METHOD_POINTER|); 

    #For a method with anonymous parameters
    BS_EXPORT(|METHOD_POINTER|, .name="|METHOD_NAME|");

    #For a method with fully described parameters
    BS_EXPORT(|METHOD_POINTER|, .name="|METHOD_NAME|", .args={ "|ARG_NAME[_0]|", ...[ARG_NAME], "ARG_NAME[_N]" });

```
```
 Syntax |EXPORT_MEMBER_DATA_DECLARATION|
     #Normal case
     BS_EXPORT(|MEMBER_POINTER|, .name="|MEMBER_NAME|");

     #For a member with manually defined usage flags
     BS_EXPORT(|MEMBER_POINTER|, .name="|MEMBER_NAME|", .additionUsageFlags = |USAGE_FLAG[_0]| bitor ... bitor |USAGE_FLAG[_N]| );

     
```
```
 Syntax |USING_SIGNAL_DECLARATION|                             
    #For a signal with no parameters
    using |SIGNAL_ALIAS| = Bs::Signal< \
        Bs::label( "|SIGNAL_NAME|" ),  \
    >;

    #For a signal with parameters
    using |SIGNAL_ALIAS| = Bs::Signal<                     \
        Bs::label( "|SIGNAL_NAME|" ),                      \
        Bs::param< |PARAM_TYPE[_0]| >( "|PARAM_NAME_0|" ), \
        ...[PARAM_TYPE],                                   \
        Bs::param< |PARAM_TYPE[_N]| >( "|PARAM_NAME_N|" )  \
    >;                                                           
```
```
 Syntax |EXTERNAL_SIGNAL_DECLARATION|                        
                                                                   
     DECLARE_EXTERNAL_SIGNALS(|SIGNAL_EMITTER_TYPE|) \
     {                                               \
         |USING_SIGNAL_DECLARATION[_0]|,             \
         ...[USING_SIGNAL_DECLARATION],              \
         |USING_SIGNAL_DECLARATION[_N]|,             \
     }                                                          
```
```
 Syntax |RX_SIGNAL_DECLARATION|                                
                                                                     
     BS_SIGNAL_RX( |SIGNAL_ALIAS|, .name = |SIGNAL_NAME| );   
```
```
 Syntax |TX_SIGNAL_DECLARATION|                                
                                                                     
     BS_SIGNAL_RX( |SIGNAL_ALIAS|, .name = |SIGNAL_STRING_NAME|, .callback = "|SIGNAL_NAME|" );   

```