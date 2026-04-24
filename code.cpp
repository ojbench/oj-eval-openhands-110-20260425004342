#include <bits/stdc++.h>
using namespace std;

class Date {
private:
    int _year;
    int _month;
    int _day;

    static bool is_leap(int y) {
        if (y % 400 == 0) return true;
        if (y % 100 == 0) return false;
        return y % 4 == 0;
    }
    static int days_in_month(int y, int m) {
        static const int mdays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        if (m == 2) return mdays[m] + (is_leap(y) ? 1 : 0);
        return mdays[m];
    }
    static bool valid(int y, int m, int d) {
        if (y < 1) return false;
        if (m < 1 || m > 12) return false;
        int dim = days_in_month(y, m);
        if (d < 1 || d > dim) return false;
        return true;
    }

    // Howard Hinnant's algorithms
    static long long days_from_civil(int y, unsigned m, unsigned d) {
        y -= m <= 2;
        const int era = (y >= 0 ? y : y - 399) / 400;
        const unsigned yoe = static_cast<unsigned>(y - era * 400);              // [0, 399]
        const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;     // [0, 365]
        const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;              // [0, 146096]
        return era * 146097LL + static_cast<long long>(doe) - 719468LL;          // days since 1970-01-01
    }
    static void civil_from_days(long long z, int &y, unsigned &m, unsigned &d) {
        z += 719468LL;
        const long long era = (z >= 0 ? z : z - 146096) / 146097;
        const unsigned doe = static_cast<unsigned>(z - era * 146097);            // [0, 146096]
        const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
        y = static_cast<int>(yoe) + static_cast<int>(era) * 400;
        const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
        const unsigned mp = (5 * doy + 2) / 153;                                 // [0, 11]
        d = doy - (153 * mp + 2) / 5 + 1;                                        // [1, 31]
        m = mp + (mp < 10 ? 3 : -9);                                             // [1, 12]
        y += (m <= 2);
    }

    long long to_days() const { return days_from_civil(_year, _month, _day); }
    static Date from_days(long long z) {
        int y; unsigned m, d;
        civil_from_days(z, y, m, d);
        return Date(y, static_cast<int>(m), static_cast<int>(d), true);
    }

    // internal constructor bypassing validation
    Date(int y, int m, int d, bool /*trusted*/) : _year(y), _month(m), _day(d) {}

public:
    Date() : _year(1900), _month(1), _day(1) {}
    Date(int y, int m, int d) {
        if (valid(y, m, d)) { _year = y; _month = m; _day = d; }
        else { _year = 1900; _month = 1; _day = 1; }
    }

    // add/subtract days
    Date operator+(int days) const {
        long long z = to_days();
        return from_days(z + days);
    }
    Date operator-(int days) const { return (*this) + (-days); }

    // difference in days (absolute value)
    long long operator-(const Date &other) const {
        long long diff = llabs(to_days() - other.to_days());
        return diff;
    }

    // comparisons
    bool operator<(const Date &other) const {
        if (_year != other._year) return _year < other._year;
        if (_month != other._month) return _month < other._month;
        return _day < other._day;
    }

    // ++/--
    Date &operator++() { // pre-increment
        *this = *this + 1;
        return *this;
    }
    Date operator++(int) { // post-increment
        Date tmp = *this;
        *this = *this + 1;
        return tmp;
    }
    Date &operator--() { // pre-decrement
        *this = *this - 1;
        return *this;
    }
    Date operator--(int) { // post-decrement
        Date tmp = *this;
        *this = *this - 1;
        return tmp;
    }

    // output helper
    void out() const { cout << _year << '-' << _month << '-' << _day << '\n'; }

    friend ostream &operator<<(ostream &os, const Date &dt) {
        os << dt._year << '-' << dt._month << '-' << dt._day;
        return os;
    }
};

static void Test() {
    int op; if (!(cin >> op)) return;
    int yy, mm, dd;
    if (op == 1 || op == 0) {
        Date d0;
        Date d1(2000, 2, 29);
        Date d2(1900, 2, 29);
        cout << d0 << '\n';
        cout << d1 << '\n';
        cout << d2 << '\n';
        // d0.out(); d1.out(); d2.out();
    }
    if (op == 2 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        for (int i = 0; i < 5; ++i) cout << ++d0 << '\n';
        for (int i = 0; i < 5; ++i) cout << d0++ << '\n';
        for (int i = 0; i < 5; ++i) cout << d0-- << '\n';
        for (int i = 0; i < 2; ++i) cout << --d0 << '\n';
        cout << d0 << '\n';
        // d0.out();
    }
    if (op == 3 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        cout << d0 + 100 << '\n';
        cout << d0 - 1000 << '\n';
    }
    if (op == 4 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        Date d1(2020, 12, 21);
        cout << (d0 < d1) << '\n';
    }
    if (op == 5 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        Date d1(1912, 6, 23);
        cout << (d0 - d1) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Test();
    return 0;
}
