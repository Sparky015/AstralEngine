# C++ Naming Conventions



## Variables
(Pascal Case)
The first letter of each word in a name (such as type name or variable name) is capitalized, and there is usually no 
underscore between words. For example, Health and UPrimitiveComponent are correct, but not lastMouseCoordinates or 
delta_coordinates.

## Constants
Convention: upper case with underscores
Example: const int MAX_SCORE;

## Functions
Convention: Pamel Case
Example: void CalculateScore();

## Classes / Structures / Enums
Convention: Pascal case (camel case starting with an uppercase letter)
Example: class PlayerScore;
         struct PlayerData;
         enum class GameStatus;

## Private Member Variables
Convention: prefix with m_ and use pascal case
Example: int m_PlayerScore;

## Public Member Variables
Convention: Pascal case
Example: int PlayerScore;

## Global Variables
Convention: prefix with g_ and use pascal case
Example: int g_GlobalScore;

## Namespaces
Convention: Pascal Case
Example: namespace InputDriver;

## Macro Definitions
Convention: upper case with underscores
Example: #define MAX_HEALTH 100

## Type Names
Convention: Pascal case
Example: using PlayerList = std::vector<Player>;



Template classes are prefixed by T.

Classes that are abstract interfaces are prefixed by I.

Enums are prefixed by E.

Boolean variables must be prefixed by b (for example, bPendingDestruction , or bHasFadedIn ).

Most other classes are prefixed by F, though some subsystems use other letters.

Typedefs should be prefixed by whatever is appropriate for that type: F if it's a typedef of a struct, U if it's a typedef of a UObject and so on.



Inspiration (and some word for word) from https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/

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

For more good practices to use, check this link out!
https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/