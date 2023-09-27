#include "SynTaxTree.h"

Node::Node() : sibling(nullptr) {
    child[0] = child[1] = child[2] = nullptr;
    lineno = 0;
}

Node::~Node() {
    for (int i = 0; i < 3; ++i) {
        if (child[i] != nullptr)
            delete child[i];
    }
    if (sibling != nullptr)
        delete sibling;
}
