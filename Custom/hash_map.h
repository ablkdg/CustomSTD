//
// Created by ablkdg on 1.09.2023.
//

#ifndef CUSTOM_HASHMAP_H
#define CUSTOM_HASHMAP_H

#include <cstdint>
#include <memory>

namespace custom {
    template<typename KeyType, typename DataType, int depth = 16>
    class hash_map {
    public:
        struct pair {
            KeyType first;
            DataType second;

            pair(KeyType f, DataType s) : first(f), second(s) {}
        };

        void insert(KeyType first, DataType second);

        bool exist(KeyType &first);

        bool find(KeyType &first, DataType &return_destination);


        DataType &operator[](KeyType &&first);

        hash_map &operator=(const hash_map<KeyType, DataType, depth>& other);

    public:
        hash_map();

        ~hash_map();


    private:
        struct Node {
            uint32_t key;
            pair values;
            Node *next;

            Node(uint32_t k, KeyType f, DataType s, Node *n = nullptr) : key(k), values(f, s), next(n) {}

            Node() : key(0), next(nullptr) {}
        };

        Node *m_bucket[depth];
        std::hash<KeyType> m_Hasher;

        void m_Clear();

        void m_Init();

        // Inserts the value without caring about whether it exists or not.
        void m_force_insert(KeyType first, DataType second);


        // This will hold the last accessed element to fast find.
        Node *hold = nullptr;
    };
}

template<typename KeyType, typename DataType, int depth>
custom::hash_map<KeyType, DataType, depth> &custom::hash_map<KeyType, DataType, depth>::operator=(const hash_map<KeyType, DataType, depth> &other) {
    if (this != &other) {
        m_Clear();

        for (int i = 0; i < depth; ++i) {
            Node* node = other.m_bucket[i];
            while(node) {
                m_force_insert(node->values.first, node->values.second);
                node = node->next;
            }
        }
    }

    return *this;
}


template<typename KeyType, typename DataType, int depth>
DataType &custom::hash_map<KeyType, DataType, depth>::operator[](KeyType &&first) {
    if (hold->values.first == first) return hold->values.second;

    if (exist(first))
        return hold->values.second;
    else {
        m_force_insert(std::forward<KeyType>(first), DataType());
        return hold->values.second;
    }
}


template<typename KeyType, typename DataType, int depth>
custom::hash_map<KeyType, DataType, depth>::hash_map() {
    m_Init();
}


template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::insert(KeyType first, DataType second) {
    // Use std::move to properly pass lvalue and rvalue arguments.
    uint32_t key = m_Hasher(first);
    int bucketIndex = key % depth;

    if (!m_bucket[bucketIndex]) {
        m_bucket[bucketIndex] = new Node(key, std::move(first), std::move(second));
    } else {
        if (exist(first)) {
            hold->values.second = std::move(second);
            return;
        }

        Node *holder = m_bucket[bucketIndex];
        m_bucket[bucketIndex] = new Node(key, std::move(first), std::move(second), holder);
    }

    hold = m_bucket[bucketIndex];
}


template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::m_force_insert(KeyType first, DataType second) {
    // Use std::move to properly pass lvalue and rvalue arguments.
    uint32_t key = m_Hasher(first);
    int bucketIndex = key % depth;

    if (!m_bucket[bucketIndex]) {
        m_bucket[bucketIndex] = new Node(key, std::move(first), std::move(second));
    } else {
        Node *holder = m_bucket[bucketIndex];
        m_bucket[bucketIndex] = new Node(key, std::move(first), std::move(second), holder);
    }

    hold = m_bucket[bucketIndex];
}


template<typename KeyType, typename DataType, int depth>
custom::hash_map<KeyType, DataType, depth>::~hash_map() {
    m_Clear();
}


template<typename KeyType, typename DataType, int depth>
bool custom::hash_map<KeyType, DataType, depth>::find(KeyType &first, DataType &return_destination) {
    if (hold->values.first == first) return_destination = hold->values.second;

    if (!exist(first)) return false;
    return_destination = hold->values.second;
    return true;
}


template<typename KeyType, typename DataType, int depth>
bool custom::hash_map<KeyType, DataType, depth>::exist(KeyType &first) {
    uint32_t key = m_Hasher(first);
    int bucketIndex = key % depth;

    if (!m_bucket[bucketIndex]) return false;
    Node *currentNode = m_bucket[bucketIndex];

    while (currentNode) {
        // Okay, assume that hashes are the same but is the real value also the same? We need to check that to be sure about this is not a conflict.
        if (currentNode->key != key || currentNode->values.first != first)
            currentNode = currentNode->next;
        else {
            hold = currentNode;
            return true;
        }
    }
    return false;
}


template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::m_Clear() {
    for (Node *node: m_bucket) {
        while (node) {
            Node *temp = node;
            node = node->next;
            delete temp;
        }
    }
}


template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::m_Init() {
    for (int i = 0; i < depth; ++i) {
        m_bucket[i] = nullptr;
    }
}

#endif //CUSTOM_HASHMAP_H