//
// Created by Andrew Fagan on 12/25/24.
//

#include <gtest/gtest.h>

#include "GameState/SquareLocation.h"

class SquareLocationTest : public ::testing::Test
{
public:
    SquareLocation a1 = SquareLocation(A1);
    SquareLocation b6 = SquareLocation(B6);
    SquareLocation d1 = SquareLocation(D1);
    SquareLocation e2 = SquareLocation(E2);
    SquareLocation e4 = SquareLocation(E4);
    SquareLocation f5 = SquareLocation(F5);
    SquareLocation g1 = SquareLocation(G1);
    SquareLocation g4 = SquareLocation(G4);
    SquareLocation g8 = SquareLocation(G8);

};


TEST_F(SquareLocationTest, IsOnBoard)
{
    EXPECT_TRUE(e4.IsOnBoard());
    EXPECT_TRUE(g8.IsOnBoard());
    EXPECT_TRUE(a1.IsOnBoard());

    SquareLocation offBoardLocation(EMPTY);
    SquareLocation offBoardLocation2(64);
    EXPECT_FALSE(offBoardLocation.IsOnBoard());
    EXPECT_FALSE(offBoardLocation2.IsOnBoard());
}


TEST_F(SquareLocationTest, IsOffBoard)
{
    EXPECT_FALSE(e4.IsOffBoard());
    EXPECT_FALSE(g8.IsOffBoard());
    EXPECT_FALSE(a1.IsOffBoard());

    SquareLocation offBoardLocation(EMPTY);
    SquareLocation offBoardLocation2(64);
    EXPECT_TRUE(offBoardLocation.IsOffBoard());
    EXPECT_TRUE(offBoardLocation2.IsOffBoard());
}


TEST_F(SquareLocationTest, GetRow)
{
    EXPECT_EQ(e4.GetRow(), 4);
    EXPECT_EQ(a1.GetRow(), 1);
    EXPECT_EQ(g8.GetRow(), 8);
    EXPECT_EQ(d1.GetRow(), 1);
    EXPECT_EQ(g4.GetRow(), 4);
    EXPECT_EQ(b6.GetRow(), 6);
    EXPECT_EQ(f5.GetRow(), 5);
}


TEST_F(SquareLocationTest, GetColumn)
{
    EXPECT_EQ(e4.GetColumn(), 5);
    EXPECT_EQ(a1.GetColumn(), 1);
    EXPECT_EQ(d1.GetColumn(), 4);
    EXPECT_EQ(g8.GetColumn(), 7);
    EXPECT_EQ(g4.GetColumn(), 7);
    EXPECT_EQ(b6.GetColumn(), 2);
    EXPECT_EQ(f5.GetColumn(), 6);
}


TEST_F(SquareLocationTest, IsInSameRow)
{
    EXPECT_TRUE(a1.IsInSameRow(d1));
    EXPECT_TRUE(e4.IsInSameRow(g4));
    EXPECT_FALSE(g1.IsInSameRow(g4));
    EXPECT_FALSE(e2.IsInSameRow(a1));
    EXPECT_FALSE(f5.IsInSameRow(b6));
}


TEST_F(SquareLocationTest, IsInSameColumn)
{
    EXPECT_FALSE(a1.IsInSameColumn(d1));
    EXPECT_FALSE(e4.IsInSameColumn(g4));
    EXPECT_FALSE(f5.IsInSameColumn(b6));
    EXPECT_TRUE(g1.IsInSameColumn(g4));
    EXPECT_TRUE(e2.IsInSameColumn(e4));
}


TEST_F(SquareLocationTest, GetRawValue)
{
    EXPECT_EQ(e4.GetRawValue(), E4);
    EXPECT_EQ(g8.GetRawValue(), G8);
    EXPECT_EQ(a1.GetRawValue(), A1);
    EXPECT_EQ(d1.GetRawValue(), D1);
    EXPECT_EQ(b6.GetRawValue(), B6);
    EXPECT_EQ(f5.GetRawValue(), F5);
}


// SquareLocation a1 = SquareLocation(A1);
// SquareLocation b6 = SquareLocation(B6);
// SquareLocation d1 = SquareLocation(D1);
// SquareLocation e2 = SquareLocation(E2);
// SquareLocation e4 = SquareLocation(E4);
// SquareLocation f5 = SquareLocation(F5);
// SquareLocation g1 = SquareLocation(G1);
// SquareLocation g4 = SquareLocation(G4);
// SquareLocation g8 = SquareLocation(G8);
TEST_F(SquareLocationTest, FromRowAndColumn)
{
    SquareLocation location1 = SquareLocation::FromRowAndColumn(8, 1);
    EXPECT_EQ(location1.GetRawValue(), A1);
    SquareLocation location2 = SquareLocation::FromRowAndColumn(1, 8);
    EXPECT_EQ(location2.GetRawValue(), G8);
}


TEST_F(SquareLocationTest, OperatorEquals_NormalOperation) {
    // Test 1: Compare same squares
    SquareLocation e4Copy(E4);
    EXPECT_TRUE(e4 == e4Copy);

    // Test 2: Compare different squares
    EXPECT_FALSE(e4 == g4);
}

TEST_F(SquareLocationTest, OperatorEquals_BoundaryConditions) {
    // Test 1: Compare with bottom-left corner
    EXPECT_TRUE(a1 == SquareLocation(A1));

    // Test 2: Compare with top-right corner
    EXPECT_TRUE(g8 == SquareLocation(G8));
}

TEST_F(SquareLocationTest, OperatorEquals_InvalidOperations) {
    // Test with invalid square location
    SquareLocation invalidSquare(EMPTY);
    EXPECT_FALSE(e4 == invalidSquare);
}


TEST_F(SquareLocationTest, OperatorNotEquals)
{
    SquareLocation loc1(42);
    SquareLocation loc2(43);
    EXPECT_TRUE(loc1 != loc2);
    SquareLocation loc3(42);
    EXPECT_FALSE(loc1 != loc3);
}


TEST_F(SquareLocationTest, OperatorPlusInt)
{
    SquareLocation loc(42);
    SquareLocation result = loc + 1;
    EXPECT_EQ(result.GetRawValue(), 43);
}


TEST_F(SquareLocationTest, OperatorMinusInt)
{
    SquareLocation loc(42);
    SquareLocation result = loc - 1;
    EXPECT_EQ(result.GetRawValue(), 41);
}


TEST_F(SquareLocationTest, OperatorPlusSquareLocation)
{
    SquareLocation loc1(21);
    SquareLocation loc2(21);
    SquareLocation result = loc1 + loc2;
    EXPECT_EQ(result.GetRawValue(), 42);
}


TEST_F(SquareLocationTest, OperatorMinusSquareLocation)
{
    SquareLocation loc1(43);
    SquareLocation loc2(1);
    SquareLocation result = loc1 - loc2;
    EXPECT_EQ(result.GetRawValue(), 42);
}


TEST_F(SquareLocationTest, OperatorPlusEquals)
{
    SquareLocation loc1(21);
    SquareLocation loc2(21);
    loc1 += loc2;
    EXPECT_EQ(loc1.GetRawValue(), 42);
}


TEST_F(SquareLocationTest, OperatorMinusEquals)
{
    SquareLocation loc1(43);
    SquareLocation loc2(1);
    loc1 -= loc2;
    EXPECT_EQ(loc1.GetRawValue(), 42);
}


TEST_F(SquareLocationTest, OperatorMod)
{
    SquareLocation loc(42);
    EXPECT_EQ(loc % 10, 2);
}


TEST_F(SquareLocationTest, OperatorPreIncrement)
{
    SquareLocation loc(42);
    ++loc;
    EXPECT_EQ(loc.GetRawValue(), 43);
}


TEST_F(SquareLocationTest, OperatorPostIncrement)
{
    SquareLocation loc(42);
    SquareLocation result = loc++;
    EXPECT_EQ(result.GetRawValue(), 42);
    EXPECT_EQ(loc.GetRawValue(), 43);
}


TEST_F(SquareLocationTest, OperatorPreDecrement)
{
    SquareLocation loc(42);
    --loc;
    EXPECT_EQ(loc.GetRawValue(), 41);
}


TEST_F(SquareLocationTest, OperatorPostDecrement)
{
    SquareLocation loc(42);
    SquareLocation result = loc--;
    EXPECT_EQ(result.GetRawValue(), 42);
    EXPECT_EQ(loc.GetRawValue(), 41);
}