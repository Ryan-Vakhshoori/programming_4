#include <gtest/gtest.h>

#include <functional>

#include "pqueue.h"

TEST(PQueue, less) {
  PQueue<int> pq;
  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
}

TEST(PQueue, great) {
  PQueue<int, std::greater<int> > pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
}

// test that uses less_equal (<=) comparator
TEST(PQueue, less_equal) {
  PQueue<int, std::less_equal<int> > pq;

  pq.Push(55);
  pq.Push(67);
  pq.Push(55);
  pq.Push(3);
  pq.Push(34);
  pq.Push(55);
  pq.Pop();

  EXPECT_EQ(pq.Top(), 34);
  EXPECT_EQ(pq.Size(), 5);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 55);
}
// test that uses greater_equal (>=) comparator
TEST(PQueue, greater_equal) {
  PQueue<int, std::greater_equal<int> > pq;

  pq.Push(55);
  pq.Push(67);
  pq.Push(55);
  pq.Push(3);
  pq.Push(34);
  pq.Push(55);
  pq.Pop();
  pq.Pop();
  pq.Push(10);
  pq.Push(97);

  EXPECT_EQ(pq.Top(), 97);
  EXPECT_EQ(pq.Size(), 6);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 55);
}

class MyClass {
 public:
  explicit MyClass(int n) : n_(n) {}
  bool operator<(const MyClass &mc) const { return n_ < mc.n_; }
  int n() { return n_; }

 private:
  int n_;
};

TEST(PQueue, custom_class) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

  PQueue<MyClass> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[2].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[1].n());
}

// class that compares MyClass objects with greater operator
class MyClassGreater {
 public:
  explicit MyClassGreater(int n) : n_(n) {}
  bool operator<(const MyClassGreater &mc) const { return mc.n_ > n_; }
  int n() { return n_; }

 private:
  int n_;
};

// test that uses MyClassGreater comparator
TEST(PQueue, custom_class_for_max_heap) {
  std::vector<MyClassGreater> vec{MyClassGreater(42), MyClassGreater(23),
                                  MyClassGreater(2),  MyClassGreater(34),
                                  MyClassGreater(15), MyClassGreater(1)};

  PQueue<MyClassGreater> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);
  pq.Push(vec[4]);
  pq.Push(vec[5]);

  EXPECT_EQ(pq.Top().n(), vec[5].n());
  EXPECT_EQ(pq.Size(), 6);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[2].n());
}

/*
TEST(PQueue, custom_class_pointer) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34)};

  PQueue<MyClass*, /> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);
}
*/
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
