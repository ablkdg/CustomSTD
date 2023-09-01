//
// Created by ablkd on 1.09.2023.
//

#ifndef CUSTOM_HASHMAP_H
#define CUSTOM_HASHMAP_H

#include <cstdint>
#include <memory>

namespace custom{
    template<typename KeyType, typename DataType, int depth = 10>
    class hash_map{
    public:
        void insert(KeyType first, DataType second);

    public:
        hash_map(){
            for (auto& element:m_bucket) {
                element = new Node();
            }
        }
        struct Node{
            uint32_t key;
            KeyType first;
            DataType second;
            Node* next;

            Node(uint32_t k, KeyType f, DataType s, Node* n = nullptr) : key(k), first(f), second(s), next(n) {}
            Node() : key(0), next(nullptr){}
        };

        ~hash_map();
    private:
        Node *m_bucket[depth];
        std::hash<KeyType> m_Hasher;
    };
}

template<typename KeyType, typename DataType, int depth>
void custom::hash_map<KeyType, DataType, depth>::insert(KeyType first, DataType second) {
    uint32_t key = m_Hasher(first);
    int bucketIndex = key % depth;

    if(m_bucket[bucketIndex]->key == 0){
        m_bucket[bucketIndex] = new Node(key, first, second);
    } else {
        Node* holder = m_bucket[bucketIndex];
        m_bucket[bucketIndex]  = new Node(key, first, second, holder);
    }
}

template<typename KeyType, typename DataType, int depth>
custom::hash_map<KeyType, DataType, depth>::~hash_map() {
    {
        for (Node *node: m_bucket) {
            while (node) {
                Node *temp = node;
                node = node->next;
                delete temp;
            }
        }
    }
}

#endif //CUSTOM_HASHMAP_H