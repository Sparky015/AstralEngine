# C++ Naming Conventions



### Variables
For local varibles, use camelCase.
For parameters, use camelCase. 
For global variables in the global namespace (shouldn't be doing this though), use g_ and PascalCase.
Public member variables of classes and structs use CamelCase.
Private member variables of classes and structs prefix with m_ and use CamelCase.
###

### Constants
Convention: upper case with underscores
Example: const int MAX_SCORE;
###

### Functions
Convention: Pamel Case
Example: void CalculateScore();
###

### Classes / Structures / Enums
Convention: Pascal case (camel case starting with an uppercase letter)
Example: class PlayerScore;
         struct PlayerData;
         enum class GameStatus;
###

### Private Member Variables
Convention: prefix with m_ and use pascal case
Example: int m_PlayerScore;
###

### Public Member Variables
Convention: Pascal case
Example: int PlayerScore;
###

### Global Variables
Convention: prefix with g_ and use pascal case
Example: int g_GlobalScore;
###

### Namespaces
Convention: Pascal Case
Example: namespace InputDriver;
###

### Macro Definitions
Convention: upper case with underscores
Example: #define MAX_HEALTH 100
###

### Type Names
Convention: Pascal case
Example: using PlayerList = std::vector<Player>;
###

### Const Correctness

Const is documentation as much as it is a compiler directive, so all code should strive to be const-correct.

This includes:

    Passing function arguments by const pointer or reference if those arguments are not intended to be modified by the function,

    Flagging methods as const if they do not modify the object,

    and using const iteration over containers if the loop isn't intended to modify the container.
###

### Comments
The following example demonstrates the format of class, method, and variable comments. Remember that comments should 
augment the code. The code documents the implementation, and the comments document the intent. Make sure to update 
comments when you change the intent of a piece of code.

Note that two different parameter comment styles are supported, shown by the Steep and Sweeten methods. The @param 
style used by Steep is the traditional multi-line style, but for simple functions it can be clearer to integrate the 
parameter and return value documentation into the descriptive comment for the function, as in the Sweeten example. 
Special comment tags like @see or @return should only be used to start new lines following the primary description.

Method comments should only be included once, where the method is publicly declared. The method comments should only 
contain information relevant to callers of the method, including any information about overrides of the method that may
be relevant to the caller. Details about the implementation of the method and its overrides, that are not relevant to
callers, should be commented within the method implementation.
###

## Other Rules and Stuff

Template classes are prefixed by T.
Classes that are abstract interfaces are prefixed by I.
Enums are prefixed by E.
Singletons are prefixed by S.

Most other classes are prefixed by F, though some subsystems use other letters.

Typedefs should be prefixed by whatever is appropriate for that type: F if it's a typedef of a struct, U if it's a typedef of a UObject and so on.



Inspiration (and some word for word) from https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/

## More Details

Variable, method, and class names should be clear, unambiguous, and descriptive. The greater the scope of the name, the 
greater the importance of a good, descriptive name. Avoid over-abbreviation.

All variables should be declared one at a time, so that a comment on the meaning of the variable can be provided. Also, 
the JavaDocs style requires it. You can use multi-line or single line comments before a variable, and the blank line is 
optional for grouping variables.

All functions that return a bool should ask a true/false question, such as IsVisible() or ShouldClearBuffer() .

A procedure (a function with no return value) should use a strong verb followed by an Object. An exception is, if the 
Object of the method is the Object it is in. Then the Object is understood from context. Names to avoid include those 
beginning with "Handle" and "Process" because the verbs are ambiguous.

Though not required, we encourage you to prefix function parameter names with "Out" if they are passed by reference, 
and the function is expected to write to that value. This makes it obvious that the value passed in this argument will 
be replaced by the function.

If an In or Out parameter is also a boolean, put the "b" before the In/Out prefix, such as bOutResult.

## For more good practices to use, check this link out!
### https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/