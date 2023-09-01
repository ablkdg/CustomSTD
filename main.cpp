#include <iostream>
#include "Custom/hash_map.h"
int main() {
    custom::hash_map<std::string, int, 1> test{};
    test.insert("as",2);
    test.insert("df",3);

    return 0;
}
