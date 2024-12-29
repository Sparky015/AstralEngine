/**
* @file ChessBoardTest.cpp
* @author Andrew Fagan
* @date 12/26/2024
*/

#include <gtest/gtest.h>

class ChessBoardTest : public::testing::Test
{
public:

};

/**
 * @brief Verifies that move piece correctly modifies the state of the chessboard
 * @test Ensures that */
TEST_F(ChessBoardTest, MovePiece_ModifiesBoardStateCorrectly)
{
    struct TestCase
    {

        bool expectedResult;
        std::string_view testDescription;
    };

    std::vector<TestCase> testCases = {

    };

    for (const auto& testCase : testCases)
    {

    }
}
