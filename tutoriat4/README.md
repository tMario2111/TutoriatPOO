# Tutoriat 4 - Introducere în moștenire și smart pointers

Moștenirea este una dintre modalitățile prin care putem scrie cod modular și reutilizabil. Dacă compunerea definește relații de tip „has a” (i.e. mașina are un motor), moștenirea definește relații de tip „is a” (i.e. câinele este un animal). Puteți citi mai multe despre conceptul de moștenire [aici](
    https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/
).

## Modificatori de acces
Moștenirea în `c++` este de 3 tipuri: `public`, `protected`, `private`.

| Tip moștenire \ Specificator acces în bază | Public                | Protejat              | Privat         |
|-------------------------------|-----------------------|-----------------------|----------------|
| `public`                       | Public                | Protejat              | Inaccesibil    |
| `protected`                    | Protejat              | Protejat              | Inaccesibil    |
| `private`                      | Privat                | Privat                | Inaccesibil    |

Cu alte cuvinte:
* moștenire publică: datele își păstrează modificatorul de acces în clasa derivată
* moștenire protejată: datele publice devin protejate în clasa derivată
* moștenire privată: toate datele devin private în clasa derivată
* **Datele private din clasa de bază nu se moștenesc!**

## Ordinea de apel a constructorilor / destructorilor
Să presupunem că avem următoarele clase
```cpp
class A {
public:
    A() { std::cout << "A "; }
    ~A() { std::cout << "~A "; }
};

class B {
public:
    B() { std::cout << "B "; }
    ~B() { std::cout << "~B "; }
};

class C : public A {
public:
    C(): A() { std::cout << "C "; }
    ~C() { std::cout << "~C "; }
private:
    B b;
};

int main() {
    C c;
    return 0;
}
```
Prima oară se apelează constructorul clasei de bază (dacă sunt mai multe clase de bază se apelează în ordinea în care a fost făcută moștenirea), apoi constructorii obiectelor membre (în ordinea în care au fost declarate, de sus în jos), apoi constructorul clasei noastre. Deci, codul de sus va afișa `A B C ~C ~B ~A` (destructorii se apelează în ordine inversă față de constructori).

## Moștenirea operatorilor
În c++, toți operatorii membri se moștenesc, cu excepția operatorului de atribuire. Operatorii non-membri (`friend`) nu se moștenesc, este nevoie de un cast explicit pentru a-i apela. Dacă nu definim un operator de atribuire în clasa derivată, este creat unul implicit care îl apelează pe cel din clasa de bază. Totuși, de cele mai multe ori ne dorim să extindem funcționaliatea operatorilor și în clasa derivată, fără să îi rescriem pe cei din clasa de bază. Pentru asta, putem apela explicit operatori din clasa de bază. De exemplu, pentru operatorul de atriuire:

```cpp
class B {
public:
    B(int x) : x(x) { }

    B &operator =(const B &other) {
        if (this != &other)
            this->x = other.x;
        return *this;
    }
protected:
    int x;
};

class D : public B {
public:
    D(int x, int y) : B(x), y(y) { }

    D &operator =(const D &other) {
        if (this != &other) {
            B::operator =(other);
            this->y = other.y;
        }
        return *this;
    }
private:
    int y;
};
```
Sau pentru operatorul de stream
```cpp
class B {
public:
    B(int x) : x(x) { }

    friend std::ostream &operator<<(std::ostream &os, const B &obj) {
        return os << "x: " << obj.x;
    }
protected:
    int x;
};

class D : public B {
public:
    D(int x, int y) : B(x), y(y) { }

    friend std::ostream &operator<<(std::ostream &os, const D &obj) {
        return os
               << static_cast<const B &>(obj)
               << " y: " << obj.y;
    }
private:
    int y;
};
```
Am realizat acel cast în clasa derivată pentru a apela explicit operatorul de stream definit pe clasa de bază (operatorul este prieten, deci nu îl mai putem apela din `B`).

## Redefinirea funcțiilor membre
În `c++` putem redefini funcții membre ale clasei de bază în clasa derivată.
```cpp
class B {
public:
    void f() { cout << "B\n"; }
};

class D : public B {
public:
    void f() { cout << "D\n"; }
};

int main() {
    D d;
    d.f();
    return 0;
}
```
Următorul cod va afișa `D`. Totuși, intervin probleme atunci când folosim pointeri:
```cpp
int main() {
    B* d = new D;
    d->f();
    delete d;
    return 0;
}
```
Se va afișa `B`, chiar dacă obiectul este de tip `D` (în memorie). Faptul că îl atribuim unui pointer de tip `B` determină obiectul să nu apeleze funcția redefinită. Vom vedea în următorul tutoriat de ce ne dorim să atribuim obiecte ale clasei derivate în pointeri de tip clasă de bază și cum putem rezolva această situație cu `virtual`.

## Smart pointers
În `c++`, memoria este gestionată manual, programatorul fiind responsabil să elibereze memoria alocată. Cu toate acestea, noile versiuni de `c++` au introdus conceptul de `smart pointers`. Acești pointeri își eliberează memoria automat atunci când aceasta nu mai este necesară. Ei sunt de 2 tipuri:
* Unique
* Shared

### `unique_ptr`
După cum spune și numele, pointerii unici sunt deținuți de o singură resursă. Mai concret, evidența acestora este ținută într-un singur loc (într-o funcție, într-un obiect, etc.). Atunci când resursa este ștearsă sau iese din `scope`, pointer-ul este șters. Sintaxa este următoarea:
```cpp
class C {
public:
    C(int x) : x(x) { }
    void f() { }
private:
    int x;
};

int main() {
    std::unique_ptr<C> p1(new C(1)); // Metoda 1
    std::unique_ptr<C> p2 = std::make_unique<C>(2); // Metoda 2
    std::unique_ptr<C> p3 = p1; // Eroare, pointer-ul este unic, nu poate fi copiat
    return 0;
}
```
`unique_ptr` poate fi inițializat în 2 moduri, cu `new` sau cu `make_unique` (a doua variantă este recomandată). De asemenea, el nu poate fi copiat (altfel nu ar mai fi unic). În rest, sintaxa este similară cu pointerii clasici:
```cpp
int main()
{
    std::unique_ptr<C> p = std::make_unique<C>(2);
    p->f();
    C &p_ref = *p;
    return 0;
}
```
Chiar dacă nu putem copia pointer-ul, putem transmite referințe la obiect, astfel având acces la acesta fără a-i influența `lifetime`-ul.
```cpp
void g(C& c)
{
    c.f();    
}

int main()
{
    std::unique_ptr<C> p = std::make_unique<C>(2);
    g(*p);
    return 0;
}
```

### `shared_ptr`
Pointerii `shared` pot fi deținuți de mai multe resurse, aceștia fiind șterși atunci când nu mai este referențiat nicăieri. Această tehnică se numește [reference counting](https://en.wikipedia.org/wiki/Reference_counting). Pointerii `shared` sunt instanțiați în mod similar cu cei unici (`make_shared`).
```cpp
class A {
public:
    ~A() { cout << "~A\n"; }
};

class B {
public:
    B() {
        a = std::make_shared<A>();
    }
    std::shared_ptr<A> getA() {
        return this->a;
    }
private:
    std::shared_ptr<A> a;
};

int main()
{
    {
        B* b = new B();
        std::shared_ptr<A> a = b->getA();
        delete b;
        cout << "Obiectul nu a fost sters inca\n";
    }
    cout << "Obiectul a fost sters\n";
    return 0;
}
```
Se poate observa că `shared_ptr<A>` a fost șters după ce `std::shared_ptr<A> a` a ieșit din `scope`, nu după ce l-am șters pe `b`, întrucât acesta încă mai era referențiat.