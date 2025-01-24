//
// Created by Andrew Fagan on 1/23/25.
//

#include "Core/Containers/Vector.h"
#include <gtest/gtest.h>

class VectorTest : public ::testing::Test
{
public:
    Astral::Vector<int> m_DefaultVector = Astral::Vector<int>();
};

TEST_F(VectorTest, DefaultConstructor_SetsSizeAndCapacityToZero)
{
    EXPECT_EQ(m_DefaultVector.GetSize(), 0);
    EXPECT_EQ(m_DefaultVector.GetCapacity(), 0);
}

TEST_F(VectorTest, ReserveConstructor_HandlesZeroAsArgument)
{
    EXPECT_NO_THROW(Astral::Vector<int>(0));
}

TEST_F(VectorTest, InitializerConstructor_HandlesInitializerList)
{
    Astral::Vector<int> a;
    EXPECT_NO_THROW(a = Astral::Vector<int>({2, 1, 3, 4, 5, 7}));
    EXPECT_EQ(a[2], 3);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[5], 7);
    EXPECT_EQ(a.GetSize(), 6);
    EXPECT_EQ(a.GetCapacity(), 6);
}

/**@brief Tests if the copy constructor creates a separate instance from the other instance which can't change the other instance */
TEST_F(VectorTest, CopyConstructor_CreatesSeparateInstance)
{
    Astral::Vector<int> a(3);

    a.Add(2);
    a.Add(3);
    a.Add(4);

    EXPECT_EQ(a.GetSize(), 3);
    EXPECT_EQ(a.GetCapacity(), 3);

    Astral::Vector<int> b(a);

    EXPECT_EQ(a.GetSize(), b.GetSize());
    EXPECT_EQ(a.GetCapacity(), b.GetCapacity());

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[b.GetEndIndex()], b[a.GetEndIndex()]);

    a.Add(4);

    EXPECT_NE(a.GetSize(), b.GetSize());
    EXPECT_NE(a.GetCapacity(), b.GetCapacity());
}

/**@brief Tests if the copy asignment operator creates a separate instance from the other instance which can't change the other instance */
TEST_F(VectorTest, CopyAssignmentOperator_CreatesSeparateInstance)
{
    Astral::Vector<int> a(3);

    a.Add(2);
    a.Add(3);
    a.Add(4);

    EXPECT_EQ(a.GetSize(), 3);
    EXPECT_EQ(a.GetCapacity(), 3);

    Astral::Vector<int> b = Astral::Vector<int>();
    b = a;

    EXPECT_EQ(a.GetSize(), b.GetSize());
    EXPECT_EQ(a.GetCapacity(), b.GetCapacity());

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[b.GetEndIndex()], b[a.GetEndIndex()]);

    a.Add(4);

    EXPECT_NE(a.GetSize(), b.GetSize());
    EXPECT_NE(a.GetCapacity(), b.GetCapacity());
}

TEST_F(VectorTest, MoveConstructor_ClearsOtherInstanceState)
{
    Astral::Vector<int> a(3);

    a.Add(2);
    a.Add(3);
    a.Add(4);

    EXPECT_EQ(a.GetSize(), 3);
    EXPECT_EQ(a.GetCapacity(), 3);

    Astral::Vector<int> b(std::move(a));

    EXPECT_EQ(a.GetSize(), 0);
    EXPECT_EQ(a.GetCapacity(), 0);
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_EQ(b.GetCapacity(), 3);

    a.Add(23);

    EXPECT_EQ(a.GetSize(), 1);
    EXPECT_EQ(a.GetCapacity(), 1);
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_EQ(b.GetCapacity(), 3);
}

TEST_F(VectorTest, MoveAssignmentOperator_ClearsOtherInstanceState)
{
    Astral::Vector<int> a(3);

    a.Add(2);
    a.Add(3);
    a.Add(4);

    EXPECT_EQ(a.GetSize(), 3);
    EXPECT_EQ(a.GetCapacity(), 3);

    Astral::Vector<int> b(std::move(a));

    EXPECT_EQ(a.GetSize(), 0);
    EXPECT_EQ(a.GetCapacity(), 0);
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_EQ(b.GetCapacity(), 3);

    a.Add(23);

    EXPECT_EQ(a.GetSize(), 1);
    EXPECT_EQ(a.GetCapacity(), 1);
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_EQ(b.GetCapacity(), 3);
}

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

TEST_F(VectorTest, Add_WritesDataToInternalArray)
{
    EXPECT_EQ(m_DefaultVector.GetSize(), 0);

    m_DefaultVector.Add(1);
    EXPECT_EQ(m_DefaultVector[0], 1);

    m_DefaultVector.Add(0);
    EXPECT_EQ(m_DefaultVector[1], 0);

    m_DefaultVector.Add(-1);
    EXPECT_EQ(m_DefaultVector[2], -1);
}

/**@brief Tests if the Add method works with an uninitialized buffer (nullptr).  */
TEST_F(VectorTest, Add_WorksWithNullptrBuffer)
{
    EXPECT_EQ(m_DefaultVector.GetCapacity(), 0); // Default constructor sets buffer to nullptr
    m_DefaultVector.Add(2);
    EXPECT_EQ(m_DefaultVector.GetCapacity(), 1);
}

// TODO: Maybe add more tests for Add method