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

TEST_CASE("distance","[math]"){
    Eigen::Matrix<double,4,4> T_1;
    Eigen::Matrix<double,4,4> T_2;
    T_1<<-0.956395958000000,-0.292073218000000,0.000084963000000,0,0.292073230000000,-0.956395931000000,0.000227268000000,0,0.000014880000000,0.000242173000000,0.999999971000000,0,0,0,0,1;
    T_2<<-0.956227882000000,-0.292623029000000,-0.000021887000000,0.15,0.292623030000000,-0.956227882000000,-0.000024473000000,-0.5,-0.000013768000000,-0.000029806000000,0.999999999000000,0.1,0,0,0,1;

    REQUIRE(fabs(get_linear_distance(T_1,T_2)-0.5315072906)<1e-6);
    REQUIRE(fabs(get_angular_distance(T_1,T_2)-0.000636266)<1e-6);
}


}
