#ifndef POINT_HH
#define POINT_HH

#include <cassert>
#include <cmath>
#include <vector>
using namespace std;

template <class T, int N>
class Point
{
private:
    vector<T> coordinates;

public:
    Point() { coordinates = vector<T>(N); }

    Point(const vector<T> &coordinates)
    {
        this->coordinates = coordinates;
    }

    void setCoordinate(int i, T coordinate)
    {
        assert(i < N);
        coordinates[i] = coordinate;
    }

    T getCoordinate(int i) const { return coordinates[i]; }

    static double sqEuclidianDistance(const Point<T, N> &p1,
                                      const Point<T, N> &p2)
    {
        double sqDistance = 0;
        for (uint i = 0; i < N; ++i)
            sqDistance = pow(p1.getCoordinate(i) - p2.getCoordinate(i), 2);
        return sqDistance;
    }

    static double euclidianDistance(const Point<T, N> &p1,
                                    const Point<T, N> &p2)
    {
        return sqrt(sqEuclidianDistance(p1, p2));
    }

    template <class P, int M>
    void operator=(const Point<P, M> &p2)
    {
        int n = min(M, N);
        for (uint i = 0; i < n; ++i)
            this->coordinates[i] = p2.getCoordinate(i);
    }

    void operator+=(const Point<T, N> &p2)
    {
        for (uint i = 0; i < N; ++i)
            this->coordinates[i] += p2.getCoordinate(i);
    }

    void operator-=(const Point<T, N> &p2)
    {
        for (uint i = 0; i < N; ++i)
            this->coordinates[i] -= p2.getCoordinate(i);
    }

    void operator*=(const Point<T, N> &p2)
    {
        for (uint i = 0; i < N; ++i)
            this->coordinates[i] *= p2.getCoordinate(i);
    }

    static double dotProduct(const Point<T, N> &p1, const Point<T, N> &p2)
    {
        double dotProduct = 0;
        for (uint i = 0; i < N; ++i)
            dotProduct += p1.getCoordinate(i) * p2.getCoordinate(i);
        return dotProduct;
    }
};

#endif