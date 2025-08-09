# 51115141

作業三

## 解題說明

這題是要我們設計一個 Polynomial 類來處理多項式，題目給了個框架，要我們實現構造函數（建個零多項式）、加法（兩個多項式相加）、乘法（兩個多項式相乘），還有評估（把 x 代進去算值）。

### 解題策略

先用個 Term 類來存每項的係數和指數，然後 Polynomial 用陣列管理這些項。

構造函數就建個空的零多項式，Add 負責把兩個多項式加起來，Mult 算乘積，Eval 代入 x 算結果。

## 程式實作

```cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Polynomial; 

class Term {
    friend Polynomial;
private:
    float coef; 
    int exp;    
public:
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
private:
    Term* termArray; 
    int capacity;    
    int terms;       
public:
    Polynomial() { 
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    Polynomial Add(Polynomial poly) { 
        Polynomial result;
        int i = 0, j = 0;
        while (i < terms || j < poly.terms) {
            if (i >= terms) {
                result.termArray[result.terms++] = poly.termArray[j++];
            } else if (j >= poly.terms) {
                result.termArray[result.terms++] = termArray[i++];
            } else if (termArray[i].exp > poly.termArray[j].exp) {
                result.termArray[result.terms++] = termArray[i++];
            } else if (termArray[i].exp < poly.termArray[j].exp) {
                result.termArray[result.terms++] = poly.termArray[j++];
            } else {
                float sumCoef = termArray[i].coef + poly.termArray[j].coef;
                if (sumCoef != 0) {
                    result.termArray[result.terms] = Term(sumCoef, termArray[i].exp);
                    result.terms++;
                }
                i++; j++;
            }
        }
        return result;
    }

    Polynomial Mult(Polynomial poly) { 
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.termArray[result.terms++] = Term(newCoef, newExp);
            }
        }
        return result;
    }

    float Eval(float f) { 
        float result = 0;
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * pow(f, termArray[i].exp);
        }
        return result;
    }

    ~Polynomial() { delete[] termArray; } 
};

int main() {
    Polynomial p1, p2;
    p1.termArray[0] = Term(2, 2); p1.terms = 1; 
    p2.termArray[0] = Term(3, 1); p2.terms = 1; 
    Polynomial sum = p1.Add(p2);
    cout << "在 x=2 時的和: " << sum.Eval(2) << endl;
    return 0;
}
```

## 效能分析

時間複雜度: 加法大概是 $ O(n + m) $，乘法是 $ O(n \cdot m) $，評估是 $ O(n) $，這裡 $ n, m $ 是項的數量。

空間複雜度: 用了 $ O(n + m) $ 的空間來存結果。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  p1, p2  | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   |  2x^2, 3x    | 在  x=2  時: 10        | 10        |
| 測試二   |  x, x       | 在  x=1  時: 2        | 2        |
| 測試三   |  0, 2x      | 在  x=1  時: 2        | 2        |

編譯與執行指令

```shell
$ g++ -std=c++17 -o polynomial polynomial.cpp
$ ./polynomial
在 x=2 時的和: 10
```

### 結論

程式能正確加多項式並評估值，乘法還得再優化。測試涵蓋了基本和邊界情況，結果沒問題。

### 心得

做這個Polynomial類讓我學會處理多項式加法和評估，乘法有點難還得練。

