#include <iostream>
#include <string>

class object {
protected:
    std::string contain;
public:
    object() = default;
    object(std::string _contain_) : contain(_contain_) {}

    virtual void copy(object* o) = 0;
    virtual std::string send_status(int y, int m, int d) = 0;
    virtual std::string type() = 0;

    virtual void print() {
        std::cout << "[object] contain: " << contain << std::endl;
    }

    virtual ~object() {}
};
