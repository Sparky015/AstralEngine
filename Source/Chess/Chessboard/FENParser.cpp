/**
* @file FENParser.cpp
* @author Andrew Fagan
* @date 12/27/2024
*/
#include "FENParser.h"

#include "../Common/ConversionHelpers.h"

namespace Game {

    FENParser::FENParser() : m_FullMoveCount(1), m_HalfMoveCount(1), activeColor(0) {}

    void FENParser::ParseFENString(const std::string& FEN_String)
    {
        PROFILE_SCOPE("FENParser::ParseFENString");
        // Initializing an empty board and empty piece locations
        m_Board = InternalBoardRepresentation();
        m_WhitePieceLocations.fill(EMPTY);
        m_BlackPieceLocations.fill(EMPTY);


        std::istringstream iss(FEN_String);
        iss >> pieceLocations >> activeColor >> castleRights >> enPassantPawn >> halfMoveCount >> fullMoveCount;

        PopulatePieceLocations();
        PopulateActiveColor();
        PopulateCastleRights();
        PopulateEnpassantState();
        PopulateMoveCounts();
    }


    void FENParser::PopulateMoveCounts()
    {
        m_HalfMoveCount = (!halfMoveCount.empty()) ? std::stoi(halfMoveCount) : 0;
        m_FullMoveCount = (!fullMoveCount.empty()) ? std::stoi(fullMoveCount) : 0;
    }


    void FENParser::PopulateActiveColor()
    {
        m_ActiveColor = (activeColor == 'w') ? PieceColor::WHITE : PieceColor::BLACK;
    }


    void FENParser::PopulateCastleRights()
    {
        if (castleRights[0] != '-')
        {
            for (const char castleRight: castleRights)
            {
                if (islower(castleRight))
                { m_CastleRights.SetCastleRight(PieceColor::BLACK, (castleRight == 'q') ? QUEEN_SIDE : KING_SIDE); }
                else
                { m_CastleRights.SetCastleRight(PieceColor::WHITE, (castleRight == 'Q') ? QUEEN_SIDE : KING_SIDE); }
            }
        }
    }


    void FENParser::PopulatePieceLocations()
    {
        uint8 whitePieceIDMarker = 0; // Holds the next white piece ID that needs to be assigned
        uint8 blackPieceIDMarker = 0; // Holds the next black piece ID that needs to be assigned

        // Going through each character in the FEN sequence and adding the piece to the board (top to bottom)
        SquareLocation squareLocation = SquareLocation(EMPTY);
        for (const char FEN_character : pieceLocations)
        {
            // Check if the character is a number (a number means to skip/set empty the that number of squares)
            if (isdigit(FEN_character))
            {
                squareLocation += FEN_character - '0';
            }
            else
            { // Then the character is a letter

                // Check to see if there is a break symbol (which means to go to the next row)
                if (FEN_character == '/')
                {
                    if (squareLocation % 8 == 0)
                    { continue; }

                    // Set squareLocation to the next row
                    squareLocation = squareLocation + 8 - (squareLocation % 8);
                    continue;
                }


                // Set the color. Upper case is white and lower case is black
                PieceColor pieceColor = isupper(FEN_character) ? PieceColor::WHITE : PieceColor::BLACK;
                m_Board.WriteSquareColor(pieceColor, squareLocation);

                // Set the piece type.
                m_Board.WriteSquareType(PieceType::CreateFromCharacter(FEN_character), squareLocation);

                // Set the location of the piece
                if (pieceColor.IsWhite())
                {
                    m_WhitePieceLocations[whitePieceIDMarker] = squareLocation;
                    whitePieceIDMarker++;
                }
                else
                {
                    m_BlackPieceLocations[blackPieceIDMarker] = squareLocation;
                    blackPieceIDMarker++;
                }

                ++squareLocation;
            }
        }
    }


    void FENParser::PopulateEnpassantState()
    {
        if (enPassantPawn != "-" && !enPassantPawn.empty())
        {
            m_PreviousMoveData.FinalPieceLocation = ConvertChessNotationToInt(enPassantPawn);
            const int8 initialPositionStep = (m_PreviousMoveData.FinalPieceLocation.GetRow() > 5) ? 8 : -8;
            m_PreviousMoveData.FinalPieceLocation += initialPositionStep;

            const uint8 moveStep = (m_PreviousMoveData.FinalPieceLocation.GetRow() > 5) ? -16 : 16;
            m_PreviousMoveData.InitialPieceLocation = m_PreviousMoveData.FinalPieceLocation + moveStep;

            m_PreviousMoveData.MovingPieceType = PieceType::PAWN;
            // m_PreviousMoveData.MovingPieceID = ReadSquarePieceID(m_PreviousMoveData.FinalPieceLocation);

            m_PreviousMoveData.PieceTakenType = PieceType::NONE;
            m_PreviousMoveData.TakenPieceID = PIECE_1; // == 0
        }
    }

}