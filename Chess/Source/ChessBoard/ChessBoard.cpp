/**
* @file Board.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "ChessBoard.h"

#include "Utils/ChessConversions.h"

namespace Game {

/*********************************************
                   Board
**********************************************/

    ChessBoard::ChessBoard() : m_BlackPieceLocations({A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8}),
                               m_WhitePieceLocations({A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1}),
                               m_FullMoveCount(1),
                               m_HalfMoveCount(1),
                               m_ActiveColor(PieceColor::WHITE)
    {
        PROFILE_SCOPE();
        std::array<PieceType, 16> OrderOfPieceTypes
        {
            PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
            PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
            PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
            PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
        };
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

        m_CastleRights.SetCastleRight(PieceColor::WHITE, KING_SIDE);
        m_CastleRights.SetCastleRight(PieceColor::WHITE, QUEEN_SIDE);
        m_CastleRights.SetCastleRight(PieceColor::BLACK, KING_SIDE);
        m_CastleRights.SetCastleRight(PieceColor::BLACK, QUEEN_SIDE);

        memset(&m_LastMoveBuffer, 0, sizeof(m_LastMoveBuffer));
    }


    ChessBoard::ChessBoard(const std::string& FEN)
    {
        PROFILE_SCOPE();
        // Initializing an empty board and empty piece locations
        m_Board = InternalBoardRepresentation();
        m_WhitePieceLocations.fill(EMPTY);
        m_BlackPieceLocations.fill(EMPTY);

        std::string pieceLocations;
        char activeColor;
        std::string castleRights;
        std::string enPassantPawn;
        std::string halfMoveCount;
        std::string fullMoveCount;

        std::istringstream iss(FEN);
        iss >> pieceLocations >> activeColor >> castleRights >> enPassantPawn >> halfMoveCount >> fullMoveCount;

        m_HalfMoveCount = (!halfMoveCount.empty()) ? std::stoi(halfMoveCount) : 0;
        m_FullMoveCount = (!fullMoveCount.empty()) ? std::stoi(fullMoveCount) : 0;

        // Setting the active color
        m_ActiveColor = (activeColor == 'w') ? PieceColor::WHITE : PieceColor::BLACK;

        // Setting the castle rights of the board
        if (castleRights[0] != 'c')
        {
            for (char castleRight: castleRights)
            {
                if (islower(castleRight))
                {
                    m_CastleRights.SetCastleRight(PieceColor::BLACK, (castleRight == 'q') ? QUEEN_SIDE : KING_SIDE);
                }
                else
                {
                    m_CastleRights.SetCastleRight(PieceColor::WHITE, (castleRight == 'Q') ? QUEEN_SIDE : KING_SIDE);
                }
            }
        }

        uint8 whitePieceIDMarker = 0; // Holds the next white piece ID that needs to be assigned
        uint8 blackPieceIDMarker = 0; // Holds the next black piece ID that needs to be assigned

        // Going through each character in the FEN sequence and adding the piece to the board (top to bottom)
        uint8 squareLocation = 0;
        for (char FEN_character : pieceLocations)
        {
            // Check if the character is a number (a number means to skip/set empty the that number of squares)
            if (isdigit(FEN_character))
            {
                squareLocation += (FEN_character - '0');
            }
            else
            { // Then the character is a letter

                // Check to see if there is a break symbol (which means to go to the next row)
                if (FEN_character == '/')
                {
                    if (squareLocation % 8 == 0)
                    { continue; }

                    // Set squareLocation to the next row
                    squareLocation += 8 - (squareLocation % 8);
                    continue;
                }


                // Set the color. Upper case is white and lower case is black
                PieceColor pieceColor = (PieceColor)isupper(FEN_character);
                m_Board.WriteSquareColor(pieceColor, squareLocation);

                // Set the piece type.
                m_Board.WriteSquareType(ConvertCharToPieceType(FEN_character), squareLocation);

                // Set the location of the piece
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

        // Setting the en passant state
        if (enPassantPawn != "-" && !enPassantPawn.empty())
        {
            m_LastMoveBuffer.FinalPieceLocation = ConvertChessNotationToInt(enPassantPawn);
            uint8 initialPositionStep = (m_LastMoveBuffer.FinalPieceLocation < A4) ? 8 : -8;
            m_LastMoveBuffer.FinalPieceLocation += initialPositionStep;

            uint8 moveStep = (m_LastMoveBuffer.FinalPieceLocation < A4) ? -16 : 16;
            m_LastMoveBuffer.InitialPieceLocation = m_LastMoveBuffer.FinalPieceLocation + moveStep;

            m_LastMoveBuffer.MovingPieceType = PieceType::PAWN;
            m_LastMoveBuffer.MovingPieceID = ReadSquarePieceID(m_LastMoveBuffer.FinalPieceLocation);

            m_LastMoveBuffer.PieceTakenType = PieceType::NONE;
            m_LastMoveBuffer.TakenPieceID = PIECE_1; // == 0
        }
        else
        {
            memset(&m_LastMoveBuffer, 0, sizeof(m_LastMoveBuffer));
        }
    }


    void ChessBoard::MovePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
    {
        PROFILE_SCOPE();
        ASSERT(targetBoardLocation < 64, "squareLocation is not a valid location on the chess board");
        ASSERT(m_Board.ReadSquareType(targetBoardLocation) == PieceType::NONE, "targetBoardLocation already has a piece on the square");
        PieceType movingPieceType = ReadPieceType(color, pieceID);
        ASSERT(movingPieceType != PieceType::NONE, "Can't move a piece of type NONE (aka that piece isn't on the board)")

        // Set the half move counter. (if a pawn moves or a take happens, then reset)
        if (movingPieceType == PieceType::PAWN) { m_HalfMoveCount = 1;}
        else { m_HalfMoveCount++; }

        // Increment the full move counter if the piece is black
        if (color == PieceColor::BLACK) { m_FullMoveCount++; }

        // Updating the castle rights when a king or rook moves
        if (movingPieceType == PieceType::KING)
        {
            m_CastleRights.UnsetCastleRight(color, KING_SIDE);
            m_CastleRights.UnsetCastleRight(color, QUEEN_SIDE);
        }
        else if (movingPieceType == PieceType::ROOK)
        {
            uint8 pieceLocation = ReadPieceLocation(color, pieceID);
            if (pieceLocation % 8 == 0) {m_CastleRights.UnsetCastleRight(color, QUEEN_SIDE);}
            else if (pieceLocation % 8 == 7) {m_CastleRights.UnsetCastleRight(color, KING_SIDE);}
        }

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


    void ChessBoard::TakePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
    {
        PROFILE_SCOPE();
        ASSERT(targetBoardLocation < 64, "squareLocation is not a valid location on the chess board");
        ASSERT(ReadSquareType(targetBoardLocation) != PieceType::NONE, "Can't take on a square that is empty");
        ASSERT(ReadSquareColor(targetBoardLocation) != color, "Can't take a piece of the same type");
        ASSERT(ReadPieceType(color, pieceID) != PieceType::NONE, "Can't take with a piece of type NONE (aka that piece isn't on the board)");

        m_HalfMoveCount = 1; // Reset the half move counter on takes
        if (color == PieceColor::BLACK) { m_FullMoveCount++; } // Increment the full move counter if the piece is black

        uint8 oldPieceLocation = (color == PieceColor::WHITE) ? m_WhitePieceLocations[pieceID] : m_BlackPieceLocations[pieceID];

        // Updating the castle rights when a rook is taken
        if (targetBoardLocation % 8 == 0 || targetBoardLocation % 8 == 7)
        {
            PieceColor takenPieceColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
            PieceType takenPieceType = ReadPieceType(takenPieceColor, ReadSquarePieceID(targetBoardLocation));

            if (takenPieceType == PieceType::ROOK)
            {
                if (targetBoardLocation % 8 == 0)
                { m_CastleRights.UnsetCastleRight(color, QUEEN_SIDE); }
                else if (targetBoardLocation % 8 == 7)
                { m_CastleRights.UnsetCastleRight(color, KING_SIDE); }
            }
        }

        // Updating the castle rights if a rook or king takes
        PieceType takingPieceType = ReadPieceType(color, pieceID);
        if (takingPieceType == PieceType::KING)
        {
            m_CastleRights.UnsetCastleRight(color, KING_SIDE);
            m_CastleRights.UnsetCastleRight(color, QUEEN_SIDE);
        }
        else if (takingPieceType == PieceType::ROOK)
        {
            uint8 pieceLocation = ReadPieceLocation(color, pieceID);
            if (pieceLocation % 8 == 0) {m_CastleRights.UnsetCastleRight(color, QUEEN_SIDE);}
            else if (pieceLocation % 8 == 7) {m_CastleRights.UnsetCastleRight(color, KING_SIDE);}
        }


        // Get the ID of the piece being taken that is on targetBoardLocation and clear the location of the piece from piece locations
        PieceID targetPieceID = ReadSquarePieceID(targetBoardLocation);
        (color == PieceColor::WHITE) ? m_BlackPieceLocations[targetPieceID] = EMPTY : m_WhitePieceLocations[targetPieceID] = EMPTY;


        // Move the taking piece to the new location
        m_Board.WriteSquareType(takingPieceType, targetBoardLocation);
        m_Board.WriteSquareColor(color, targetBoardLocation);
        if (color == PieceColor::WHITE) {m_WhitePieceLocations[pieceID] = targetBoardLocation;}
        else {m_BlackPieceLocations[pieceID] = targetBoardLocation;}


        // Clear the old position of the piece that is taking
        m_Board.WriteSquareType(PieceType::NONE, oldPieceLocation);
    }


    void ChessBoard::PromotePawn(const PieceColor color, const PieceID pieceID, const PieceType promotionType)
    {
        PROFILE_SCOPE();
        ASSERT(promotionType != PieceType::PAWN && promotionType != PieceType::KING, "Promotion type must be bishop, knight, rook, or queen.")
        ASSERT(ReadPieceType(color, pieceID) == PieceType::PAWN, "Piece must be a pawn to promote it.")

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


    uint8 ChessBoard::ReadPieceLocation(const PieceColor color, const PieceID pieceID) const
    {
        if (color == PieceColor::WHITE)
        {
            return m_WhitePieceLocations[pieceID];
        }
        else
        {
            return m_BlackPieceLocations[pieceID];
        }
    }


    PieceType ChessBoard::ReadPieceType(const PieceColor color, const PieceID pieceID) const
    {
        uint8 pieceLocation = ReadPieceLocation(color, pieceID);
        if (pieceLocation == EMPTY) {return PieceType::NONE;}
        return ReadSquareType(pieceLocation);
    }


//    void Board::WritePieceLocation(const PieceColor color, const PieceID pieceID, const uint8_t boardLocation)
//    {
//        PROFILE_SCOPE();
//        if (color == PieceColor::WHITE)
//        {
//            m_WhitePieceLocations[pieceID] = boardLocation;
//        }
//        else
//        {
//            m_BlackPieceLocations[pieceID] = boardLocation;
//        }
//    }


    PieceColor ChessBoard::ReadSquareColor(uint8 squareLocation) const
    {
        ASSERT(squareLocation < 64, "squareLocation is not a valid location on the chess board");
        return m_Board.ReadSquareColor(squareLocation);
    }


//    void Board::WriteSquareColor(uint8 squareLocation, PieceColor pieceColor)
//    {
//        PROFILE_SCOPE();
//        if (squareLocation > 63)
//        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
//
//        m_Board.WriteSquareColor(pieceColor, squareLocation);
//    }


    PieceType ChessBoard::ReadSquareType(uint8 squareLocation) const
    {
        ASSERT(squareLocation < 64, "squareLocation is not a valid location on the chess board");
        return m_Board.ReadSquareType(squareLocation);
    }


//    void Board::WriteSquareType(uint8_t squareLocation, PieceType pieceType)
//    {
//        PROFILE_SCOPE();
//        if (squareLocation > 63)
//        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
//        m_Board.WriteSquareType(pieceType, squareLocation);
//    }


    PieceID ChessBoard::ReadSquarePieceID(uint8_t squareLocation) const
    {
        PROFILE_SCOPE();
        ASSERT(squareLocation < 64, "squareLocation is not a valid location on the chess board");

        PieceType pieceType = ReadSquareType(squareLocation);
        PieceColor pieceColor = ReadSquareColor(squareLocation);

        ASSERT(pieceType != PieceType::NONE, "There is no piece on square");

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

        ERROR("No piece ID was found with that location");
    }


    void ChessBoard::ClearBoard()
    {
        PROFILE_SCOPE();
        m_Board = InternalBoardRepresentation();
    }


    KingCastleRights ChessBoard::GetCastleRights(PieceColor color) const
    {
        return m_CastleRights.ReadCastleRights(color);
    }


    PieceColor ChessBoard::GetActiveColor() const
    {
        return m_ActiveColor;
    }


    char ChessBoard::GetCharacterOfPiece(uint8 squareLocation)
    {
        ASSERT(squareLocation < 64, "squareLocation is not a valid location on the chess board");

        if (m_Board.ReadSquareColor(squareLocation) == PieceColor::WHITE)
        {
            return (char)toupper(ConvertPieceTypeToChar(m_Board.ReadSquareType(squareLocation)));
        }
        else
        {
            return ConvertPieceTypeToChar(m_Board.ReadSquareType(squareLocation));
        }

    }


    void ChessBoard::PrintBoardToConsole()
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