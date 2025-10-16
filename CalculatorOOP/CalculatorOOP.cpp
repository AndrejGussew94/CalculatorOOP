
#include <iostream>
#include <cmath>

class Char_Vector {
private:
    char* data = nullptr;
    int size = 0;

public:

    Char_Vector() : data(), size() { std::cout << "Default constructor is called!" << "\n"; }
    Char_Vector(int size) : size(size) { data = new char[size]; }
    Char_Vector(char* data) : data(data) {
        int lenght = 0;
        while (data[lenght] != '\0') lenght++;
        this->data = new char[lenght + 1];
        for (int a = 0; a <= lenght; a++) {
            this->data[a] = data[a];
        }
        this->size = lenght;
    }
    ~Char_Vector() { delete[] data; data = nullptr;  size = 0; }

    void set_size(int size) {
        this->size = size;
    }
    
    int getlen() {
        int index = 0;
        while (data[index] != '\0') {
            index++;
        }
        return index;
    }

    void push_back(char value) { //actual
        size++;
        char* new_data = new char[size];
        for (int b = 0; b < (size)-1; b++) {
            new_data[b] = data[b];
        }
        new_data[size - 1] = '\0';
        new_data[size - 2] = value;
        delete[] data;
        data = new_data;
    }

    void delete_first() { // actual
        char* new_data = new char[size - 1]; // might be a problem
        for (int a = 0; a < size; a++) {
            new_data[a] = data[a + 1];
        }
        new_data[size - 1] = '\0';
        delete[] data;
        data = new_data;// size!!!static //std::vector
    }

    void add_first(char value) { //actual
        char* new_data = new char[size + 1];
        new_data[size] = '\0'; // possible fuck-up
        for (int a = size; a > 0; a--) {
            new_data[a] = data[a - 1];
        }
        new_data[0] = value;
        delete[] data;
        data = new_data;
    }

    void reverse() { // actual
        char* new_data = new char[size];
        new_data[size - 1] = '\0';
        for (int b = 0; b < size - 1; b++) {
            new_data[b] = data[size - 2 - b]; // working only this way
        }
        delete[] data;
        data = new_data;
    }

    void to_upper_case() { // actual
        for (int b = 0; b < size; b++) {
            if (data[b] >= 97 && data[b] <= 122) {
                data[b] = data[b] - 32;
            }
        }
    }

    bool is_valid(int base_system) { // change+ actual
        for (int a = 0; a < size; a++) {
            if (data[a] >= 48 && data[a] <= 57) {
                if ((data[a] - 48) >= base_system) return false;
            }
            if (data[a] >= 65 && data[a] <= 90) {
                if ((data[a] - 55) >= base_system) return false;
            }
        }
        return true;
    }

    bool is_zero() { // actual
        for (int a = 0; a < size - 1; a++) {
            if ((data[a] - 48) != 0) return false;
        }
        return true;
    }

    bool is_negative() {
        if (data[0] != 45) return false;
        return true;
    }

    char& operator[](int index) { 
        if (this == nullptr) throw std::invalid_argument("It doesn't work this way!\n");
        if (index > size || index < 0) {
            std::cout << "Array index out of bound, exiting";
            exit(0);
        }
        return data[index];
    }

    void get_char_vector_from_char(char* data) {
        int lenght = 0;
        while (data[lenght] != '\0') lenght++;
        this->data = new char[lenght + 1];
        for (int a = 0; a <= lenght; a++) {
            this->data[a] = data[a];
        }
        this->size = lenght;
    };

    char* print() {
        return data;
        //for (int a = 0; a < size; a++) {
        //    std::cout << this->data[a];
        //}
    }
    friend std::ostream& operator<< (std::ostream& os, Char_Vector& vector);
};

std::ostream& operator<< (std::ostream& os, Char_Vector& vector) {
    os << vector.print();
    return os;
}


class Number {
private:
    Char_Vector* data = nullptr;
    int base_system;

public:

    Number() : data (), base_system(0) {}
    Number(Char_Vector* data, int base_system) : data(data), base_system(base_system) {}
    ~Number() {}

    Char_Vector* get_data() {
        return data;
    }
    void set_data(Char_Vector* data) {
        this->data = data;
    }

    int get_base_system() {
        return base_system;
    }

    void set_base_system() {
        this->base_system = base_system;
    }

    Char_Vector* converting(int final_system) { // change
        data->to_upper_case();
        if (!(data->is_valid(base_system))) {
            throw std::invalid_argument("This operation cannot be executed cause digit cannot be more or equal than base system.\n");
        }
        if (base_system > 36 || base_system < 1 && final_system > 36 || final_system < 1) {
            throw std::invalid_argument("Base and final system must be between 2 and 36.\n");
        }
        if (base_system > 36 || base_system < 1) {
            throw std::invalid_argument("Base system must be between 2 and 36.\n");
        }
        if (data->is_zero()) {
            throw std::invalid_argument("0");
        }
        if (final_system > 36 || final_system < 1) {
            throw std::invalid_argument("Final system must be between 2 and 36.DECIMAL.\n");
        }
        bool negative_status = false;
        if (data->is_negative()) {
            data->delete_first();
            negative_status = true;
        }
        long long result = 0; //место уязвимости to decimal
        int n = 0;
        int size = data->getlen();
        for (int b = size - 1; b >= 0; b--) {
            if ((*data)[b] >= 48 && (*data)[b] <= 57) {
                result = result + ((*data)[b] - 48) * pow(base_system, n);
                n++;
            }
            if ((*data)[b] >= 65 && (*data)[b] <= 90) {
                result = result + ((*data)[b] - 55) * pow(base_system, n);
                n++;
            }
        } // right result at this moment
        int new_size = 1;
        Char_Vector* end_result = new Char_Vector(new_size);
        long long value;
        while (result > 0) {
            value = result % final_system;
            if (value < 10) {
                value = value + 48;
            }
            else if (value >= 10) {
                value = value + 55;
            }
            end_result->push_back(value);
            result = result / final_system;
        }
        end_result->reverse();
        if (negative_status) end_result->add_first('-');
        data = end_result;
        base_system = final_system;
        /*end_result->print();*/
        return end_result; // right result at this moment

    }

    void reset_number() {
        delete this->data;
        base_system = 0;
        data->set_size(0);
    }

    friend std::istream& operator>> (std::istream& is, Number& number);
    friend std::ostream& operator<< (std::ostream& os, Number& number);
    
};

std::istream& operator>> (std::istream& is, Number& number) {
    char buffer[256];
    std::cout << "Enter the number: ";
    is >> buffer;
    if (number.data != nullptr) delete number.data;
    number.data = new Char_Vector(buffer);
    if (buffer[0] == '!') {
        return is;
    }
    std::cout << "Select the base number system: ";
    is >> number.base_system;
    return is;
}

std::ostream& operator<< (std::ostream& os, Number& number) {
    os << number.data->print();
    return os;
}

int main()
{
    std::cout << "Number systems" << "\n\n";
    std::cout << "Translation of numbers" << "\n\n";
    std::cout << "If you want to stop working input '!'" << "\n\n";
    Number number;
    int final_system;
    while (true) {
        try {
            std::cin >> number;
            if ((*number.get_data())[0] == '!') break;
            std::cout << "Select the final number system: ";
            std::cin >> final_system;
            std::cout << *number.converting(final_system) << "\n\n";
            /*number.reset_number();*/
        }
        catch (std::invalid_argument e) {
          std::cout << e.what() << "\n\n";
        }
       
    } 
}
    



