#include <iostream>
#include <cstring>
#include <algorithm>


class String{
public:
    //Конструкторы:
    //Конструктор по умолчанию
        String() = default;
    //Конструктор от C-style строки
        String(const char* input){
            size = strlen(input);
            while (max_size < size){
                max_size *= 2;
            }
            str = new char[max_size];
            memcpy(str, input, size);
        }
    // Конструктор создающий amount одинаковых символов
        String(size_t amount, char c) {
            while(max_size < amount) {
                max_size *= 2;
            }
            str = new char [max_size];
            size = amount;
            memset(str, c, amount);     //копирует младший байт c в следующие amount символов str
        }
    //Конструктор от символа
        String(char c){
            size = 1;
            max_size = 2;
            str = new char[1];
            str[0] = c;
        }
    //Конструктор копирования
        String(const String& cpy_string){
            str = new char [cpy_string.size];
            size = cpy_string.size;
            max_size = cpy_string.max_size;
            memcpy(str, cpy_string.str, cpy_string.size);
        }
    //Методы
    //Функция перестановки строк
        void swap(String& swap_string){
            std::swap(str, swap_string.str);
            std::swap(size, swap_string.size);
            std::swap(max_size, swap_string.max_size);
        }
    //Функция length() - возвращает длину строки
        [[nodiscard]] size_t length() const{
            return size;
        }
    //Метод pop_back() - удаляет последний элемент
        void pop_back(){
            size --;
            if (size <= max_size/4){
                max_size /= 2;
                char* new_str = new char [max_size];
                memcpy(new_str, str, size);
                delete[] str;
                str = new_str;
            }
        }
    //Метод push_back() - добавляет элемент в строку
        void push_back(const char c){
            size++;
            if (size >= max_size){
                max_size *= 2;
                char* new_str = new char [max_size];
                memcpy(new_str, str, size - 1);
                str = new_str;
            }
            str[size - 1] = c;
        }
    //Метод front() - возвращает ссылку на первый элемент
        [[nodiscard]] char& front(){
            return str[0];
        }
        [[nodiscard]] const char& front() const{
            return str[0];
        }
    //Метод back() - возвращает ссылку на последний элемент
        [[nodiscard]] char& back(){
            return str[size - 1];
        }
        [[nodiscard]] const char& back() const{
            return str[size - 1];
        }
    //Метод empty() - проверяет пустая ли строка
        [[nodiscard]] bool empty() const{
            return size <= 0;
        }
    //Метод clear() - очищает строку
        void clear(){
            delete[] str;
            str = nullptr;
            size = 0;
            max_size = 1;
        }
    //Метод find() - ищет самую левую подстроку
        size_t find(const String&  substring) const{
            size_t  result = 0;
            for (size_t i = 0; i <= size - substring.size; ++i) {
                result = trail(substring, i);
                if (result != size_t(-1)) return result;
            }
            return size;
        }
        size_t trail(const String& substring, size_t i) const {
            bool flag = true;
            for (size_t j = 0; j < substring.size; ++j) {
                if (str[i + j] != substring.str[j]){
                    flag = false;
                    break;
                }
            }
            if (flag){
                return i;
            }
            return -1;
        }
    //Метод rfind() - ищет самую правую подстроку
    size_t rfind(const String& substring) const {
        size_t result = 0;
        for (size_t i = size - substring.size + 1; i > 0 ; --i) {
            result = trail(substring, i);
            if (result != size_t(-1)) return result;
        }
        return size;
    }
    String substr(size_t start, size_t count) const{
        count = std::min(count, size - start);
        String sub_string;
        for (size_t i = start; i < count + start; ++i) {
            sub_string.push_back(str[i]);
        }
        return sub_string;
    }
    //Операторы
    //Оператор сравнения
        //строка == строка
            friend bool operator==(const String& string, const String& sec_string);
        //строка == "слово"
            friend bool operator==(const String& string, const char* word);
        //строка == 'символ'
            friend bool operator==(const String& string, char symbol);
        //'символ' == строка
            friend bool operator==(char symbol, const String& string);
        //"слово" == 'символ'
            friend bool operator==(const char* word, const String& string);

    //Оператор присваивания
    String& operator=(const String& cpy_string){
        if (str != cpy_string.str){
            String copy_string = cpy_string;
            swap(copy_string);
        }
        return *this;
    }
    String& operator=(const char input[]){
        for (size_t i = 0; i < strlen(input); ++i) {
            this->push_back(input[i]);
        }
        return *this;
    }
    //Оператор []
    char& operator[](const size_t index) {
        return str[index];
    }
    const char& operator[](const size_t index) const{
        return str[index];
    }
    //Оператор +=
    //для строки
        String& operator+=(const String& add_string){
            if (size + add_string.size >= max_size){
                max_size = std::max(max_size, add_string.max_size) * 2;
                char* new_str = new char[max_size];
                memcpy(new_str, str, size);
                memcpy(&new_str[size], add_string.str, add_string.size);
                size += add_string.size;
                str = new_str;
            }else{
                memcpy(&str[size], add_string.str, add_string.size);
                size += add_string.size;
            }
            return  *this;
        }
    //для символа
    String& operator+=(const char c){
        push_back(c);
        return *this;
    }
    //Оператор +
    //строка + строка
        friend String operator+(const String& first_string, const String& sec_string);
    //строка + символ
        friend String operator+(const String& first_string, char c);
    //символ + строка
        friend String operator+(char c, const String& string);
    //Операторы ввода/вывода
        friend std::ostream& operator<<(std::ostream& output, const String& string);
        friend std::istream& operator>>(std::istream& input, String& string);
    // Деструктор
        ~String(){
            delete[] str;
        }
private:
    char* str = nullptr;
    size_t size = 0;
    size_t max_size = 1;
};

//Оператор сравнения
//для строк
    bool operator==(const String& string, const String& sec_string){
            if (string.size == sec_string.size){
                for(size_t i = 0; i < string.size; ++i){
                    if (string.str[i] != sec_string.str[i]){
                        return false;
                    }
                }
                return true;
            }
        return false;
    }
//для строки и символов
    bool operator==(const String& string, const char* word){
        if (string.size == strlen(word)){
            for(size_t i = 0; i < string.size; ++i){
                if (string.str[i] != word[i]){
                    return false;
                }
            }
            return true;
        }
        return false;
    }
//для строки и символа
    bool operator==(const String& string, const char symbol){
        if (string.size == 1){
            if (string.str[0] == symbol) return true;
            return false;
        }
        return false;
}
//для символа и строки
    bool operator==(const char symbol, const String& string){
    if (string.size == 1){
        if (string.str[0] == symbol) return true;
        return false;
    }
    return false;
}
//для символов и строки
    bool operator==(const char* word, const String& string){
        if (string.size == strlen(word)){
            for(size_t i = 0; i < string.size; ++i){
                if (string.str[i] != word[i]){
                    return false;
                }
            }
            return true;
        }
        return false;
    }
//Оператор суммирования
//строка + строка
    String operator+(const String& first_string, const String& sec_string){
        String new_string(first_string);
        new_string += sec_string;
        return new_string;
    }
//строка + символ
    String operator+(const String& first_string, char c){
        String new_string(first_string);
        new_string.push_back(c);
        return new_string;
    }
//символ + строка
    String operator+(char c, const String& string){
        String new_string(1, c);
        new_string += string;
        return new_string;
    }
//Оператор вывода
std::ostream& operator<<(std::ostream& output, const String& string){
    output.tie(nullptr);
    for (size_t i = 0; i < string.size; ++i) {
        output << string.str[i];
    }
    return output;
}
//Оператор ввода
std::istream& operator>>(std::istream& input, String& string){
    input.tie(nullptr);
    string.clear();
    char symbol;
    do{
        input >> symbol;
        string.push_back(symbol);
    }while(input.peek() != 10 && input.peek() != 32 && input.peek() != -1);
    return input;
}
