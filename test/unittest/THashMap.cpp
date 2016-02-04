#include "limonp/HashMap.hpp"
#include "gtest/gtest.h"
#include <map>

#define SIZEOF(x) (sizeof(x)/sizeof(*x))

using namespace limonp;
using namespace std;

static int TEST_DATA[] = {1, 3, 5, 10, 19, 82, 65, 78, 12, 14, 12, 15, 19, 79};
static int TEST_DATA_NOT_EXISTA[] = {2, 4, 6, 7, 8, 9};

TEST(HashMap, IteratorTest0) {
  const size_t N = 6;
  HashMap<int, int> mp;
  ASSERT_EQ(mp.Size(), 0u);
  mp.Rehash(4);
  ASSERT_EQ(mp.Size(), 0u);
  ASSERT_EQ(mp.BucketSize(), 4u);
  for (size_t i = 0; i < N; i+=2) {
    ASSERT_TRUE(mp.Insert(make_pair(i, i)));
  }
  ASSERT_EQ(mp.Size(), 3u);
  ASSERT_EQ(mp.BucketSize(), 4u);
  for (size_t i = 0; i < N; i+=2) {
    ASSERT_FALSE(mp.Insert(make_pair(i, i)));
  }
  ASSERT_EQ(mp.Size(), 3u);
  ASSERT_EQ(mp.BucketSize(), 4u);

  const size_t arr[] = {0, 4, 2};
  size_t len = SIZEOF(arr);
  
  size_t t;
  t = 0;
  for (HashMap<int, int>::const_iterator it = mp.Begin(); it != mp.End(); ++it) {
    ASSERT_LT(t, len);
    ASSERT_EQ(it->second, arr[t]);
    t++;
  }

  mp.Rehash(6);
  ASSERT_EQ(3, mp.Size());
  ASSERT_EQ(6, mp.BucketSize());
  t = 0;
  for (HashMap<int, int>::const_iterator it = mp.Begin(); it != mp.End(); ++it) {
    ASSERT_EQ(t, it->second);
    t+=2;
  }
}

TEST(HashMap, Test0) {
  const size_t N = 100;
  HashMap<int, int> mp;
  ASSERT_EQ(mp.Size(), 0u);
  ASSERT_EQ(mp.BucketSize(), 0u);
  for (size_t i = 0; i < N; ++i) {
    ASSERT_TRUE(mp.Insert(make_pair(i, i)));
  }
  ASSERT_EQ(mp.Size(), 100u);
  ASSERT_EQ(mp.BucketSize(), 163u);
  for (size_t i = 0; i < N; ++i) {
    ASSERT_FALSE(mp.Insert(make_pair(i, i)));
  }
  ASSERT_EQ(mp.Size(), 100u);
  ASSERT_EQ(mp.BucketSize(), 163u);

  size_t cur = 0;
  for (HashMap<int, int>::const_iterator it = mp.Begin(); it != mp.End(); ++it) {
    ASSERT_EQ(it->second, cur);
    cur++;
  }
}

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
  ASSERT_EQ(mp.BucketSize(), 3);
  ASSERT_EQ(mp.Size(), 1);
  ASSERT_TRUE(mp.Insert(pair<int,int>(5, 1)));
  ASSERT_EQ(mp.Size(), 2);
  ASSERT_EQ(mp.BucketSize(), 3);
  ASSERT_TRUE(mp.Insert(pair<int,int>(3, 1)));
  ASSERT_EQ(mp.Size(), 3);
  ASSERT_EQ(mp.BucketSize(), 3);
  ASSERT_TRUE(mp.Insert(pair<int,int>(4, 1)));
  ASSERT_EQ(mp.Size(), 4);
  ASSERT_EQ(mp.BucketSize(), 7);

  mp.Rehash(1);
  ASSERT_EQ(mp.Size(), 4);
  ASSERT_EQ(mp.BucketSize(), 1);
}
