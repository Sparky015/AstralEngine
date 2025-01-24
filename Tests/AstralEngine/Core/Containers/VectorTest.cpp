//
// Created by Andrew Fagan on 1/23/25.
//

#include "Core/Containers/Vector.h"
#include <gtest/gtest.h>

class VectorTest : ::testing::Test
{
public:
    Astral::Vector<int> m_DefaultVector = Astral::Vector<int>();
};

TEST_F(VectorTest, Add_GrowsVectorSize)
{
    EXPECT_EQ(m_DefaultVector.GetSize(), 0);

    m_DefaultVector.Add(1);
    EXPECT_EQ(m_DefaultVector.GetSize(), 1);

    m_DefaultVector.Add(0);
    EXPECT_EQ(m_DefaultVector.GetSize(), 2);

    m_DefaultVector.Add(-1);
    EXPECT_EQ(m_DefaultVector.GetSize(), 3);
}

/**@brief Tests that the Add method does not change the capacity when it does not meet the capacity limits. */
TEST_F(VectorTest, Add_DoesNotGrowCapacityWhenNotAtLimit)
{
    m_DefaultVector.Reserve(5);

    size_t m_OriginalCapacity = m_DefaultVector.GetCapacity();
    EXPECT_EQ(m_OriginalCapacity, 5);

    m_DefaultVector.Add(5);
    EXPECT_EQ(m_OriginalCapacity, m_DefaultVector.GetCapacity());

    m_DefaultVector.Add(5);
    m_DefaultVector.Add(11);
    m_DefaultVector.Add(222);
    EXPECT_EQ(m_DefaultVector.GetSize(), 4);
    EXPECT_EQ(m_OriginalCapacity, m_DefaultVector.GetCapacity());

    m_DefaultVector.Add(222);
    EXPECT_EQ(m_DefaultVector.GetSize(), 5);
    EXPECT_EQ(m_DefaultVector.GetCapacity(), m_OriginalCapacity * Astral::Vector<int>::GROWTH_FACTOR);
}
