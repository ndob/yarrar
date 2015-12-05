namespace yarrar_test {

template <typename T>
bool almostEqual(const T& a, const T& b, const T& epsilon)
{
    return std::abs(a - b) < epsilon;
}

}


