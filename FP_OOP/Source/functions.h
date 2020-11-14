#pragma once

#include <algorithm>

/*
====================
Functions
====================
*/
struct Functions {
    template<typename Collection, typename UnaryOperation>
    static void for_each(Collection& collection, UnaryOperation operation) {
        std::for_each(collection.begin(), collection.end(), operation);
    }
};
