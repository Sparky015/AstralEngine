/**
* @file Board.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "Board.h"


namespace Game {

/*********************************************
                   Board
**********************************************/

    Board::Board() :
            m_BlackPieceLocations({A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8}),
            m_WhitePieceLocations({A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1})

    {
        PROFILE_SCOPE();
        std::array<PieceType, 16> OrderOfPieceTypes
        {PieceType::PAWN,PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
         PieceType::PAWN,PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
         PieceType::ROOK,PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
         PieceType::KING,PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK};
        // Take the location of each piece and populate that square with the piece's info

        for (uint8 pieceID = 0; pieceID < m_BlackPieceLocations.size(); pieceID++)
        {
            uint8 pieceLocation = m_BlackPieceLocations[pieceID];
            m_Board.WriteSquareColor(BLACK, pieceLocation);
            m_Board.WriteSquareType(OrderOfPieceTypes[pieceID], pieceLocation);
        }

        for (uint8 pieceID = 0; pieceID < m_WhitePieceLocations.size(); pieceID++)
        {
            uint8 pieceLocation = m_WhitePieceLocations[pieceID];
            m_Board.WriteSquareColor(WHITE, pieceLocation);
            m_Board.WriteSquareType(OrderOfPieceTypes[pieceID], pieceLocation);
        }

    }

// Helper function for FEN constructor
    PieceType ConvertCharToPieceType(char c)
    {
        c = tolower(c);
        switch (c)
        {
            case 'p':
                return PieceType::PAWN;
            case 'r':
                return PieceType::ROOK;
            case 'b':
                return PieceType::BISHOP;
            case 'n':
                return PieceType::KNIGHT;
            case 'k':
                return PieceType::KING;
            case 'q':
                return PieceType::QUEEN;
            default:
                throw std::logic_error("Char did not map to any character.");
        }
    }

    Board::Board(const std::string& FEN)
    {
        PROFILE_SCOPE();
        // Initializing an empty board and empty piece locations
        m_Board = InternalBoardRepresentation();
        m_WhitePieceLocations.fill(EMPTY);
        m_BlackPieceLocations.fill(EMPTY);

        uint8 whitePieceIDMarker = 0; // Holds the next white piece ID that needs to be assigned
        uint8 blackPieceIDMarker = 0; // Holds the next black piece ID that needs to be assigned

        // Going through each character in the FEN sequence and adding the piece to the board (top to bottom)
        uint8 squareLocation = 0;
        for (char c: FEN)
        {
            // Check if the character is a number (a number means to skip/set empty the that number of squares)
            if (isdigit(c))
            {
                squareLocation += (c - '0');
            }
            else
            { // Then the character is a letter

                // Check to see if there is a break symbol (which means to go to the next row)
                if (c == '/')
                {
                    if (squareLocation % 8 == 0)
                    { continue; }

                    // Set squareLocation to the next row
                    squareLocation += 8 - (squareLocation % 8);
                    continue;
                }

                PieceColor pieceColor = (PieceColor) isupper(c);
                // Set the color. Upper case is white and lower case is black
                m_Board.WriteSquareColor(pieceColor, squareLocation);

                // Set the piece type.
                m_Board.WriteSquareType(ConvertCharToPieceType(c), squareLocation);

                if (pieceColor == PieceColor::WHITE)
                {
                    m_WhitePieceLocations[whitePieceIDMarker] = squareLocation;
                    whitePieceIDMarker++;
                }
                else
                {
                    m_BlackPieceLocations[blackPieceIDMarker] = squareLocation;
                    blackPieceIDMarker++;
                }

                // Increment the square location for the next iteration
                squareLocation++;
            }
        }
    }


    void Board::MovePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
    {
        PROFILE_SCOPE();
        if (targetBoardLocation > 63)
        { throw std::out_of_range("targetBoardLocation is not between 0 and 63"); }
        if (m_Board.ReadSquareType(targetBoardLocation) != PieceType::NONE)
        { throw std::logic_error("targetBoardLocation already has a piece on the square"); }
        if (ReadPieceType(color, pieceID) == PieceType::NONE)
        { throw std::logic_error("Can't move a piece of type NONE (aka that piece isn't on the board)"); }

        // Getting type of the piece from the old location
        PieceType pieceType = ReadPieceType(color, pieceID);

        // Clearing piece type from old location
        m_Board.WriteSquareType(PieceType::NONE, ReadPieceLocation(color, pieceID));

        // Updating new location with piece information
        m_Board.WriteSquareType(pieceType, targetBoardLocation);
        m_Board.WriteSquareColor(color, targetBoardLocation);

        if (color == PieceColor::WHITE)
        {
            m_WhitePieceLocations[pieceID] = targetBoardLocation;
        }
        else
        {
            m_BlackPieceLocations[pieceID] = targetBoardLocation;
        }
    }


    void Board::TakePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
    {
        PROFILE_SCOPE();
        if (targetBoardLocation > 63)
        { throw std::out_of_range("targetBoardLocation is not a valid location on the chess board"); }
        if (ReadSquareType(targetBoardLocation) == PieceType::NONE)
        { throw std::logic_error("Can't take on a square that is empty"); }
        if (ReadSquareColor(targetBoardLocation) == color)
        { throw std::logic_error("Can't take a piece of the same type"); }
        if (ReadPieceType(color, pieceID) == PieceType::NONE)
        { throw std::logic_error("Can't take with a piece of type NONE (aka that piece isn't on the board)"); }


        uint8 oldPieceLocation = (color == PieceColor::WHITE) ? m_WhitePieceLocations[pieceID] : m_BlackPieceLocations[pieceID];

        // Get the ID of the piece being taken that is on targetBoardLocation and clear the location of the piece from piece locations
        PieceID targetPieceID = ReadSquarePieceID(targetBoardLocation);
        (color == PieceColor::WHITE) ? m_BlackPieceLocations[targetPieceID] = EMPTY : m_WhitePieceLocations[targetPieceID] = EMPTY;


        // Move the taking piece to the new location
        m_Board.WriteSquareType(ReadPieceType(color, pieceID), targetBoardLocation);
        m_Board.WriteSquareColor(color, targetBoardLocation);
        if (color == PieceColor::WHITE) {m_WhitePieceLocations[pieceID] = targetBoardLocation;}
        else {m_BlackPieceLocations[pieceID] = targetBoardLocation;}


        // Clear the old position of the piece that is taking
        m_Board.WriteSquareType(PieceType::NONE, oldPieceLocation);
    }


    void Board::PromotePawn(const PieceColor color, const PieceID pieceID, const PieceType promotionType)
    {
        PROFILE_SCOPE();
        if (promotionType == PieceType::KING || promotionType == PieceType::PAWN)
        { throw std::logic_error("Promotion type must be bishop, knight, rook, or queen."); }
        if (ReadPieceType(color, pieceID) != PieceType::PAWN)
        { throw std::logic_error("Piece must be a pawn to promote it."); }

        if (color == PieceColor::WHITE)
        {
            uint8 pieceLocation = m_WhitePieceLocations[pieceID];
            m_Board.WriteSquareType(promotionType, pieceLocation);
        }
        else
        {
            uint8 pieceLocation = m_BlackPieceLocations[pieceID];
            m_Board.WriteSquareType(promotionType, pieceLocation);
        }
    }


    uint8 Board::ReadPieceLocation(const PieceColor color, const PieceID pieceID) const
    {
        PROFILE_SCOPE();
        if (color == PieceColor::WHITE)
        {
            return m_WhitePieceLocations[pieceID];
        }
        else
        {
            return m_BlackPieceLocations[pieceID];
        }
    }


    PieceType Board::ReadPieceType(const PieceColor color, const PieceID pieceID) const
    {
        uint8 pieceLocation = ReadPieceLocation(color, pieceID);
        if (pieceLocation == EMPTY) {return PieceType::NONE;}
        return ReadSquareType(pieceLocation);
    }


    void Board::WritePieceLocation(const PieceColor color, const PieceID pieceID, const uint8_t boardLocation)
    {
        PROFILE_SCOPE();
        if (color == PieceColor::WHITE)
        {
            m_WhitePieceLocations[pieceID] = boardLocation;
        }
        else
        {
            m_BlackPieceLocations[pieceID] = boardLocation;
        }
    }


    PieceColor Board::ReadSquareColor(uint8 squareLocation) const
    {
        PROFILE_SCOPE();
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        return m_Board.ReadSquareColor(squareLocation);
    }


    void Board::WriteSquareColor(uint8 squareLocation, PieceColor pieceColor)
    {
        PROFILE_SCOPE();
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }

        m_Board.WriteSquareColor(pieceColor, squareLocation);
    }


    PieceType Board::ReadSquareType(uint8 squareLocation) const
    {
        PROFILE_SCOPE();
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        return m_Board.ReadSquareType(squareLocation);
    }


    void Board::WriteSquareType(uint8_t squareLocation, PieceType pieceType)
    {
        PROFILE_SCOPE();
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        m_Board.WriteSquareType(pieceType, squareLocation);
    }


    PieceID Board::ReadSquarePieceID(uint8_t squareLocation) const
    {
        PROFILE_SCOPE();
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }

        PieceType pieceType = ReadSquareType(squareLocation);
        PieceColor pieceColor = ReadSquareColor(squareLocation);

        if (pieceType == PieceType::NONE)
        { throw std::logic_error("There is no piece on square"); }

        if (pieceColor == PieceColor::WHITE)
        {
            for (uint8 pieceID = 0; pieceID < 16; pieceID++)
            {
                if (m_WhitePieceLocations[pieceID] == squareLocation)
                {
                    return (PieceID) pieceID;
                }
            }
        }
        else
        {
            for (uint8 pieceID = 0; pieceID < 16; pieceID++)
            {
                if (m_BlackPieceLocations[pieceID] == squareLocation)
                {
                    return (PieceID) pieceID;
                }
            }
        }

        throw std::logic_error("No piece ID was found with that location");
    }


    void Board::ClearBoard()
    {
        PROFILE_SCOPE();
        m_Board = InternalBoardRepresentation();
    }





// Helper function for printing board to console and converting piece to character
    char ConvertPieceTypeToChar(PieceType pieceType)
    {
        switch (pieceType)
        {
            case PieceType::PAWN:
                return 'p';
            case PieceType::ROOK:
                return 'r';
            case PieceType::BISHOP:
                return 'b';
            case PieceType::KNIGHT:
                return 'n';
            case PieceType::KING:
                return 'k';
            case PieceType::QUEEN:
                return 'q';
            case PieceType::NONE:
                return '_';
            default:
                throw std::logic_error("PieceType did not map to any character.");
        }
    }


    char Board::GetCharacterOfPiece(uint8 squareLocation)
    {
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }

        if (m_Board.ReadSquareColor(squareLocation) == PieceColor::WHITE)
        {
            return (char)toupper(ConvertPieceTypeToChar(m_Board.ReadSquareType(squareLocation)));
        }
        else
        {
            return ConvertPieceTypeToChar(m_Board.ReadSquareType(squareLocation));
        }

    }


    void Board::PrintBoardToConsole()
    {
        PROFILE_SCOPE();
        std::cout << "\n";
        // Loop through each square
        for (uint8 squareLocation = 0; squareLocation < 64; squareLocation++)
        {
            if (squareLocation % 8 == 0)
            { std::cout << "\n"; }

            std::cout << GetCharacterOfPiece(squareLocation) << " ";
        }
        std::cout << "\n";
    }




}