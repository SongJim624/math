#ifndef _MATH_OPTIMIZATION_UNSGA_REPRODUCER_
#define _MATH_OPTIMIZATION_UNSGA_REPRODUCER_
template<typename T>
class Reproducor
{
private:
    size_t scale_;
    std::vector<T, Allocator<T>> randoms_, weights_;
	Configuration<T>* configuration_;

private:
    void Random() {
        if(std::is_same<T, float>) {
            vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, scale_, &randoms_[0], 0, 1);
        }

        if(std::is_same<T, double>) {
            vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, scale_, &randoms_[0], 0, 1);
        }
    }

    void power(T exp, std::vector<T, Allocator<T>>& vector) {
        if(std::is_same<T, float>) {
            vsPowI(scale_, &vector[0], 1, &exp, 0, &vector[0], 1);
        }

        if(std::is_same<T, double>) {
            vdPowI(scale_, &vector[0], 1, &exp, 0, &vector[0], 1);
        }
    }

    void add(const std::vector<T, Allocator<T>>& lhs, const std::vector<T, Allocator<T>>& rhs, std::vector<T, Allocator<T>>& res) {
        if(std::is_same<T, float>) {
            vsAdd(scale_, &lhs[0], &rhs[0], &res[0]);
        }

        if(std::is_same<T, double>) {
            vdAdd(scale_, &lhs[0], &rhs[0], &res[0]);
        }
    }

    void add(T lhs, const std::vector<T, Allocator<T>>& rhs, std::vector<T, Allocator<T>>& res) {
        if(std::is_same<T, float>) {
            vsAddI(scale_, &lhs, 0, &rhs[0], 1, &res[0], 1);
        }

        if(std::is_same<T, double>) {
            vdAddI(scale_, &lhs, 0, &rhs[0], 1, &res[0], 1);
        }
    }

    void substract(T lhs, const std::vector<T, Allocator<T>>& rhs, std::vector<T, Allocator<T>>& res) {
        if(std::is_same<T, float>) {
            vsSubI(scale_, &lhs, 0, &rhs[0], 1, &res[0], 1);
        }

        if(std::is_same<T, double>) {
            vdSubI(scale_, &lhs, 0, &rhs[0], 1, &res[0], 1);
        }
    }

    void substract(const std::vector<T, Allocator<T>>& lhs, const std::vector<T, Allocator<T>>& rhs, std::vector<T, Allocator<T>>& res) {
        if(std::is_same<T, float>) {
            vsSub(scale_, &lhs[0], &rhs[0], &res[0]);
        }

        if(std::is_same<T, double>) {
            vdSub(scale_, &lhs[0], &rhs[0], &res[0]);
        }
    }

    void multiply(const std::vector<T, Allocator<T>>& lhs, const std::vector<T, Allocator<T>>& rhs, std::vector<T, Allocator<T>>& res) {
        if(std::is_same<T, float>) {
            vsMul(scale_, &lhs[0], &rhs[0], &res[0]);
        }

        if(std::is_same<T, double>) {
            vdMul(scale_, &lhs[0], &rhs[0], &res[0]);
        }
    }

private:
	void Cross(const Individual<T>& father, const Individual<T>& mother, Individual<T>& son, Individual<T>& daughter) {
        Random();
        for (size_t i = 0; i < scale_; ++i)
        {
            randoms_[i] = (randoms_[i] < 0.5) ? 2.0 * randoms_[i] : 0.5 / (1.0 - randoms_[i]);
        }

        power(1 / (configuration_->cross + 1), randoms_);

        add(1, randoms_, weights_);
        multiply(weights_, father.decisions, son.decisions);
        substract(1, randoms_, weights_);
        multiply(weights_, mother.decisions, weights_);
        add(weights_, son.decisions, son.decisions);

        substract(1, randoms_, weights_);
        multiply(weights_, father.decisions, daughter.decisions);
        add(1, randoms_, weights_);
        multiply(weights_, mother.decisions, weights_);
        add(weights_, daughter.decisions, son.decisions);

        cblas_sscal(scale_, 0.5, &son.decisions[0], 1);
        cblas_sscal(scale_, 0.5, &daughter.decisions[0], 1);
    }

	void Mutate(Individual<T>& individual) {
        std::vector<bool> labels(scale_, false);

        Random();

        for (size_t i = 0; i < scale_; ++i)
        {
            if (randoms_[i] > 0.5)
            {
                randoms_[i] = 1 - randoms_[i];
                labels[i] = true;
            }
        }

        cblas_sscal(scale_, 2, &randoms_[0], 1);
        power(1 / (configuration_->mutation + 1), randoms_);
        add(-1, randoms_, randoms_);

        for (size_t i = 0; i < scale_; ++i)
        {
            if (labels[i])
            {
                randoms_[i] = -randoms_[i];
            }
        }

        substract(configuration_->upper(), configuration_->lower(), individual.decisions);
        multiply(randoms_, individual.decisions, randoms_);
        add(randoms_, configuration_->lower(), individual.decisions);
    }

	void check(Individual<T>& individual);

public:
    Reproducor(Configuration<T>* configuration) {}

public:
	//worse individuals in the children set would be replaced by children
	std::list<Individual<T>*> Reproduce(std::pair<std::list<Individual<T>*>, std::list<Individual<T>*>>& population) {
        std::list<Individual<T>*> result, temporary;

        auto& elites = population.first;
        auto& ordinary = population.second;

        if (elites.size() % 2)
        {
            ordinary.push_front(*elites.rbegin());
            elites.pop_back();
        }

        auto father = elites.begin();
        auto mother = std::next(father);

        while (true)
        {
            if (ordinary.empty())
            {
                break;
            }

            auto son = ordinary.rbegin();
            auto daughter = std::next(son);

            Cross(**father, **mother, **son, **daughter);

            Random();

            if (randoms_[0] > configuration_->threshold)
            {
                Mutate(**son);
            }

            if (randoms_[1] > configuration_->threshold)
            {
                Mutate(**daughter);
            }

            temporary.push_back(*son);
            temporary.push_back(*daughter);

            ordinary.pop_back();
            ordinary.pop_back();

            std::advance(father, 2);
            if (father == elites.end())
            {
                break;
            }

            std::advance(mother, 2);
        }

        result.splice(result.end(), elites);
        result.splice(result.end(), temporary);
        result.splice(result.end(), ordinary);
        return result;
    }
};

template<typename T>
void Reproducor<T>::check(Individual<T>& individual)
{
	for (size_t i = 0; i < configuration_->dimension; ++i)
	{
		individual.decisions[i] = std::max(std::min(individual.decisions[i], configuration_->objective->upper()[i]), configuration_->objective->lower()[i]);

		if (configuration_->objective->integer()[i])
		{
			individual.decisions[i] = std::round(individual.decisions[i]);
		}
	}
}

#endif //!_MATH_OPTIMIZATION_UNSGA_REPRODUCER_
