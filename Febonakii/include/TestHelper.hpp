#ifndef TESTHELPER_HPP
#define TESTHELPER_HPP

int _fib(int n)
{
    if (n <= 1)
        return n;
    return _fib(n-1) + _fib(n-2);
}
inline bool testResults(Analyzer analyzer, int input) {
    int actuals, expected;
 
    expected = _fib(input);
    
    actuals = analyzer.fib(input);
    return (expected == actuals);
    
}

#endif // TESTHELPER_HPP
