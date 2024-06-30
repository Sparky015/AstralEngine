# C++ Naming Conventions

## Variables
Convention: lowercase with underscores 
Example: int player_score;

## Constants
Convention: upper case with underscores
Example: const int MAX_SCORE;

## Functions
Convention: camel case starting with a lowercase letter
Example: void calculateScore();

## Classes / Structures / Enums
Convention: Pascal case (camel case starting with an uppercase letter)
Example: class PlayerScore;
         struct PlayerData;
         enum class GameStatus;

## Private Member Variables
Convention: prefix with m_ and use camel case
Example: int m_playerScore;

## Public Member Variables
Convention: Pascal case
Example: int PlayerScore;

## Global Variables
Convention: prefix with g_ and use camel case
Example: int g_globalScore;

## Namespaces
Convention: lowercase
Example: namespace gameutils;

## Macro Definitions
Convention: upper case with underscores
Example: #define MAX_HEALTH 100

## Type Names
Convention: Pascal case
Example: using PlayerList = std::vector<Player>;

Replace "Convention" with your chosen convention and modify examples accordingly. This template should be used as a guide and can be modified to best suit your project's needs.