/**
* @file ChessBoard.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "Chessboard.h"

#include "FENParser.h"
#include "Utils/ChessConversions.h"

namespace Game {

/*********************************************
                   Board
**********************************************/

    Chessboard::Chessboard()
        : m_FullMoveCount(1), m_HalfMoveCount(1), m_ActiveColor(PieceColor::WHITE),
          m_BlackPieceLocations({A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8}),
          m_WhitePieceLocations({A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1})
    {
        PROFILE_SCOPE("Chessboard Constructor");
        constexpr std::array<PieceType, NUMBER_OF_PIECES_PER_COLOR> OrderOfPieceTypes
        {
            PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
            PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
            PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
            PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
        };
        // Take the location of each piece and populate that square with the piece's info

        for (uint8 pieceID = 0; pieceID < NUMBER_OF_PIECES_PER_COLOR; pieceID++)
        {
            const SquareLocation pieceLocation = m_BlackPieceLocations[pieceID];
            m_Board.WriteSquareColor(PieceColor::BLACK, pieceLocation);
            m_Board.WriteSquareType(OrderOfPieceTypes[pieceID], pieceLocation);
        }

        for (uint8 pieceID = 0; pieceID < NUMBER_OF_PIECES_PER_COLOR; pieceID++)
        {
            const SquareLocation pieceLocation = m_WhitePieceLocations[pieceID];
            m_Board.WriteSquareColor(PieceColor::WHITE, pieceLocation);
            m_Board.WriteSquareType(OrderOfPieceTypes[pieceID], pieceLocation);
        }

        m_CastleRights.SetAllFlags();

    }


    Chessboard::Chessboard(const std::string& FEN)
        : m_FullMoveCount(1), m_HalfMoveCount(1), m_ActiveColor(PieceColor::WHITE)
    {
        FENParser fenParser;

        fenParser.ParseFENString(FEN);


    }


    void Chessboard::MovePiece(const PieceID pieceID, const PieceColor pieceColor, const SquareLocation targetLocation)
    {
        PROFILE_SCOPE("Chessboard::MovePiece");
        ASSERT(targetLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board. Raw location: " << targetLocation.GetRawValue());
        ASSERT(m_Board.ReadSquareType(targetLocation) == PieceType::NONE, "targetBoardLocation already has a piece on the square");
        const PieceType movingPieceType = GetPieceType(pieceID, pieceColor);
        ASSERT(movingPieceType != PieceType::NONE, "Can't move a piece of type NONE (aka that piece isn't on the board)")
        ASSERT(pieceColor == m_ActiveColor, "Can't take when it is not the piece's turn to go.")

        // Set the half move counter. (if a pawn moves or a take happens, then reset)
        if (movingPieceType == PieceType::PAWN) { m_HalfMoveCount = 1;}
        else { m_HalfMoveCount++; }

        // Increment the full move counter if the piece is black
        if (pieceColor.IsBlack()) { m_FullMoveCount++; }

        // Updating the castle rights when a king or rook moves
        if (movingPieceType == PieceType::KING)
        {
            m_CastleRights.UnsetCastleRight(pieceColor, KING_SIDE);
            m_CastleRights.UnsetCastleRight(pieceColor, QUEEN_SIDE);
        }
        else if (movingPieceType == PieceType::ROOK)
        {
            const SquareLocation pieceLocation = GetPieceLocation(pieceID, pieceColor);
            if (pieceLocation.GetColumn() == 1) {m_CastleRights.UnsetCastleRight(pieceColor, QUEEN_SIDE);}
            else if (pieceLocation.GetColumn() == 8) {m_CastleRights.UnsetCastleRight(pieceColor, KING_SIDE);}
        }

        // Getting type of the piece from the old location
        const PieceType pieceType = GetPieceType(pieceID, pieceColor);

        RemovePiece(pieceID, pieceColor); // Remove the old piece information from the board
        AddPiece(pieceID, pieceType, pieceColor, targetLocation); // ADd the new piece information to the board

        // Flip active color
        m_ActiveColor = (m_ActiveColor == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    }


    void Chessboard::CapturePiece(const PieceID pieceID, const PieceColor pieceColor, const SquareLocation targetPieceLocation)
    {
        PROFILE_SCOPE("Chessboard::CapturePiece");
        ASSERT(targetPieceLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board. Raw location: " << targetPieceLocation.GetRawValue());
        ASSERT(GetSquareType(targetPieceLocation) != PieceType::NONE, "Can't take on a square that is empty");
        ASSERT(GetSquareColor(targetPieceLocation) != pieceColor, "Can't take a piece of the same type");
        ASSERT(GetPieceType(pieceID, pieceColor) != PieceType::NONE, "Can't take with a piece of type NONE (aka that piece isn't on the board)");
        ASSERT(pieceColor == m_ActiveColor, "Piece's color is not active. Can't take when it is not the piece's turn to go.")

        m_HalfMoveCount = 1; // Reset the half move counter on takes
        if (pieceColor == PieceColor::BLACK) { m_FullMoveCount++; } // Increment the full move counter if the piece is black


        // Updating the castle rights when a rook is taken
        if (targetPieceLocation.GetColumn() == 1 || targetPieceLocation.GetColumn() == 8)
        {
            const PieceColor takenPieceColor = pieceColor.Opposite();
            const PieceType takenPieceType = GetPieceType(GetSquarePieceID(targetPieceLocation), takenPieceColor);

            if (takenPieceType == PieceType::ROOK)
            {
                if (targetPieceLocation.GetColumn() == 1)
                    { m_CastleRights.UnsetCastleRight(pieceColor, QUEEN_SIDE); }
                else if (targetPieceLocation.GetColumn() == 8)
                    { m_CastleRights.UnsetCastleRight(pieceColor, KING_SIDE); }
            }
        }

        // Updating the castle rights if a rook or king takes
        const PieceType takingPieceType = GetPieceType(pieceID, pieceColor);
        if (takingPieceType == PieceType::KING)
        {
            m_CastleRights.UnsetCastleRight(pieceColor, KING_SIDE);
            m_CastleRights.UnsetCastleRight(pieceColor, QUEEN_SIDE);
        }
        else if (takingPieceType == PieceType::ROOK)
        {
            const SquareLocation pieceLocation = GetPieceLocation(pieceID, pieceColor);
            if (pieceLocation.GetColumn() == 1) {m_CastleRights.UnsetCastleRight(pieceColor, QUEEN_SIDE);}
            else if (pieceLocation.GetColumn() == 8) {m_CastleRights.UnsetCastleRight(pieceColor, KING_SIDE);}
        }

        const SquareLocation oldPieceLocation = pieceColor.IsWhite() ? m_WhitePieceLocations[pieceID] : m_BlackPieceLocations[pieceID];

        // Get the ID of the piece being taken that is on targetBoardLocation and clear the location of the piece from piece locations
        const PieceID targetPieceID = GetSquarePieceID(targetPieceLocation);
        std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR>& capturedColorPieceLocation = pieceColor.Opposite().IsWhite() ? m_WhitePieceLocations : m_BlackPieceLocations;
        capturedColorPieceLocation[targetPieceID] = EMPTY;

        // Update capturing piece location
        m_Board.WriteSquareType(takingPieceType, targetPieceLocation);
        m_Board.WriteSquareColor(pieceColor, targetPieceLocation);
        std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR>& capturingColorPieceLocations = pieceColor.IsWhite() ? m_WhitePieceLocations : m_BlackPieceLocations;
        capturingColorPieceLocations[pieceID] = targetPieceLocation;

        // Clear the old position of the piece that is capturing
        m_Board.WriteSquareType(PieceType::NONE, oldPieceLocation);

        // Flip active color
        m_ActiveColor = m_ActiveColor.Opposite();
    }


    void Chessboard::PromotePawn(const PieceID pieceID, const PieceColor pieceColor, const PieceType promotionType)
    {
        PROFILE_SCOPE("Chessboard::PromotePawn");
        ASSERT(promotionType != PieceType::PAWN && promotionType != PieceType::KING, "Promotion type must be bishop, knight, rook, or queen.")
        ASSERT(GetPieceType(pieceID, pieceColor) == PieceType::PAWN, "Piece must be a pawn to promote it.")
        SetPieceType(pieceID, promotionType, pieceColor);
    }


    void Chessboard::AddPiece(PieceID pieceID, PieceType pieceType,  PieceColor pieceColor, SquareLocation location)
    {
        ASSERT(GetPieceLocation(pieceID, pieceColor).IsEmpty(), "This piece is already on the board!");
        ASSERT(location.IsOnBoard(), "The given location is not a valid location on the board. Raw location: " << location.GetRawValue());
        ASSERT(!pieceType.IsNone(), "Added piece can not of a piece type of NONE.")

        std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR>& pieceLocations = pieceColor.IsWhite() ? m_WhitePieceLocations : m_BlackPieceLocations;
        pieceLocations[pieceID] = location;
        m_Board.WriteSquareColor(pieceColor, location);
        m_Board.WriteSquareType(pieceType, location);
    }


    void Chessboard::RemovePiece(PieceID pieceID, PieceColor pieceColor)
    {
        ASSERT(GetPieceLocation(pieceID, pieceColor).IsOnBoard(), "Piece location is not on the board. Raw location: " << GetPieceLocation(pieceID, pieceColor).GetRawValue());

        SquareLocation pieceLocation = GetPieceLocation(pieceID, pieceColor);
        m_Board.WriteSquareType(PieceType::NONE, pieceLocation);

        std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR>& colorPieceLocations = pieceColor.IsWhite() ? m_WhitePieceLocations : m_BlackPieceLocations;
        colorPieceLocations[pieceID] = EMPTY;
    }


    SquareLocation Chessboard::GetPieceLocation(const PieceID pieceID, const PieceColor pieceColor) const
    {
        if (pieceColor.IsWhite())
            { return m_WhitePieceLocations[pieceID]; }
        else
            { return m_BlackPieceLocations[pieceID]; }
    }


    PieceType Chessboard::GetPieceType(const PieceID pieceID, const PieceColor pieceColor) const
    {
        const SquareLocation pieceLocation = GetPieceLocation(pieceID, pieceColor);
        if (pieceLocation.IsEmpty())
            { return PieceType::NONE; }

        return GetSquareType(pieceLocation);
    }


    PieceColor Chessboard::GetSquareColor(const SquareLocation squareLocation) const
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board. Raw location: " << squareLocation.GetRawValue());
        return m_Board.ReadSquareColor(squareLocation);
    }


    PieceType Chessboard::GetSquareType(const SquareLocation squareLocation) const
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board. Raw location: " << squareLocation.GetRawValue());
        return m_Board.ReadSquareType(squareLocation);
    }


    PieceID Chessboard::GetSquarePieceID(const SquareLocation squareLocation) const
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board. Raw location: " << squareLocation.GetRawValue());

        const PieceType pieceType = GetSquareType(squareLocation);
        const PieceColor pieceColor = GetSquareColor(squareLocation);

        ASSERT(pieceType != PieceType::NONE, "There is no piece on square");

        const std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR>& pieceLocations = pieceColor.IsWhite() ? m_WhitePieceLocations : m_BlackPieceLocations;

        for (uint8 pieceID = 0; pieceID < NUMBER_OF_PIECES_PER_COLOR; pieceID++)
        {
            if (pieceLocations[pieceID] == squareLocation)
            {
                return static_cast<PieceID>(pieceID);
            }
        }
        ERROR("No piece ID was found with that location");
    }


    void Chessboard::ClearBoard()
    {
        m_Board = InternalBoardRepresentation();
        m_BlackPieceLocations.fill(EMPTY);
        m_WhitePieceLocations.fill(EMPTY);
    }


    char Chessboard::GetCharacterOfPiece(const SquareLocation squareLocation) const
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board. Raw location: " << squareLocation.GetRawValue());
        const PieceColor squareColor = m_Board.ReadSquareColor(squareLocation);
        const PieceType squareType = m_Board.ReadSquareType(squareLocation);

        if (squareColor.IsWhite())
            { return static_cast<char>(toupper(squareType.ToCharacterSymbol())); }
        else
            { return squareType.ToCharacterSymbol(); }
    }


    void Chessboard::PrintBoardToConsole() const
    {
        std::cout << "\n";

        for (uint8 squareLocation = 0; squareLocation < 64; squareLocation++)
        {
            if (squareLocation % 8 == 0)
            { std::cout << "\n"; }

            std::cout << GetCharacterOfPiece(squareLocation) << " ";
        }
        std::cout << "\n";
    }


    void Chessboard::SetPieceType(const PieceID pieceID, const PieceType pieceType, const PieceColor pieceColor)
    {
        const SquareLocation pieceLocation = GetPieceLocation(pieceID, pieceColor);
        m_Board.WriteSquareType(pieceType, pieceLocation);
    }


    PieceID Chessboard::GetKingPieceID(const PieceColor kingColor) const
    {

        if (kingColor.IsWhite())
        {
            static bool hasBeenCalculated = false;
            static PieceID whiteKingPieceID;

            if (hasBeenCalculated) { return whiteKingPieceID; }

            for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
            {
                if (GetPieceType(static_cast<PieceID>(pieceID), kingColor) == PieceType::KING)
                {
                    whiteKingPieceID = static_cast<PieceID>(pieceID);
                    hasBeenCalculated = true;
                    return whiteKingPieceID;
                }
            }

        }
        else
        {
            static bool hasBeenCalculated = false;
            static PieceID blackKingPieceID;

            if (hasBeenCalculated) { return blackKingPieceID; }

            for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
            {
                if (GetPieceType(static_cast<PieceID>(pieceID), kingColor) == PieceType::KING)
                {
                    blackKingPieceID = static_cast<PieceID>(pieceID);
                    hasBeenCalculated = true;
                    return blackKingPieceID;
                }
            }

        }

        ERROR("King Piece ID not found!");
    }

}