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

    // Перемещающий конструктор
    PreallocatedContainer(PreallocatedContainer&& other) noexcept
        : data(other.data), capacity(other.capacity), count(other.count) {
        other.data = nullptr;
        other.capacity = 0;
        other.count = 0;
    }

    // Перемещающий оператор присваивания
    PreallocatedContainer& operator=(PreallocatedContainer&& other) noexcept {
        if (this != &other) {
            delete[] data; // Освобождаем старые ресурсы
            data = other.data;
            capacity = other.capacity;
            count = other.count;

            other.data = nullptr;
            other.capacity = 0;
            other.count = 0;
        }
        return *this;
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

// Двунаправленный список
class DoublyLinkedList {
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
    DoublyLinkedList() : count(0) {}

    // Перемещающий конструктор
    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head(std::move(other.head)), tail(std::move(other.tail)), count(other.count) {
        other.count = 0;
    }

    // Перемещающий оператор присваивания
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            head = std::move(other.head);
            tail = std::move(other.tail);
            count = other.count;

            other.count = 0;
        }
        return *this;
    }

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
        if (index > count) {
            throw std::out_of_range("Индекс выходит за пределы списка.");
        }
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
            newNode->next = current->next;
            newNode->prev = current;
            if (current->next) current->next->prev = newNode;
            current->next = newNode;
            if (!newNode->next) tail = newNode;
        }
        ++count;
    }

    void erase(size_t index) {
        if (index >= count) {
            throw std::out_of_range("Индекс выходит за пределы списка.");
        }
        auto current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        auto prev = current->prev.lock();
        auto next = current->next;

        if (prev) prev->next = next;
        if (next) next->prev = prev;
        if (current == head) head = next;
        if (current == tail) tail = prev;

        --count;
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

// Однонаправленный список
class SinglyLinkedList {
    struct Node {
        int value;
        std::shared_ptr<Node> next;
        Node(int val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    size_t count;

public:
    SinglyLinkedList() : count(0) {}

    // Перемещающий конструктор
    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head(std::move(other.head)), count(other.count) {
        other.count = 0;
    }

    // Перемещающий оператор присваивания
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            head = std::move(other.head);
            count = other.count;
            other.count = 0;
        }
        return *this;
    }

    void push_back(int value) {
        auto newNode = std::make_shared<Node>(value);
        if (!head) {
            head = newNode;
        } else {
            auto current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++count;
    }

    void insert(size_t index, int value) {
        if (index > count) {
            throw std::out_of_range("Индекс выходит за пределы списка.");
        }
        auto newNode = std::make_shared<Node>(value);
        if (index == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            auto current = head;
            for (size_t i = 0; i < index - 1 && current; ++i) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        ++count;
    }

    void erase(size_t index) {
        if (index >= count) {
            throw std::out_of_range("Индекс выходит за пределы списка.");
        }
        if (index == 0) {
            head = head->next;
        } else {
            auto current = head;
            for (size_t i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            current->next = current->next->next;
        }
        --count;
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

// Демонстрация использования всех контейнеров
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

    // Двунаправленный список
    DoublyLinkedList doublyLinkedList;

    for (int i = 0; i < 10; ++i) {
        doublyLinkedList.push_back(i);
    }

    std::cout << "Двунаправленный список после добавления элементов: ";
    doublyLinkedList.print();
    std::cout << "Размер: " << doublyLinkedList.size() << std::endl;

    doublyLinkedList.erase(2);
    doublyLinkedList.erase(3);
    
    std::cout << "Двунаправленный список после удаления элементов: ";
    doublyLinkedList.print();

    doublyLinkedList.insert(0, 10);
    std::cout << "Двунаправленный список после вставки в начало: ";
    doublyLinkedList.print();

    doublyLinkedList.insert(4, 20);
    std::cout << "Двунаправленный список после вставки в середину: ";
    doublyLinkedList.print();

    doublyLinkedList.push_back(30);
    std::cout << "Двунаправленный список после добавления в конец: ";
    doublyLinkedList.print();

    // Однонаправленный список
    SinglyLinkedList singlyLinkedList;

    for (int i = 0; i < 10; ++i) {
        singlyLinkedList.push_back(i);
    }

    std::cout << "Однонаправленный список после добавления элементов: ";
    singlyLinkedList.print();
    std::cout << "Размер: " << singlyLinkedList.size() << std::endl;

    singlyLinkedList.erase(2);
    singlyLinkedList.erase(3);
    
    std::cout << "Однонаправленный список после удаления элементов: ";
    singlyLinkedList.print();

    singlyLinkedList.insert(0, 10);
    std::cout << "Однонаправленный список после вставки в начало: ";
    singlyLinkedList.print();

    singlyLinkedList.insert(4, 20);
    std::cout << "Однонаправленный список после вставки в середину: ";
    singlyLinkedList.print();

    singlyLinkedList.push_back(30);
    std::cout << "Однонаправленный список после добавления в конец: ";
    singlyLinkedList.print();

    return 0;
}
