#pragma once

// ------------------------------------------------------------------
template<typename T>
class array {

public:
    size_t size() const {return size_;}

    virtual void add   (const T& item, size_t index) = 0;
    virtual T    remove(size_t index)                = 0;
    virtual ~array() {
        delete []arr_;
    }
private:
    virtual void resize(size_t size) = 0;

protected:
    T* arr_{ nullptr };
    size_t size_{ 0 };
};

// ------------------------------------------------------------------
template<typename T>
class single_array : public array<T> {
public:
    void add(const T& item, size_t index) override {
        if (index < size_) {
            
        } else {

        }
    }

    T remove(size_t index) override {
        if (index >= size_) return T();
        
        return T();
    }

private:
    void resize(size_t size) override {
        delete []arr_;
        arr_ = new T[size];
        size_ = size;
    }
};

// TODO
//class vector_array {};
//class factor_array {};
//class matrix_array {};