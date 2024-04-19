#include <iostream>
#include <vector>
#include <functional>

using namespace std;

// реализация хэш-таблицы с разрешением коллизий методом цепочек

template <class T>
struct HashTableNode{
    T data;
    HashTableNode* next;
    HashTableNode();
    HashTableNode(T, HashTableNode*);
    void get_info();
};

template <class T>
HashTableNode<T>::HashTableNode(): next(nullptr){}

template <class T>
HashTableNode<T>::HashTableNode(T data_, HashTableNode * next_): data(data_), next(next_){}

template <class T>
void HashTableNode<T>::get_info(){
    cout << "data: " << data << endl;
}


template <class T, class Hasher = hash<T>>
class HashTable{
    public:
    void get_info();
    HashTable(int initial_buffersize=2);
    ~HashTable();
    bool Add(const T& key);
    bool Delete(const T& key);
    bool Has(const T& key);

    private:
    void grow();
    vector<HashTableNode<T>*>buffer;
    Hasher hasher;
    size_t num_elems;
};

template <class T, class Hasher>
void HashTable<T, Hasher>::get_info(){
    cout << "buffer size: " << buffer.size() << endl;
    cout << "num elems: " << num_elems << endl;
}

template <class T, class Hasher>
HashTable<T, Hasher>::HashTable(int initial_buffer_size): buffer(initial_buffer_size, nullptr), num_elems(0), hasher(Hasher()){}

template <class T, class Hasher>
bool HashTable<T, Hasher>::Has(const T& key){
    size_t hash = hasher(key) % buffer.size();

    HashTableNode<T>* head = buffer[hash];

    while (head != nullptr){
        if (head->data == key){
            return true;
        }
        head = head->next;
    }
    return false;
}

template <class T, class Hasher>
bool HashTable<T, Hasher>::Add(const T& key){

    if (buffer.size() * 2 <= num_elems){
        // cout << "hash table growing !" << endl;
        grow();
    }

    size_t hash = hasher(key) % buffer.size();

    HashTableNode<T>* head = buffer[hash];

    while (head != nullptr){
        if (head->data == key){
            return false;
        }
        head = head->next;
    }
    HashTableNode<T>* new_node = new HashTableNode<T>(key, buffer[hash]);
    buffer[hash] = new_node;
    num_elems++;
    return true;
}

template <class T, class Hasher>
bool HashTable<T, Hasher>::Delete(const T& key){

    size_t hash = hasher(key) % buffer.size();

    HashTableNode<T>* prev = nullptr;
    HashTableNode<T>* head = buffer[hash];

    while (head != nullptr){
        if (head->data == key){
            break;
        }
        prev = head;
        head = head->next;
    }

    if (head == nullptr){
        return false;
    }

    if (prev == nullptr){
        buffer[hash] = head -> next;
        delete head;
    }else{
        prev -> next = head -> next;
        delete head;
    }
    num_elems--;
    return true;
}

template <class T, class Hasher>
HashTable<T, Hasher>::~HashTable(){
    for (int i = 0 ; i < buffer.size(); i++){
        HashTableNode<T>* head = buffer[i];
        HashTableNode<T>* next = nullptr;
        while (head != nullptr){
            next = head -> next;
            delete head;
            head = next;
        }
    }
}

template <class T, class Hasher>
void HashTable<T, Hasher>::grow(){
    vector<HashTableNode<T>*> new_buffer(buffer.size() * 2, nullptr);
    for (int i = 0; i < buffer.size(); i++){
        HashTableNode<T>* head = buffer[i];
        HashTableNode<T>* next = nullptr;
        while (head != nullptr){
            next = head -> next;
            size_t new_hash = hasher(head -> data) % (new_buffer.size());
            head -> next = new_buffer[new_hash];
            new_buffer[new_hash] = head;
            head = next;
        }
    }
    buffer = new_buffer;
}


int main(){
    HashTable<string> hash_table;
    char op = '+';
    string key = "";
    while (cin >> op >> key){
        switch (op){
            case '+':
                cout << (hash_table.Add(key) == true ? "OK" : "FAIL") << endl;
                // hash_table.get_info();
                break;
            case '-':
                cout << (hash_table.Delete(key) == true ? "OK" : "FAIL") << endl;
                // hash_table.get_info();
                break;
            case '?':
                cout << (hash_table.Has(key) == true ? "OK" : "FAIL") << endl;
                // hash_table.get_info();
                break;
        }
    }
    return 0;
}