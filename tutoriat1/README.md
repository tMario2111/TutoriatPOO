# Tutoriat 1 - Alocare dinamică si constructori

## Pointeri:
|                 | *                                      | &                           |   |   |
|-----------------|----------------------------------------|-----------------------------|---|---|
| La inițializare | Declară un pointer                     | Declară o referință         |   |   |
| Ca operator     | Preia valoarea de la adresa respectivă | Preia adresa unei variabile |   |   |
|                 |                                        |                             |   |   |

Referință:
```cpp
int x = 2;
int &y = x;
++y;
cout << x << '\n'; // Afiseaza 3
```
**IMPORTANT**: Referințele nu pot fi nule și trebuie mereu inițializate
```cpp
int &x;        // Nu compileaza
int &y = NULL; // Nu compileaza
```

Pointer:
```cpp
int x = 10;
int* y = &x;
++(*y);
cout << x << '\n'; // Afiseaza 11
```

Alocare dinamică:
```cpp
int* x = new int(10); // *x ia valoarea 10
delete x;
```
**IMPORTANT**: Orice variabilă alocată dinamic trebuie ștearsă (`delete`) atunci când nu mai este folosită, altfel programul va avea un **memory leak**.

```cpp
int* x = new int[100];
delete[] x;
```
Un vector alocat dinamic trebuie șters cu `delete[]`, altfel se șterge doar primul element.

## Constructori
Constructorii pot fi neparametrizați, parametrizați și de copiere. Orice clasă are implicit un constructor neparametrizat și unul de copiere (care copiază bit cu bit). Dacă definim explicit un constructor neparametrizat / parametrizat, dispare cel implicit. Constructorul rulează de fiecare dată când un obiect nou este instanțiat.
```cpp
class C
{
public:
    C(int x) { }
};
int main()
{
    C c;     // Eroare, nu avem constructor implicit
    C c(10); // Trebuie sa apelam constructorul parametrizat
    return 0;
}
```

Următorul cod compilează?
```cpp
class C
{
public:
    C(int x) { }
};
int main()
{
    C c = 10;
    return 0;
}
```
<details>
<summary>Vezi răspuns</summary>

Da, codul va compila. Compiler-ul apelează constructorul parametrizat cu `x = 10`. În c++ există keyword-ul `explicit`, care te forțează să folosești explicit constructorul.
```cpp
class C
{
public:
    explicit C(int x) { }
};
int main()
{
    C c = 10; 
    return 0;
}
```
```
error: no viable conversion from 'int' to 'C'
    C c = 10;
```
</details>
<br/>
În mod similar, destructorul rulează de fiecare dată când obiectul devine „out of scope” sau este șters.  

### Exemplu constructori și destructor
```cpp
class MyClass
{
public:
    MyClass();
    MyClass(int x);
    MyClass(const MyClass& other);

    ~MyClass();
private:
    int x;
};

MyClass::MyClass() : x(0) // Initializam x
{
    cout << "Constructor neparametrizat\n";
}

MyClass::MyClass(int x) : x(x) // Initializam x cu parametrul dat
{
    // alternativ this->x = x;
    cout << "Constructor parametrizat\n";
}

MyClass::MyClass(const MyClass& other)
{
    x = other.x;
    cout << "Constructor de copiere\n";
}

MyClass::~MyClass()
{
    cout << "Destructor\n";
}

int main()
{
    {                    // Afiseaza:
        MyClass m1;      // Constructor neparametrizat
        MyClass m2(10);  // Constructor parametrizat
        MyClass m3(m1);  // Constructor de copiere
        MyClass m4 = m2; // ! Tot constructor de copiere !
    }
    // Afiseaza Destructor de 4 ori, toate obiectele sunt out of scope
    return 0;
}
```

### Ordine de apel
Ce afișează următorul program?
```cpp
class A
{
public:
    A();
    ~A();
};

A::A()
{
    cout << "A\n";
}

A::~A()
{
    cout << "~A\n";
}

class B
{
public:
    B();
    ~B();
};

B::B()
{
    cout << "B\n";
}

B::~B()
{
    cout << "~B\n";
}

class C
{
public:
    C();
    ~C();
private:
    B b;
    A a;
};

C::C()
{
    cout << "C\n";
}

C::~C()
{
    cout << "~C\n";
}

int main()
{
    A a;
    return 0;
}
```
<details>
<summary>Vezi răspuns</summary>

Programul afișează B A C ~C ~A ~B (pe rânduri noi). Se execută mai întâi constructorii obiectelor membre în ordinea în care au fost declarate și ulterior constructorul lui C. Deconstructorii se vor executa mereu în ordine inversă a constructorilor.

</details>

### De ce este necesar să redefinim constructorul de copiere?
Nu întotdeauna dorim să facem copia unui obiect bit cu bit. Intervin probleme atunci când în clasă avem memorie alocată dinamic. De exemplu:
```cpp
class A
{
public:
    A();
    ~A();
    int getX();
    void setX(int x);
private:
    int* x;
};

A::A()
{
    x = new int(10);
}

A::~A()
{
    delete x;
}

int A::getX()
{
    return *x;
}

void A::setX(int x)
{
    *this->x = x;
}

int main()
{
    A a1;
    A a2(a1);
    a2.setX(20);
    cout << a1.getX() << '\n'; // Afiseaza 20
    return 0;
}
```
Se poate vedea că, modificând x-ul din a2, acesta a fost modificat și în a1. Întrucât a fost folosit constructorul de copiere implicit, acesta a copiat adresa lui x din a1 în a2, astfel obiectele partajând același `int`. Mai mult, programul va da eroare deoarece fiecare destructor îl va șterge pe x, iar ștergerea unei zone de memorie de mai multe ori nu este permisă.

Programul corect va arăta așa:
```cpp
class A
{
public:
    A();
    A(const A& other);
    ~A();
    int getX();
    void setX(int x);
private:
    int* x;
};

A::A()
{
    x = new int(10);
}

A::A(const A& other)
{
    x = new int(*other.x);
}

A::~A()
{
    delete x;
}

int A::getX()
{
    return *x;
}

void A::setX(int x)
{
    *this->x = x;
}

int main()
{
    A a1;
    A a2(a1);
    a2.setX(20);
    cout << a1.getX() << '\n'; // Afiseaza 10
    return 0;
}
```