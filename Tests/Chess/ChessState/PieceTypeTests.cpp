/**
* @file PieceTypeTests.cpp
* @author Andrew Fagan
* @date 12/28/20
*/

#include "gtest/gtest.h"
#include "GameState/PieceType.h"

/**
 * @brief Test suite for the PieceType class
 * This test fixture verifies the functionality of the PieceType class,
 * which represents different chess piece types in the game. */
class PieceTypeTest : public ::testing::Test {};


/**
 * @brief Verifies that the default constructor creates a NONE piece  */
TEST_F(PieceTypeTest, DefaultConstructor_CreatesNonePiece)
{
    PieceType piece;
    EXPECT_TRUE(piece.IsNone());
    EXPECT_EQ(piece.GetEnumValue(), PieceType::NONE);
}


/**
 * @brief Verifies that the enum constructor creates the corresponding type */
TEST_F(PieceTypeTest, EnumConstructor_CreatesCorrectPieceType)
{
    struct TestCase
    {
        PieceType pieceType;
        PieceTypeEnum pieceTypeEnum;
        bool expectedResult;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {
        {PieceType(PieceType::NONE), PieceType::NONE, true, "NONE matches NONE"},
        {PieceType(PieceType::PAWN), PieceType::PAWN, true, "PAWN matches PAWN"},
        {PieceType(PieceType::BISHOP), PieceType::BISHOP, true, "BISHOP matches BISHOP"},
        {PieceType(PieceType::KNIGHT), PieceType::KNIGHT, true, "KNIGHT matches KNIGHT"},
        {PieceType(PieceType::ROOK), PieceType::ROOK, true, "ROOK matches ROOK"},
        {PieceType(PieceType::QUEEN), PieceType::QUEEN, true, "QUEEN matches QUEEN"},
        {PieceType(PieceType::KING), PieceType::KING, true, "KING matches KING"},
        {PieceType(PieceType::PAWN), PieceType::BISHOP, false, "PAWN does not match BISHOP"},
        {PieceType(PieceType::KNIGHT), PieceType::ROOK, false, "KNIGHT does not match ROOK"},
        {PieceType(PieceType::NONE), PieceType::PAWN, false, "NONE does not match PAWN"},
        {PieceType(PieceType::PAWN), PieceType::NONE, false, "PAWN does not match NONE"},
        {PieceType(PieceType::NONE), PieceType::KNIGHT, false, "NONE does not match KNIGHT"},
        {PieceType(PieceType::KING), PieceType::ROOK, false, "KING does not match ROOK"},
    };

    for (const auto& testCase : testCases)
    {
        EXPECT_EQ(testCase.pieceType.GetEnumValue() == testCase.pieceTypeEnum, testCase.expectedResult)
            << "Failed enum value comparison. Case Description: " << testCase.testDescription;

        EXPECT_EQ(testCase.pieceType.GetRawValue() == PieceType(testCase.pieceTypeEnum).GetRawValue(), testCase.expectedResult)
            << "Failed raw value comparison. Case Description: " << testCase.testDescription;

        EXPECT_EQ(testCase.pieceType.ToStringView() == PieceType(testCase.pieceTypeEnum).ToStringView(), testCase.expectedResult)
            << "Failed string representation comparison. Case Description: " << testCase.testDescription;
    }
}

/**
 * @brief Verifies the IsPieceType functions are return correct outputs */
TEST_F(PieceTypeTest, TypeChecking_CorrectlyIdentifiesPieceTypes)
{
    struct TestCase
    {
        PieceType pieceType;
        bool (PieceType::*checkMethod)() const;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {
        {PieceType(PieceType::PAWN), &PieceType::IsPawn, "IsPawn with PAWN"},
        {PieceType(PieceType::BISHOP), &PieceType::IsBishop, "IsBishop with BISHOP"},
        {PieceType(PieceType::KNIGHT), &PieceType::IsKnight, "IsKnight with KNIGHT"},
        {PieceType(PieceType::ROOK), &PieceType::IsRook, "IsRook with ROOK"},
        {PieceType(PieceType::QUEEN), &PieceType::IsQueen, "IsQueen with QUEEN"},
        {PieceType(PieceType::KING), &PieceType::IsKing, "IsKing with KING"}
    };

    for (const auto& testCase : testCases)
    {
        EXPECT_TRUE((testCase.pieceType.*testCase.checkMethod)())
            << "Failed to identify " << testCase.testDescription;
    }
}

/**
 * @brief Verifies that ToString and ToStringView returns the correct corresponding string */
TEST_F(PieceTypeTest, ToString_ReturnsCorrectString)
{
    struct TestCase
    {
        PieceType pieceType;
        std::string_view expectedString;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases =
    {
        {PieceType(PieceType::NONE), "None", "NONE matches \"None\""},
        {PieceType(PieceType::PAWN), "Pawn", "PAWN matches \"Pawn\""},
        {PieceType(PieceType::BISHOP), "Bishop", "BISHOP matches \"Bishop\""},
        {PieceType(PieceType::KNIGHT), "Knight", "KNIGHT matches \"Knight\""},
        {PieceType(PieceType::ROOK), "Rook", "ROOK matches \"Rook\""},
        {PieceType(PieceType::QUEEN), "Queen", "QUEEN matches \"Queen\""},
        {PieceType(PieceType::KING), "King", "KING matches \"King\""}
    };

    for (const auto& testCase : testCases)
    {
        EXPECT_EQ(testCase.pieceType.ToString(), std::string(testCase.expectedString))
            << "Failed ToString() comparison. Test Case Description: " << testCase.testDescription;
        EXPECT_EQ(testCase.pieceType.ToStringView(), testCase.expectedString)
            << "Failed ToStringView() comparison. Test Case Description: " << testCase.testDescription;
    }
}

/**
 * @brief Verifies the symbol getter and PieceType creation from symbol */
TEST_F(PieceTypeTest, CharacterSymbol_Conversion)
{
    struct TestCase
    {
        PieceType pieceType;
        char symbol;
        bool expectedResult;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {
        {PieceType(PieceType::NONE),   '-', true,  "NONE matches '-'"},
        {PieceType(PieceType::PAWN),   'p', true,  "PAWN matches 'p'"},
        {PieceType(PieceType::BISHOP), 'b', true,  "BISHOP matches 'b'"},
        {PieceType(PieceType::KNIGHT), 'n', true,  "KNIGHT matches 'n'"},
        {PieceType(PieceType::ROOK),   'r', true,  "ROOK matches 'r'"},
        {PieceType(PieceType::QUEEN),  'q', true,  "QUEEN matches 'q'"},
        {PieceType(PieceType::KING),   'k', true,  "KING matches 'k'"},
        {PieceType(PieceType::NONE),   'p', false, "NONE does not match 'p'"},
        {PieceType(PieceType::PAWN),   '-', false, "PAWN does not match '-'"},
        {PieceType(PieceType::BISHOP), 'k', false, "BISHOP does not match 'k'"},
        {PieceType(PieceType::KNIGHT), 'r', false, "KNIGHT does not match 'r'"},
        {PieceType(PieceType::ROOK),   'q', false, "ROOK does not match 'q'"},
        {PieceType(PieceType::QUEEN),  'k', false, "QUEEN does not match 'k'"},
        {PieceType(PieceType::KING),   '-', false, "KING does not match '-'"}
    };

    // Test ToCharacterSymbol
    for (const auto& testCase : testCases)
    {
        EXPECT_EQ(testCase.pieceType.ToCharacterSymbol() == testCase.symbol, testCase.expectedResult)
            << "Failed ToCharacterSymbol() comparison. Test Case Description: " << testCase.testDescription;
    }

    // Test CreateFromCharacter
    for (const auto& testCase : testCases)
    {
        EXPECT_EQ(PieceType::CreateFromCharacter(testCase.symbol) == testCase.pieceType, testCase.expectedResult)
            << "Failed lower case CreateFromCharacter() comparison. Test Case Description: " << testCase.testDescription;
        EXPECT_EQ(PieceType::CreateFromCharacter(toupper(testCase.symbol)) == testCase.pieceType, testCase.expectedResult) // Test uppercase version too
            << "Failed upper case CreateFromCharacter() comparison. Test Case Description: " << testCase.testDescription;
    }
}

/**
 * @brief Verifies that CreateFromCharacter errors from invalid input */
TEST_F(PieceTypeTest, CreateFromCharacter_InvalidInput_ThrowsError)
{
    EXPECT_THROW(PieceType::CreateFromCharacter('x'), std::runtime_error);
    EXPECT_THROW(PieceType::CreateFromCharacter('_'), std::runtime_error);
    EXPECT_THROW(PieceType::CreateFromCharacter('='), std::runtime_error);
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('-'));
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('p'));
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('n'));
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('b'));
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('r'));
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('k'));
    EXPECT_NO_THROW(PieceType::CreateFromCharacter('q'));
}

/**
 * @brief Verifies that the operator overloads evaluate correctly */
TEST_F(PieceTypeTest, OperatorOverloads)
{
    struct TestCase
    {
        PieceType pieceType1;
        PieceType pieceType2;
        bool expectedResult;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {
        {PieceType::PAWN, PieceType::PAWN, true, "Comparing PAWN and PAWN"},
        {PieceType::BISHOP, PieceType::BISHOP, true, "Comparing BISHOP and BISHOP"},
        {PieceType::KNIGHT, PieceType::KNIGHT, true, "Comparing KNIGHT and KNIGHT"},
        {PieceType::ROOK, PieceType::ROOK, true, "Comparing ROOK and ROOK"},
        {PieceType::QUEEN, PieceType::QUEEN, true, "Comparing QUEEN and QUEEN"},
        {PieceType::KING, PieceType::KING, true, "Comparing KING and KING"},
        {PieceType::NONE, PieceType::NONE, true, "Comparing NONE and NONE"},
        {PieceType::PAWN, PieceType::NONE, false, "Comparing PAWN and NONE"},
        {PieceType::BISHOP, PieceType::KNIGHT, false, "Comparing BISHOP and KNIGHT"},
        {PieceType::ROOK, PieceType::QUEEN, false, "Comparing ROOK and QUEEN"},
        {PieceType::KING, PieceType::PAWN, false, "Comparing KING and PAWN"},
        {PieceType::QUEEN, PieceType::BISHOP, false, "Comparing QUEEN and BISHOP"},
        {PieceType::KNIGHT, PieceType::ROOK, false, "Comparing KNIGHT and ROOK"}
    };

    for (const auto& testCase : testCases)
    {
        EXPECT_EQ(testCase.pieceType1 == testCase.pieceType2, testCase.expectedResult)
            << "PieceType to PieceType comparison failed. Test Case Description: " << testCase.testDescription;

        EXPECT_EQ(testCase.pieceType1 == testCase.pieceType2.GetEnumValue(), testCase.expectedResult)
            << "PieceType to PieceTypeEnum comparison failed. Test Case Description: " << testCase.testDescription;

        EXPECT_EQ(testCase.pieceType1.GetEnumValue() == testCase.pieceType2, testCase.expectedResult)
            << "PieceTypeEnum to PieceType comparison failed. Test Case Description: " << testCase.testDescription;

        EXPECT_EQ(testCase.pieceType1.GetEnumValue() == testCase.pieceType2.GetEnumValue(), testCase.expectedResult)
            << "PieceTypeEnum to PieceTypeEnum comparison failed. Test Case Description: " << testCase.testDescription;
    }
}


/**
 * @brief Verifies that the raw value getter and creator returns correct values */
TEST_F(PieceTypeTest, RawValueConversion)
{
    for (uint8 i = 0; i <= PieceType::NUM_OF_PIECE_TYPES; ++i)
    {
        PieceType piece = PieceType::CreateFromRawValue(i);
        EXPECT_EQ(piece.GetRawValue(), i);
    }

    struct TestCase
    {
        PieceType pieceType;
        uint8 rawValue;
        bool expectedResult;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {
        {PieceType::NONE, 0, true, "Comparing NONE and the raw value of 0"},
        {PieceType::PAWN, 1, true, "Comparing PAWN and the raw value of 1"},
        {PieceType::BISHOP, 2, true, "Comparing BISHOP and the raw value of 2"},
        {PieceType::KNIGHT, 3, true, "Comparing KNIGHT and the raw value of 3"},
        {PieceType::ROOK, 4, true, "Comparing ROOK and the raw value of 4"},
        {PieceType::QUEEN, 5, true, "Comparing QUEEN and the raw value of 5"},
        {PieceType::KING, 6, true, "Comparing KING and the raw value of 6"},
        {PieceType::NONE, 1, false, "Comparing NONE and the raw value of 1"},
        {PieceType::PAWN, 0, false, "Comparing PAWN and the raw value of 0"},
        {PieceType::BISHOP, 3, false, "Comparing BISHOP and the raw value of 1"},
        {PieceType::KNIGHT, 2, false, "Comparing KNIGHT and the raw value of 2"},
        {PieceType::ROOK, 5, false, "Comparing ROOK and the raw value of 3"},
        {PieceType::QUEEN, 6, false, "Comparing QUEEN and the raw value of 4"}
    };

    for (const auto& testCase : testCases)
    {
        EXPECT_EQ(testCase.pieceType.GetRawValue() == testCase.rawValue, testCase.expectedResult)
            << "(GetRawValue) Raw value comparison failed. Test Case Description: " << testCase.testDescription;

        EXPECT_EQ(testCase.pieceType == PieceType::CreateFromRawValue(testCase.rawValue), testCase.expectedResult)
            << "(CreateFromRawValue) PieceType comparison failed. Test Case Description: " << testCase.testDescription;
    }
}


/**
 * @brief Verifies that the NUM_OF_PIECE_TYPES is the max value of PieceTypeEnum */
TEST_F(PieceTypeTest, NumberOfPieceTypes_CorrectlyIdentifiesAmountOfPieceTypes)
{
    EXPECT_EQ(PieceType::NUM_OF_PIECE_TYPES, static_cast<uint8>(PieceType::KING) + 1)
            << "NUM_OF_PIECE_TYPES should be equal to the max value of PieceTypeEnum plus one.";

    std::vector<PieceTypeEnum> restOfPieceTypes = {
            PieceTypeEnum::NONE, PieceTypeEnum::PAWN, PieceTypeEnum::KNIGHT,
            PieceTypeEnum::BISHOP, PieceTypeEnum::ROOK, PieceTypeEnum::QUEEN};

    for (const auto& otherPieceType : restOfPieceTypes)
    {
        EXPECT_GT(static_cast<uint8>(PieceType::KING), static_cast<uint8>(otherPieceType))
            << "King should be greater than " << PieceType(otherPieceType).ToStringView();
    }

}