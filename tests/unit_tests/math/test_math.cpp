#include "msrm_cpp_utils/math/math.hpp"
#include "catch2/catch.hpp"

namespace msrm_utils {

TEST_CASE("get rotation matrix from vector","[math]"){
    Eigen::Matrix<double,3,1> v1,v2;
    v1<<1,0,0;
    v2<<0,0,1;
    Eigen::Matrix<double,3,3> R1;
    Eigen::Matrix<double,3,3> R2;
    R1<<0,0,-1,0,1,0,1,0,0;
    R2<<1,0,0,0,0,-1,0,1,0;
    REQUIRE(build_rotation_matrix(v1,v2)==R1);
    v1<<0,2,0;
    REQUIRE(build_rotation_matrix(v1,v2)==R2);
}

TEST_CASE("concatenate_matrix","[math]"){
    Eigen::Matrix<double,3,3> R;
    R<<1,0,0,0,0,-1,0,1,0;
    Eigen::Matrix<double,3,1> v;
    v<<3,5,1;
    Eigen::Matrix<double,4,4> T;
    T<<1,0,0,3,0,0,-1,5,0,1,0,1,0,0,0,1;
    REQUIRE(concatenate_matrix(R,v)==T);
}

TEST_CASE("sign","[math]"){
    REQUIRE(sgn(1)==1);
    REQUIRE(sgn(-1)==-1);
    REQUIRE(sgn(0)==0);
}


}
