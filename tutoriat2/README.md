# Tutoriat 2 - Supraîncărcarea operatorilor, const, static

## Supraîncărcarea operatorilor

### Operatorul de atribuire
```cpp
MyClass& operator=(const MyClass& other)
{
    if (this != &other)
    {
        // Aici se face copierea efectiva
        this->x = other.x;
        // ...
    }
    return *this;
}
```
Funcția returnează o referință la obiectul curent pentru a avea chaining pe operatorul de atribuire (`x = y = z = ...`). De asemenea, este important să facem copierea doar dacă obiectele sunt distincte (`this != &other`).

```cpp
class MyClass {
public:
    MyClass() : x(new int(10)) {}

    MyClass& operator=(const MyClass& other) {
        delete this->x;
        this->x = new int(*other.x);
        return *this;
    }
public:
    int* x;
};
```
În cazul de mai sus, dacă nu ne asigurăm că avem obiecte distincte și facem operația de atribuire pe obiectul însuși (`obj = obj`), ștergerea se va face înainte de alocare.

### Operatorii de stream
Operatorii de flux permit citirea și afișarea unui obiect folosind stream-uri (`cin, cout, ifstream, ofstream, etc.`):
```cpp
friend std::ostream &operator<<(std::ostream &os, const MyClass &obj)
{
    os << "x: " << obj.x << " y: " << obj.y;
    return os;
}
friend std::istream &operator>>(std::istream &is, MyClass &obj)
{
    is >> obj.x >> obj.y;
    return is;
}
// [...]
int main()
{
    MyClass obj;
    cin >> obj;
    cout << obj;
    return 0;
}
```
Funcția este definită ca prieten întrucât operatorul face parte din `istream` / `ostream`, iar noi de dorim să poată accesa datele membre private ale clasei noastre.

### Operatorul de incrementare prefixat / postfixat
#### Prefixat (`++obj`)
```cpp
MyClass& operator++()
{
    this->x++;
    return *this;
}
```
#### Postfixat (`obj++`)
```cpp
MyClass operator++(int)
{
    MyClass old = *this;
    this->x++;
    return old;
}
```
Pentru varianta postfixată se folosește un parametru redundant (`dummy`) care să îl diferențieze de cel prefixat.

### Alți operatori
Mai sunt mulți alți operatori care pot fi supraîncărcați, așadar puteți citi mai multe pe [cppreference](https://en.cppreference.com/w/cpp/language/operators).

## Const
În `c++`, keyword-ul `const` este folosit în mai multe contexte:
- variabile constante
- metode constante

### Variabile constante
După cum spune și numele, variabilele constante sunt variabile cărora nu le putem schimba valoarea.
```cpp
const int x;     // Eroare, o variabilă constantă trebuie inițializată
const int y = 2; // Compilează
y = 3;           // Nu compilează, nu îi putem schimba valoarea
```
Într-o clasă, o dată membră constantă trebuie inițializată în fiecare constructor, **folosind liste de inițializare**:
```cpp
class MyClass
{
public:
    // Corect
    MyClass() : x(2) {}
    MyClass(int x) : x(x) {}
    MyClass(const MyClass &other) : x(other.x) {}

    MyClass(int x, int y) 
    {
        this->x = x; // Eroare, x este constant
    }
private:
    const int x;
};
```
Devine puțin mai complicat când folosim pointeri:

|                   | Pointer variabil                  | Pointer constant     |
| ----------------- | --------------------------------- | -------------------- |
| Valoare variabilă | `int* x`                           | `int* const x`       |
| Valoare constantă | `const int* x` sau `int const* x` | `const int* const x` |

Prin pointer constant înțelegem că nu putem modifica `x`, iar prin valoare constantă nu putem modifica `*x`.

Keyword-ul `const` poate fi folosit și atunci când declarăm obiecte.
```cpp
class A
{
public:
    int x;
};

int main()
{
    const A a;
    a.x = 10;  // Eroare, nu putem modifica x, obiectul a este constant
    return 0;
}
```

Keyword-ul `const` aplicat asupra unui obiect constrânge și ce metode pot fi apelate, întrucât nu putem modifica starea obiectului.

### Metode constante

```cpp
class A {
    int x;
public:
    void f() {
        this->x++;
    }
};

int main() {
    const A a;
    a.f(); // Eroare
    return 0;
}
```
Codul de mai sus va da eroare, întrucât metoda `f` modifică starea obiectului. Dar să presupunem că încercăm să apelăm o metodă care nu modifică obiectul:

```cpp
class A {
    int x;
public:
    int getX() {
        return x;
    }
};

int main() {
    const A a;
    a.getX(); // Eroare
    return 0;
}
```
Cu toate că metoda `getX` nu modifică starea obiectului, codul de mai sus nu compilează. Pentru a apela o metodă a unui obiect constant este necesar să specificăm că este constantă (și să nu facem modificări asupra obiectului, bineînțeles).
```cpp
class A {
    int x;
public:
    int getX() const {
        return x;
    }
};

int main() {
    const A a;
    a.getX(); // Compileaza!
    return 0;
}
```
Mai general, `const` aplicat unei metode specifică că `this` este constant.

Din păcate, chiar dacă metoda este constantă, putem face niște „trucuri” pentru a modifica starea obiectului. Un astfel de truc este aplicarea unui `C cast` asupra `this`:
```cpp
class A {
    int x = 1;
public:
    void f() const {
        A* neconstant = (A*)this;
        neconstant->x++;
    }
};

int main() {
    const A a;
    a.f();  // Codul compilează iar valoarea lui x a fost modificată cu succes
    return 0;
}
```

# Static

Variabilele statice într-o clasă se alocă o singură dată și sunt comune tuturor obiectelor. Ele trebuie inițializare **explicit, global**:
```cpp
class B {
public:
    B(int n) {}
    int x;
};

class A {
    static int x;
    static B b;
};

// Inițializare
int A::x = 2;
B A::b(3);
```
Întrucât sunt comune tuturor obiectelor, ele nu se accesează cu `.` / `->`, ci cu operatorul de scop (`::`):
```cpp
cout << A::x << '\n';
```

Metodele statice nu au `this`, deci nu putem interacționa cu datele membre ale obiectului, doar cu variabilele și obiectele definite ca fiind statice:
```cpp
class A {
public:
    static int x;
    int y = 3;
    static void f() {
        cout << x << '\n';        // Compilează, x este static
        cout << this->y << '\n';  // Nu compilează, nu există `this`
    }
};
int A::x = 2;

int main() {
    A::f();
    return 0;
}
```
De asemenea, `static` se poate folosi și pentru a defini variabile statice în funcții:
```cpp
void f() {
    static int x = 0;
    cout << x << '\n';
    ++x;
}

int main() {
    f(); // 0
    f(); // 1
    f(); // 2
    return 0;
}
```
Inițializarea se face o singură dată (x nu va lua valoarea 0 de fiecare dată când este rulată funcția).