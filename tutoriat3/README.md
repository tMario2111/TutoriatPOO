# Tutoriat 3 - Exemplu clasă `ComplexNumber`

Ne propunem să scriem o clasă `ComplexNumber` care să simuleze cât mai bine funcționalitățile unui număr complex. După cum știți, un număr complex este de forma `a + b * i`, deci datele membre ale clasei noastre vor fi `a` (partea reală) și `b` (partea imaginară)

```cpp 
class ComplexNumber
{
private:
    float a, b;
};
```

Următorul pas este să definim constructorii

```cpp
// Constructor parametrizat
ComplexNumber::ComplexNumber() : a(0.f), b(0.f)
{
}

// Constructor neparametrizat
ComplexNumber::ComplexNumber(float a, float b) : a(a), b(b) // parametrizat
{
}

// Constructor de copiere
ComplexNumber::ComplexNumber(const ComplexNumber &other) : a(other.a), b(other.b)
{
}
```
`f`-ul din `0.f` specifică faptul că 0 este un număr real de tip float (pentru a nu se confunda cu double)

Deși în cazul nostru nu este neapărat necesar, ar fi bine să definim și destructorul

```cpp
ComplexNumber::~ComplexNumber() {}
```

Întrucât dorim să avem libertatea de a modifica, cât și prelua, valorile lui `a` și `b`, definim `setteri` și `getteri` publici

```cpp
float ComplexNumber::getA() const
{
    return this->a;
}

float ComplexNumber::getB() const
{
    return this->b;
}

void ComplexNumber::setA(float a)
{
    this->a = a;
}

void ComplexNumber::setB(float b)
{
    this->b = b;
}
```
Reamintim că metodele `getter` pot fi făcute `const` pentru a putea fi apelate din obiecte constante (nu modifică starea obiectului, deci poate fi `const`)

Mai departe, putem supraîncărca în clasa noastră operatori pe care îi folosim mereu. Primul este operatorul de atribuire
```cpp
ComplexNumber &ComplexNumber::operator=(const ComplexNumber &other)
{
    if (this != &other)
    {
        this->a = other.a;
        this->b = other.b;
    }
    return *this;
}
```
Pentru a ne face munca mai ușoară, vom suprascrie și operatorii de `stream` (`<<`, `>>`)
```cpp
// friend
std::ostream &operator<<(std::ostream &os, const ComplexNumber &num)
{
    os << num.a << " + " << num.b << "i";
    return os;
}

// friend
std::istream &operator>>(std::istream &is, ComplexNumber &num)
{
    is >> num.a >> num.b;
    return is;
}
```
Înainte de a continua, vom testa ce am făcut până acum
```cpp
int main()
{
    ComplexNumber c1(10, 15);
    ComplexNumber c2 = c1;
    c1.setB(3);
    cout << c1 << '\n' << c2 << '\n';
    return 0;
}
// Afiseaza
// 10 + 3i
// 10 + 15i
```
De reținut că `ComplexNumber c2 = c1` apelează constructorul de copiere, nu operatorul de atribuire. Dacă adăugăm pe următoarea linie `c2 = c1`, atunci în acest caz de va apela operatorul de atribuire

În continuare, dorim să implementăm operații între numere complexe, precum adunare, scădere, înmulțire, împărțire. Bineînțeles, putem avea metode precum `c1.add(c2)` care să returneze suma dintre `c1` și `c2`, dar, pentru a avea un cod cât mai robust, vom supraîncărca operatorii necesari

```cpp
ComplexNumber ComplexNumber::operator+(const ComplexNumber &other) const
{
    return ComplexNumber(this->a + other.a, this->b + other.b);
}

ComplexNumber &ComplexNumber::operator+=(const ComplexNumber &other)
{
    this->a += other.a;
    this->b += other.b;
    return *this;
}
```
Operatorul `+` returnează un nou obiect de tip `ComplexNumber`. Operatorul `+=` returnează o referință la obiectul curent, pentru a permite înlănțuire (`c1 += c2 += c1 += c2`). Testăm operatorii noi definiți:
```cpp
int main()
{
    ComplexNumber c1(1, 5);
    ComplexNumber c2(2, 3);
    cout << c1 + c2 + c1 << '\n'; // 4 + 13i
    c2 += c1;
    cout << c2 << '\n';           // 3 + 8i
    return 0;
}
```

În mod similar se pot supraîncărca și operatorii de scădere. La operatorii de înmulțire și împărțire diferă doar funcționalitatea lor. De exemplu, înmulțirea a două numere complexe se face în felul următor:
`(a + bi) * (c + di) = ac + adi + bci - bd = (ac - bd) + (ad + bc)i`

Devine mai interesant dacă dorim să putem aduna un număr real la un număr complex (`c1 + 2`). Putem să supraîncărcam operatorul `+` astfel:
```cpp
ComplexNumber ComplexNumber::operator+(const float other) const
{
    return ComplexNumber(this->a + other, this->b);
}
```
```cpp
int main()
{
    ComplexNumber c1(1, 5);
    cout << c1 + 2 << '\n'; // 3 + 5i
    cout << 2 + c1 << '\n'; // Eroare
    return 0;
}
```
Putem observa că dacă încercăm să punem în partea stângă a adunării numărul real, programul nu va compila. Compilatorul nu tratează `+` ca fiind comutativ, trebuie să definim noi explicit operația.
```cpp
// friend
ComplexNumber operator+(const float a, const ComplexNumber &other)
{
    return ComplexNumber(a + other.a, other.b);
}
```
Întrucât nu mai avem un obiect de tip `ComplexNumber` în partea stângă a operatorului, `operator+` nu va mai aparține de obiect, ci îl definim ca funcție prieten al clasei. Aceasta are doi parametri, partea stângă a operatorului (număr real) și partea dreaptă (`ComplexNumber`)

În final, ne dorim să avem o metodă `abs` (modului unui număr complex), dar să nu o apelăm din obiect (`c1.abs()`), ci cum am apela o funcție din `<cmath>` (`abs(c1)`). Putem să facem acest lucru folosind o metodă statică
```cpp
static float abs(const ComplexNumber &num);
// [...]
float ComplexNumber::abs(const ComplexNumber &num)
{
    return sqrtf(num.a * num.a + num.b * num.b);
}
// [...]
int main()
{
    ComplexNumber c1(1, 5);
    ComplexNumber c2(2, 3);
    cout << ComplexNumber::abs(c1 + c2) << '\n';
    return 0;
}
```
A avut sens să folosim `static` în acest context întrucât metoda nu accesează `this`, `abs` având rol de funcție utilitară în clasa `ComplexNumber`

Implementarea clasei o puteți găsi în fișierele [ComplexNumber.hpp](./ComplexNumber.hpp) și [ComplexNumber.cpp](./ComplexNumber.cpp)