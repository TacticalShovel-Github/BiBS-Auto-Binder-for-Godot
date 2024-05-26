# godot-BiBS


This is a header only library that provides a better interface for C++ users to bind against godot to achieve feature parity with GdScript. We accomplish this through a mixture of hopes, prayers, templates, dark sacrifices, and coffee.

BiBS officially stands for *Binding is Baby Stuff*. However, don't allow the offical definition constrain your imagination in completing the acronym in other ways.

While it is absolutely my intention to show off my l33tcode, I recognize that imperfection is intrinsic to the nature of open source software (and software in general), but because of the collaborative and enduring nature of OS software, it can eventually surpass commercial software in quality. This project is my contribution to the godot community which I believe moves in that direction.

### Installation

BiBS is header only. You simply need to set your include path to the top level of this repository.  
    BsExample.cpp is provided as learning material and may not compile on your version of godot.  

For SCons, this would mean adding `CPP_VPATH_LST += Glob("/path/to/godot-BiBS")` to your SConstruct.  
For CMake, I don't know. You are better than me and now that's your problem.

### Requirements
1. BiBS uses the interface provided by GdExtension. It should also be compatible with the native interface. Either way, one of these need to be acquired separately. Please thank the maintainers at godot for all their hard work.  
    GdExtension Users: https://github.com/godotengine/godot-cpp  
    Native Users:  https://github.com/godotengine/godot

2. BiBS needs C++20 or newer.

3. Godot 4.2+ is recommended.

4. Conditionally, you may need a non-MSVC compiler. If MSVC is somehow able to compile these templates, I offer no assurances that it will continue to do so in future releases of BiBS.