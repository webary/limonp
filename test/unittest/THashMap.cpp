#include "limonp/ForcePublic.hpp"
#include "limonp/HashMap.hpp"
#include "gtest/gtest.h"
#include <map>

#define SIZEOF(x) (sizeof(x)/sizeof(*x))

using namespace limonp;
using namespace std;

static int TEST_DATA[] = {1, 3, 5, 10, 19, 82, 65, 78, 12, 14, 12, 15, 19, 79};
static int TEST_DATA_NOT_EXISTA[] = {2, 4, 6, 7, 8, 9};

TEST(HashMap, Test1) {
  HashMap<int, int> mp;
  map<int, int> stdmp;
  for (size_t i = 0; i < SIZEOF(TEST_DATA); ++i) {
    int x = TEST_DATA[i];
    mp.Insert(pair<int,int>(x, i));
    stdmp[x] = i;
  }

  HashMap<int, int>::const_iterator it;
  for (size_t i = 0; i < SIZEOF(TEST_DATA); ++i) {
    int x = TEST_DATA[i];
    it = mp.Find(x);
    ASSERT_NE(it, mp.End());
    ASSERT_EQ(it->first, x);
    ASSERT_EQ(it->second, stdmp[x]);
  }

  for (size_t i = 0; i < SIZEOF(TEST_DATA_NOT_EXISTA); ++i) {
    int x = TEST_DATA_NOT_EXISTA[i];
    it = mp.Find(x);
    ASSERT_EQ(it, mp.End());
  }
}

TEST(HashMap, Test2) {
  HashMap<int, int> mp;
  ASSERT_EQ(mp.Begin(), mp.End());
  ASSERT_TRUE(mp.Insert(pair<int,int>(1, 1)));
  ASSERT_NE(mp.Begin(), mp.End());
  HashMap<int, int>::const_iterator it;
  it = mp.Find(1);
  ASSERT_NE(it, mp.End());
  ASSERT_EQ(it->first, 1);
  ASSERT_EQ(it->second, 1);
  ASSERT_FALSE(mp.Insert(pair<int,int>(1, 2)));
  it = mp.Find(1);
  ASSERT_NE(it, mp.End());
  ASSERT_EQ(it->first, 1);
  ASSERT_EQ(it->second, 2);
  ASSERT_EQ(mp.buckets_.size(), 3);
  ASSERT_EQ(mp.size_, 1);
  ASSERT_TRUE(mp.Insert(pair<int,int>(5, 1)));
  ASSERT_EQ(mp.size_, 2);
  ASSERT_EQ(mp.buckets_.size(), 3);
  ASSERT_TRUE(mp.Insert(pair<int,int>(3, 1)));
  ASSERT_EQ(mp.size_, 3);
  ASSERT_EQ(mp.buckets_.size(), 3);
  ASSERT_TRUE(mp.Insert(pair<int,int>(4, 1)));
  ASSERT_EQ(mp.size_, 4);
  ASSERT_EQ(mp.buckets_.size(), 7);

  mp.Rehash(1);
  ASSERT_EQ(mp.size_, 4);
  ASSERT_EQ(mp.buckets_.size(), 1);
}
