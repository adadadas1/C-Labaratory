#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

// Последовательный контейнер с резервированием памяти
class PreallocatedContainer {
private:
    int* data;             // Указатель на массив данных
    size_t capacity;       // Текущая емкость контейнера
    size_t count;          // Текущее количество элементов

    void resize() {
        // Увеличиваем размер массива на 50%
        capacity = static_cast<size_t>(capacity * 1.5);
        int* newData = new int[capacity];
        for (size_t i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    PreallocatedContainer(size_t initial_capacity = 10)
        : capacity(initial_capacity), count(0) {
        data = new int[capacity];
    }

    ~PreallocatedContainer() {
        delete[] data;
    }

    void push_back(int value) {
        if (count >= capacity) {
            resize();
        }
        data[count++] = value;
    }

    void insert(size_t index, int value) {
        if (index > count) {
            throw std::out_of_range("Индекс выходит за пределы контейнера.");
        }
        if (count >= capacity) {
            resize();
        }
        for (size_t i = count; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++count;
    }

    void erase(size_t index) {
        if (index >= count) {
            throw std::out_of_range("Индекс выходит за пределы контейнера.");
        }
        for (size_t i = index; i < count - 1; ++i) {
            data[i] = data[i + 1];
        }
        --count;
    }

    size_t size() const {
        return count;
    }

    int operator[](size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Индекс выходит за пределы контейнера.");
        }
        return data[index];
    }

    void print() const {
        for (size_t i = 0; i < count; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

// Контейнер спискового типа
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

// Демонстрация использования обоих контейнеров
int main() {
    // Последовательный контейнер
    PreallocatedContainer preallocatedContainer;

    for (int i = 0; i < 10; ++i) {
        preallocatedContainer.push_back(i);
    }

    std::cout << "Последовательный контейнер после добавления элементов: ";
    preallocatedContainer.print();
    std::cout << "Размер: " << preallocatedContainer.size() << std::endl;

    preallocatedContainer.erase(2);
    preallocatedContainer.erase(3);
    
    std::cout << "Последовательный контейнер после удаления элементов: ";
    preallocatedContainer.print();

    preallocatedContainer.insert(0, 10);
    std::cout << "Последовательный контейнер после вставки в начало: ";
    preallocatedContainer.print();

    preallocatedContainer.insert(4, 20);
    std::cout << "Последовательный контейнер после вставки в середину: ";
    preallocatedContainer.print();

    preallocatedContainer.push_back(30);
    std::cout << "Последовательный контейнер после добавления в конец: ";
    preallocatedContainer.print();

    // Списковый контейнер
    ListContainer listContainer;

    for (int i = 0; i < 10; ++i) {
        listContainer.push_back(i);
    }

    std::cout << "Списковый контейнер после добавления элементов: ";
    listContainer.print();
    std::cout << "Размер: " << listContainer.size() << std::endl;

    listContainer.erase(2);
    listContainer.erase(3);
    
    std::cout << "Списковый контейнер после удаления элементов: ";
    listContainer.print();

    listContainer.insert(0, 10);
    std::cout << "Списковый контейнер после вставки в начало: ";
    listContainer.print();

    listContainer.insert(4, 20);
    std::cout << "Списковый контейнер после вставки в середину: ";
    listContainer.print();

    listContainer.push_back(30);
    std::cout << "Списковый контейнер после добавления в конец: ";
    listContainer.print();

    return 0;
}
