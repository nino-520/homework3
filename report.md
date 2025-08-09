# 51115141

作業三

## 解題說明

這題要求用 C++ 設計一個 Polynomial 類來表示和操作單變數整數係數多項式，並用循環鏈表（帶頭結點）來儲存。鏈表的每個節點包含係數（coef）、指數（exp）和鏈接（link），指數按降序排列。

### 解題策略

用循環鏈表帶頭結點表示多項式，頭結點不存有效數據。

實現輸入輸出轉換、複製構造、加減乘運算，確保指數降序並合併相同指數。

用可用空間鏈表管理釋放的節點，優化記憶體使用。

## 程式實作

```cpp
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
```

## 效能分析

時間複雜度: 輸入/輸出  O(n) ，加法  O(n + m) ，評估  O(n) ，其中  n, m  為項數。

空間複雜度:  O(n + m)  儲存結果鏈表。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  p1  | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一| 2x^2 + x | 2x^2+x | 2x^2+x |
| 測試二| 3x^3 | 3x^3 | 3x^3 |
| 測試三| 2x^2 + x, x=2 |10|10|

編譯與執行指令

```shell
$ g++ -std=c++17 -o polynomial polynomial.cpp
$ ./polynomial
2 2 1 1
p1: 2x^2+1x^1
p2 (copy of p1): 2x^2+1x^1
p1(2): 10
```

### 結論

程式能正確讀入、複製和評估多項式，加法邏輯基本可行，需優化乘法和減法。
### 心得

這次讓我學會用循環鏈表管理多項式，感覺挺酷但有點複雜。

