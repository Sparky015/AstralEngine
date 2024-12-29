/**
* @file PieceTypeTests.cpp
* @author Andrew Fagan
* @date 12/28/2024
*/

#include "gtest/gtest.h"
#include "GameState/PieceType.h"

// Test fixture for PieceType tests
class PieceTypeTest : public ::testing::Test {
protected:
    // Setup runs before each test
    void SetUp() override {
        // Initialize any common resources here
    }

    // TearDown runs after each test
    void TearDown() override {
        // Clean up any resources here
    }
};

// 1. Test Constructors and Default Values
TEST_F(PieceTypeTest, DefaultConstructor_CreatesNonePiece) {
    PieceType piece;
    EXPECT_TRUE(piece.IsNone());
    EXPECT_EQ(piece.GetEnumValue(), PieceType::NONE);
}

// 2. Test Enum Constructor
TEST_F(PieceTypeTest, EnumConstructor_CreatesCorrectPieceType) {
    PieceType pawn(PieceType::PAWN);
    EXPECT_TRUE(pawn.IsPawn());
    EXPECT_FALSE(pawn.IsNone());
}

// 3. Test Type Checking Methods
TEST_F(PieceTypeTest, TypeChecking_CorrectlyIdentifiesPieceTypes) {
    // Test each piece type
    struct TestCase {
        PieceType piece;
        bool (PieceType::*checkMethod)() const;
        const char* description;
    };

    std::vector<TestCase> testCases = {
        {PieceType(PieceType::PAWN), &PieceType::IsPawn, "Pawn"},
        {PieceType(PieceType::BISHOP), &PieceType::IsBishop, "Bishop"},
        {PieceType(PieceType::KNIGHT), &PieceType::IsKnight, "Knight"},
        {PieceType(PieceType::ROOK), &PieceType::IsRook, "Rook"},
        {PieceType(PieceType::QUEEN), &PieceType::IsQueen, "Queen"},
        {PieceType(PieceType::KING), &PieceType::IsKing, "King"}
    };

    for (const auto& testCase : testCases) {
        EXPECT_TRUE((testCase.piece.*testCase.checkMethod)())
            << "Failed to identify " << testCase.description;
    }
}

// 4. Test String Conversion
TEST_F(PieceTypeTest, ToString_ReturnsCorrectString) {
    struct TestCase {
        PieceType piece;
        std::string_view expectedString;
    };

    std::vector<TestCase> testCases = {
        {PieceType(PieceType::NONE), "None"},
        {PieceType(PieceType::PAWN), "Pawn"},
        {PieceType(PieceType::BISHOP), "Bishop"},
        {PieceType(PieceType::KNIGHT), "Knight"},
        {PieceType(PieceType::ROOK), "Rook"},
        {PieceType(PieceType::QUEEN), "Queen"},
        {PieceType(PieceType::KING), "King"}
    };

    for (const auto& testCase : testCases) {
        EXPECT_EQ(testCase.piece.ToString(), std::string(testCase.expectedString));
        EXPECT_EQ(testCase.piece.ToStringView(), testCase.expectedString);
    }
}

// 5. Test Character Symbol Conversion
TEST_F(PieceTypeTest, CharacterSymbol_Conversion) {
    struct TestCase
    {
        PieceType piece;
        char expectedSymbol;
    };

    std::vector<TestCase> testCases = {
        {PieceType(PieceType::NONE), '-'},
        {PieceType(PieceType::PAWN), 'p'},
        {PieceType(PieceType::BISHOP), 'b'},
        {PieceType(PieceType::KNIGHT), 'n'},
        {PieceType(PieceType::ROOK), 'r'},
        {PieceType(PieceType::QUEEN), 'q'},
        {PieceType(PieceType::KING), 'k'}
    };

    // Test ToCharacterSymbol
    for (const auto& testCase : testCases) {
        EXPECT_EQ(testCase.piece.ToCharacterSymbol(), testCase.expectedSymbol);
    }

    // Test CreateFromCharacter
    for (const auto& testCase : testCases) {
        if (testCase.piece != PieceType::NONE) {  // Skip NONE as it's not created from character
            EXPECT_EQ(PieceType::CreateFromCharacter(testCase.expectedSymbol), testCase.piece);
            // Test uppercase version too
            EXPECT_EQ(PieceType::CreateFromCharacter(toupper(testCase.expectedSymbol)), testCase.piece);
        }
    }
}

// 6. Test Invalid Character Creation
TEST_F(PieceTypeTest, CreateFromCharacter_InvalidInput_ThrowsError)
{
    EXPECT_THROW(PieceType::CreateFromCharacter('x'), std::runtime_error);
}

// 7. Test Operator Overloads
TEST_F(PieceTypeTest, OperatorOverloads)
{
    PieceType piece1(PieceType::PAWN);
    PieceType piece2(PieceType::PAWN);
    PieceType piece3(PieceType::KNIGHT);

    // Test equality operators
    EXPECT_TRUE(piece1 == piece2);
    EXPECT_FALSE(piece1 == piece3);
    EXPECT_TRUE(piece1 != piece3);
    EXPECT_FALSE(piece1 != piece2);

    // Test enum comparison
    EXPECT_TRUE(piece1 == PieceType::PAWN);
    EXPECT_FALSE(piece1 == PieceType::KNIGHT);
}

// 8. Test Raw Value Conversion
TEST_F(PieceTypeTest, RawValueConversion)
{
    for (uint8 i = 0; i <= static_cast<uint8>(PieceType::KING); ++i)
    {
        PieceType piece = PieceType::CreateFromRawValue(i);
        EXPECT_EQ(piece.GetRawValue(), i);
    }
}

// 9. Test Copy and Move Operations
TEST_F(PieceTypeTest, CopyAndMoveOperations)
{
    PieceType original(PieceType::QUEEN);

    // Test copy constructor
    PieceType copied(original);
    EXPECT_EQ(copied, original);

    // Test move constructor
    PieceType moved(std::move(copied));
    EXPECT_EQ(moved, original);

    // Test copy assignment
    PieceType copyAssigned;
    copyAssigned = original;
    EXPECT_EQ(copyAssigned, original);

    // Test move assignment
    PieceType moveAssigned;
    moveAssigned = std::move(moved);
    EXPECT_EQ(moveAssigned, original);
}