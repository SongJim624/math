//This for the continuous distribution
void Uniform(const float &a = 0.0, const float &b = 1.0, float &rng)
{
    rng = (rand() / (float)RAND_MAX) * (b - a) + a;
}
