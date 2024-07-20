namespace math
{
    template<typename T>
    class matrix
    {
    public:
        auto operator () (size_t, size_t) -> T&
        {}

        auto operator () (size_t, size_t) const T&;
    };
}