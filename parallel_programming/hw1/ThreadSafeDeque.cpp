#include <iostream>
using namespace std;

template<typename T>
class TQueue {
 void push(T item) {
  // Add imple here.
   {
     std::unique_lock<std::mutex> lock(m_);
     ++wwc_;
     cv_.wait(lock, [&]{return rc_ == 0 && wc_ == 0; });
     --wwc_;
     ++wc_;
   }
   q_.push(item);
   {
     std::unique_lock<std::mutex> lock(m_);
     --wc_;
   }
   cv_.notify_one();
 }
 T front() {
   {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [&]{return wc_ == 0 && wwc_ == 0; });
    rc_++;
   }
   auto ret = q_.front();
   {
    std::unique_lock<std::mutex> lock(m_);
    --rc_;
   }
   cv_.notify_one();
   return ret;
 }
 private:
  queue<T> q_;
  std::mutex m_;
  std::condition_variable cv_;
  int rc_ = 0;
  int wc_ = 0;
  int wwc_ = 0;
};

// To execute C++, please define "int main()"
int main() {
  auto words = { "Hello, ", "World!", "\n" };
  for (const string& word : words) {
    cout << word;
  }
  return 0;
}