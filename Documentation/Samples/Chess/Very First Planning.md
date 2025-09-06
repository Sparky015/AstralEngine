# Planning & Design for Chess Engine

---

## Data Transformations

- Board split into array of 8 arrays (because statistically, the access of the data will be more in certain rows like
the first two rows of each color piece

Click & Drags -> Coordinate -> Square Location & Piece Moved -> Check if Valid -> (if no, reset) (if yes, move piece & compute valid moves)

![Docs/Images/Data Transforms Flow.png](Documentation/Images/Data Transforms Flow.png)




### Input - Clicks and Drags - Get mouse coordinates

Input: Computer Peripherals
Output: Mouse Coordinates (x, y)

ImGui for UI and displaying images

GLFW for the window

### Coordinates - Find square number

Input: Mouse Coordinates (x, y)
Output: Square Number

Determine the square based on the mouse coordinates given by ImGui/GLFW


### Square Locations - Check if Valid

Input: Square Number
Output: True or False if move valid

### Square Locations - Move Piece

Input: Piece ID, Square Destination, board
Output: void

### Take Piece

Input: Square Number
Output: void


### Compute Valid Moves for Board

Input: board ptr
Output: void






Data Representation:

Boards: 64 byte array -> uint8 board[64]
OR
blackPieces -> uint8 bp[16]
whitePieces -> uint8 wp[16]
OR 
activePieces -> uint8 ap[32]
/
activeBlackPieces -> uint8 abp[16]
activeWhitePieces -> uint8 awp[16]

struct Pieces {
    uint8 location[32];
    uint
}
