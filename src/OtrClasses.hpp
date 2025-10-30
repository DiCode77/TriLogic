//
//  OtrClasses.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef OtrClasses_hpp
#define OtrClasses_hpp

#include <cstdlib>
#include <random>

namespace OTR {
class Rand{
    std::random_device rd;
    std::mt19937 rand_g;
    std::uniform_int_distribution<int> u_dist;
public:
    Rand(int _min, int _max) : u_dist(_min, _max), rand_g(this->rd()){};
    operator int(){
        return this->u_dist(this->rand_g);
    }
};
}


#endif /* OtrClasses_hpp */
