# initializer_array
A header only library that eliminates need to use ***std::initializer_list*** for people who dislike it, just like me. 

# *struct initializer_array\<type\>*
This class is the actually suggested replacement for ***std::initializer_list*** it grabs a **prvalue** instance of ***type*** elements on the fly and **size-erases** it.
then its member ***begin()*** and ***end()*** iterators can be used as a range of objects.

\*This class is suppossed to be used as an argument to pass an array of arbitrary size to functions and constructors.

\*It is supposed to be light-weight and **does not allocate memory**!

\***It is not a container**, thus does not copy/move/manipulates the objects within the prvalue temporary array. Once the temporary array dies,
the size of **initializer_array** jumps to zero.

# construction syntax:
**initializer_array\<type\> temp{{value_1,value_2,...value_N}};**
the values need to be enclosed in double curly braces.

# *struct replicate_initializer\<std_container\>*
this inherits from its parameter type, then deletes the constructors who need ***std::initializer_list*** and replaces them with constructors demmanding 
***initializer_array***. when constructing a replicated container with ***initializer_array***, the values need to be enclosed in triple curly braces;
two for ***initializer_array*** and one for the **container**.

# but how?!
let's just say we plant a bomb beside the temporary array, that explodes when the statement containing the construction of ***initializer_array*** instance
comes to its end (**;**). I have [ab?]used the C++ rule that extends the lifetime of referenced temporaries to the end of current statement.

#***initializer_array.cpp***
this is a test stub to demontrate use cases.

