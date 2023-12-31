//
// Created by ablkdg on 1.09.2023.
//

#ifndef CUSTOM_HASHMAP_H
#define CUSTOM_HASHMAP_H

#include <memory>

namespace custom {
    template<typename KeyType, typename DataType, int Depth = 16>
    class hash_map {
    public:
        struct pair {
            KeyType first;
            DataType second;

            pair(KeyType f, DataType s) : first(f), second(s) {}
        };

        void insert(const KeyType& first, const DataType& second);

        bool exist(const KeyType &first);

        bool find(KeyType &first, DataType &return_destination);


        DataType &operator[](KeyType &&first);
        DataType &operator[](const KeyType &first);

        hash_map &operator=(const hash_map<KeyType, DataType, Depth>& other);

    public:
        hash_map();

        ~hash_map();


    private:
        struct Node {
            size_t key;
            pair values;
            Node *next;

            Node(size_t k, KeyType f, DataType s, Node *n = nullptr) : key(k), values(f, s), next(n) {}

            Node() : key(0), next(nullptr) {}
        };

        Node *m_bucket[Depth];
        std::hash<KeyType> m_Hasher;

        void m_Clear();

        void m_Init();

        // Inserts the value without caring about whether it exists or not.
        void m_force_insert(const KeyType &first, const DataType &second);


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
DataType &custom::hash_map<KeyType, DataType, depth>::operator[](const KeyType &first) {
    if (hold->values.first == first) return hold->values.second;

    if (exist(first))
        return hold->values.second;
    else {
        m_force_insert(first, DataType());
        return hold->values.second;
    }
}


template<typename KeyType, typename DataType, int depth>
custom::hash_map<KeyType, DataType, depth>::hash_map() {
    m_Init();
}


template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::insert(const KeyType& first, const DataType& second) {
    // Use std::move to properly pass lvalue and rvalue arguments.
    size_t key = m_Hasher(first);
    int bucket_index = key % depth;

    if (!m_bucket[bucket_index]) {
        m_bucket[bucket_index] = new Node(key, first, second);
    } else {
        if (exist(first)) {
            hold->values.second = second;
            return;
        }

        Node *holder = m_bucket[bucket_index];
        m_bucket[bucket_index] = new Node(key, first, second, holder);
    }

    hold = m_bucket[bucket_index];
}


template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::m_force_insert(const KeyType& first, const DataType& second) {
    // Use std::move to properly pass lvalue and rvalue arguments.
    size_t key = m_Hasher(first);
    int bucket_index = key % depth;

    if (!m_bucket[bucket_index]) {
        m_bucket[bucket_index] = new Node(key, first, second);
    } else {
        Node *holder = m_bucket[bucket_index];
        m_bucket[bucket_index] = new Node(key, first, second, holder);
    }

    hold = m_bucket[bucket_index];
}


template<typename KeyType, typename DataType, int depth>
custom::hash_map<KeyType, DataType, depth>::hash_map::~hash_map() {
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
bool custom::hash_map<KeyType, DataType, depth>::exist(const KeyType &first) {
    size_t key = m_Hasher(first);
    int bucket_index = key % depth;

    if (!m_bucket[bucket_index]) return false;
    Node *currentNode = m_bucket[bucket_index];

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