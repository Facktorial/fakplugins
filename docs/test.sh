echo '#include <chrono>
int main() { return 0; }' >test_cpp20.cpp
clang-tidy-15 --checks='clang-diagnostic-*' test_cpp20.cpp -- -std=c++20
rm test_cpp20.cpp
