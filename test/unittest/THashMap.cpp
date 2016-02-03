#include "limonp/ForcePublic.hpp"
#include "limonp/HashMap.hpp"
#include "gtest/gtest.h"

using namespace limonp;
using namespace std;

TEST(HashMap, Test1) {
  HashMap<int, int> mp;
  ASSERT_EQ(mp.Begin(), mp.End());
  ASSERT_TRUE(mp.Insert(pair<int,int>(1, 1)));
  HashMap<int, int>::const_iterator it;
  it = mp.Find(1);
  ASSERT_NE(it, mp.End());
  ASSERT_EQ(it->first, 1);
  ASSERT_EQ(it->second, 1);
  ASSERT_FALSE(mp.Insert(pair<int,int>(1, 2)));
}
