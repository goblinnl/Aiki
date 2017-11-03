#ifndef MIXER_C_MATH_H
#define MIXER_C_MATH_H

// No Global Math Functions
template<class T> T Min(const T &r1, const T &r2) { return r1 <= r2 ? r1 : r2; }
template<class T> T Max(const T &r1, const T &r2) { return r1 >= r2 ? r1 : r2; }
template<class T> T Abs(const T &r1){ return r1 < 0 ? -r1 : r1; }
template<class T> T Swap(const T &r1, const T &r2){ T& t = r1; r1 = r2; r2 = t;}

#endif // MIXER_C_MATH_H