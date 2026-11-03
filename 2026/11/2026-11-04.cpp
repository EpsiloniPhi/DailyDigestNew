// Daily Quant Digest 2026-11-04
// FR Lesson 9: recursion and the call stack.
// Warm-up: factorial, to see the stack of calls.
// Problem: fast power, x^n for a double x and any 32-bit int n (classic "Pow(x, n)").
// Bonus: the same recursion on exact integers, (a^n) mod m, used for a brute-force check.
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-04.cpp -o power && ./power
#include <climits>
#include <cmath>
#include <cstdio>
#include <random>

// Warm-up: n! by recursion. Each call waits for the smaller one to answer.
long long factorial(int n) {
    if (n <= 1) return 1;              // base case: the last doll, nothing inside
    return n * factorial(n - 1);       // recursive case: trust the helper, add one step
}

// Brute force: multiply x by itself |n| times. O(|n|) time.
double powSlow(double x, long long n) {
    bool negative = n < 0;
    if (negative) n = -n;
    double result = 1.0;
    for (long long i = 0; i < n; ++i) result *= x;
    return negative ? 1.0 / result : result;
}

// Fast power for n >= 0: x^n = (x^(n/2))^2, times one extra x if n is odd.
// depth (optional) records how many calls deep the recursion went.
double powRec(double x, long long n, int level, int* depth) {
    if (depth && level > *depth) *depth = level;
    if (n == 0) return 1.0;                          // base case
    double half = powRec(x, n / 2, level + 1, depth); // ONE recursive call, reused twice
    return (n % 2 == 0) ? half * half : half * half * x;
}

// The function an interviewer asks for. n can be any int, including INT_MIN.
double myPow(double x, int n) {
    long long N = n;                   // widen first: -INT_MIN does not fit in an int
    if (N < 0) return 1.0 / powRec(x, -N, 1, nullptr);
    return powRec(x, N, 1, nullptr);
}

// Bonus: (a^n) mod m with the same recursion, all in exact integer arithmetic.
// Requires m < 2^31 so that a product of two remainders fits in a long long.
long long powMod(long long a, long long n, long long m) {
    if (n == 0) return 1 % m;
    long long half = powMod(a, n / 2, m);
    long long r = half * half % m;
    if (n % 2 == 1) r = r * (a % m) % m;
    return r;
}

long long powModSlow(long long a, long long n, long long m) {
    long long r = 1 % m;
    for (long long i = 0; i < n; ++i) r = r * (a % m) % m;
    return r;
}

static int failures = 0;

void checkClose(const char* name, double got, double want, double relTol) {
    double diff = std::fabs(got - want);
    double scale = std::fmax(1.0, std::fabs(want));
    if (diff > relTol * scale) {
        ++failures;
        std::printf("FAIL %s: got %.15g want %.15g\n", name, got, want);
    }
}

int main() {
    // Warm-up
    std::printf("factorial(5) = %lld\n", factorial(5));                  // 120
    if (factorial(5) != 120 || factorial(20) != 2432902008176640000LL) ++failures;

    // Examples from the text
    struct Case { double x; int n; double want; };
    const Case cases[] = {
        {2.0, 10, 1024.0},
        {2.1, 3, 9.261},
        {2.0, -2, 0.25},
        {1.0, INT_MIN, 1.0},
        {-1.0, INT_MIN, 1.0},          // INT_MIN is even
        {-1.0, INT_MAX, -1.0},         // INT_MAX is odd
        {0.5, 0, 1.0},
        {-2.0, 5, -32.0},
    };
    for (const Case& c : cases) {
        double got = myPow(c.x, c.n);
        std::printf("myPow(%g, %d) = %.10g\n", c.x, c.n, got);
        checkClose("example", got, c.want, 1e-12);
    }

    // Compounding (Derivatives Lesson 1): 4% a year, compounded daily for one year.
    double daily = myPow(1.0 + 0.04 / 365.0, 365);
    std::printf("(1 + 0.04/365)^365 = %.6f\n", daily);                  // 1.040808
    checkClose("daily compounding", daily, std::pow(1.0 + 0.04 / 365.0, 365), 1e-12);

    // Depth of the call stack for the largest n
    int depth = 0;
    powRec(1.0, INT_MAX, 1, &depth);
    std::printf("calls deep for n = INT_MAX: %d\n", depth);             // 32
    if (depth != 32) ++failures;

    // Randomized check 1: small n against the slow loop
    std::mt19937_64 rng(20261104);
    std::uniform_real_distribution<double> ux(0.5, 1.5);
    std::uniform_int_distribution<int> unSmall(-300, 300);
    for (int t = 0; t < 100000; ++t) {
        double x = ux(rng);
        int n = unSmall(rng);
        checkClose("vs slow loop", myPow(x, n), powSlow(x, n), 1e-11);
    }

    // Randomized check 2: huge n against std::pow (x close to 1 keeps answers finite)
    std::uniform_real_distribution<double> uxNear(1.0 - 1e-9, 1.0 + 1e-9);
    std::uniform_int_distribution<int> unBig(INT_MIN, INT_MAX);
    for (int t = 0; t < 100000; ++t) {
        double x = uxNear(rng);
        int n = unBig(rng);
        checkClose("vs std::pow", myPow(x, n), std::pow(x, n), 1e-6);
    }

    // Randomized check 3: exact integers, recursion vs slow loop
    std::uniform_int_distribution<long long> ua(0, 1000000000LL), un(0, 2000), um(1, 1000000007LL);
    for (int t = 0; t < 20000; ++t) {
        long long a = ua(rng), n = un(rng), m = um(rng);
        if (powMod(a, n, m) != powModSlow(a, n, m)) {
            ++failures;
            std::printf("FAIL powMod(%lld, %lld, %lld)\n", a, n, m);
        }
    }
    std::printf("2^1000000000 mod 1000000007 = %lld\n", powMod(2, 1000000000LL, 1000000007LL));

    std::printf(failures == 0 ? "All tests passed.\n" : "%d test(s) FAILED.\n", failures);
    return failures == 0 ? 0 : 1;
}
