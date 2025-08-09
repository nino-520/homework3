#include <iostream>
using namespace std;

class Polynomial;

class Node {
    friend Polynomial;
private:
    int coef; 
    int exp;  
    Node* link;
public:
    Node(int c = 0, int e = 0, Node* l = nullptr) : coef(c), exp(e), link(l) {}
};

class Polynomial {
private:
    Node* header;      
    static Node* freeList; 
public:
    Polynomial() { 
        header = new Node();
        header->link = header;
        freeList = nullptr;
    }

    void input() {
        int n, c, e;
        cin >> n;
        Node* tail = header;
        for (int i = 0; i < n; i++) {
            cin >> c >> e;
            Node* newNode = new Node(c, e);
            tail->link = newNode;
            tail = newNode;
        }
        tail->link = header;
    }

    void output(ostream& os) {
        Node* p = header->link;
        bool first = true;
        while (p != header) {
            if (!first && p->coef >= 0) os << "+";
            os << p->coef << "x^" << p->exp;
            first = false;
            p = p->link;
        }
        os << endl;
    }

    Polynomial(const Polynomial& a) {
        header = new Node();
        header->link = header;
        Node* src = a.header->link;
        Node* tail = header;
        while (src != a.header) {
            Node* newNode = new Node(src->coef, src->exp);
            tail->link = newNode;
            tail = newNode;
            src = src->link;
        }
        tail->link = header;
    }

    ~Polynomial() {
        Node* p = header->link;
        while (p != header) {
            Node* temp = p;
            p = p->link;
            temp->link = freeList;
            freeList = temp;
        }
        delete header;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* aPtr = header->link;
        Node* bPtr = b.header->link;
        Node* tail = result.header;
        while (aPtr != header || bPtr != b.header) {
            Node* newNode;
            if (aPtr == header) {
                newNode = new Node(bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            } else if (bPtr == b.header) {
                newNode = new Node(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            } else if (aPtr->exp > bPtr->exp) {
                newNode = new Node(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            } else if (aPtr->exp < bPtr->exp) {
                newNode = new Node(bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            } else {
                int sum = aPtr->coef + bPtr->coef;
                if (sum != 0) newNode = new Node(sum, aPtr->exp);
                else newNode = nullptr;
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
            if (newNode) {
                tail->link = newNode;
                tail = newNode;
            }
        }
        tail->link = result.header;
        return result;
    }

    float operator()(float x) const {
        float result = 0;
        Node* p = header->link;
        while (p != header) {
            result += p->coef * pow(x, p->exp);
            p = p->link;
        }
        return result;
    }
};

Node* Polynomial::freeList = nullptr;

int main() {
    Polynomial p1;
    p1.input(); 
    cout << "p1: ";
    p1.output(cout);
    Polynomial p2(p1);
    cout << "p2 (copy of p1): ";
    p2.output(cout);
    cout << "p1(2): " << p1(2) << endl;
    return 0;
}
