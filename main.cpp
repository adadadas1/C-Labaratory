#include <iostream>
#include <vector>
#include <memory>

class SequentialContainer {
    std::vector<int> data;

public:
    void push_back(int value) {
        data.push_back(value);
    }

    void insert(size_t index, int value) {
        if (index < data.size()) {
            data.insert(data.begin() + index, value);
        } else {
            data.push_back(value);
        }
    }

    void erase(size_t index) {
        if (index < data.size()) {
            data.erase(data.begin() + index);
        }
    }

    size_t size() const {
        return data.size();
    }

    int operator[](size_t index) const {
        return data[index];
    }

    void print() const {
        for (int val : data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

class ListContainer {
    struct Node {
        int value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        Node(int val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    size_t count;

public:
    ListContainer() : count(0) {}

    void push_back(int value) {
        auto newNode = std::make_shared<Node>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++count;
    }

    void insert(size_t index, int value) {
        auto newNode = std::make_shared<Node>(value);
        if (index == 0) {
            newNode->next = head;
            if (head) head->prev = newNode;
            head = newNode;
            if (!tail) tail = head;
        } else {
            auto current = head;
            for (size_t i = 0; i < index - 1 && current; ++i) {
                current = current->next;
            }
            if (current) {
                newNode->next = current->next;
                newNode->prev = current;
                if (current->next) current->next->prev = newNode;
                current->next = newNode;
                if (!newNode->next) tail = newNode;
            }
        }
        ++count;
    }

    void erase(size_t index) {
        if (index < count) {
            auto current = head;
            for (size_t i = 0; i < index && current; ++i) {
                current = current->next;
            }
            if (current) {
                auto prev = current->prev.lock();
                auto next = current->next;
                if (prev) prev->next = next;
                if (next) next->prev = prev;
                if (current == head) head = next;
                if (current == tail) tail = prev;
                --count;
            }
        }
    }

    size_t size() const {
        return count;
    }

    int operator[](size_t index) const {
        auto current = head;
        for (size_t i = 0; i < index && current; ++i) {
            current = current->next;
        }
        return current ? current->value : -1;
    }

    void print() const {
        auto current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

// Демонстрация использования
int main() {
    SequentialContainer seqContainer;
    ListContainer listContainer;

    for (int i = 0; i < 10; ++i) {
        seqContainer.push_back(i);
        listContainer.push_back(i);
    }

    std::cout << "Последовательный контейнер после добавления элементов: ";
    seqContainer.print();
    std::cout << "Размер: " << seqContainer.size() << std::endl;

    std::cout << "Списковый контейнер после добавления элементов: ";
    listContainer.print();
    std::cout << "Размер: " << listContainer.size() << std::endl;

    seqContainer.erase(2);
    seqContainer.erase(3);
    seqContainer.erase(4);

    listContainer.erase(2);
    listContainer.erase(3);
    listContainer.erase(4);

    std::cout << "Последовательный контейнер после удаления элементов: ";
    seqContainer.print();

    std::cout << "Списковый контейнер после удаления элементов: ";
    listContainer.print();

    seqContainer.insert(0, 10);
    listContainer.insert(0, 10);

    std::cout << "Последовательный контейнер после вставки в начало: ";
    seqContainer.print();

    std::cout << "Списковый контейнер после вставки в начало: ";
    listContainer.print();

    seqContainer.insert(4, 20);
    listContainer.insert(4, 20);

    std::cout << "Последовательный контейнер после вставки в середину: ";
    seqContainer.print();

    std::cout << "Списковый контейнер после вставки в середину: ";
    listContainer.print();

    seqContainer.push_back(30);
    listContainer.push_back(30);

    std::cout << "Последовательный контейнер после добавления в конец: ";
    seqContainer.print();

    std::cout << "Списковый контейнер после добавления в конец: ";
    listContainer.print();

    return 0;
}
