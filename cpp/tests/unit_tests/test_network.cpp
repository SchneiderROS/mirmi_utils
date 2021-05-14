#include "msrm_utils/network.hpp"
#include "msrm_utils/benchmarking.hpp"
#include <utility>
#include "catch/catch.hpp"

namespace msrm_utils {

TEST_CASE("ping","[network]"){
    REQUIRE(ping("127.0.0.1"));
    REQUIRE(ping("0.0.0.0"));
//    REQUIRE(ping("localhost"));
    REQUIRE(!ping("192.168.2.54"));
    REQUIRE(!ping("abc"));
    REQUIRE(!ping("128.0.0.1"));
}

TEST_CASE("ip address check","[network]"){
    REQUIRE(!is_valid_ip_address("1"));
    REQUIRE(is_valid_ip_address("1.1.1.1"));
    REQUIRE(is_valid_ip_address("192.168.1.1"));
    REQUIRE(is_valid_ip_address("192.168.1.0"));
    REQUIRE(is_valid_ip_address("192.168.1.255"));
    REQUIRE(!is_valid_ip_address("192.168.1.256"));
    REQUIRE(!is_valid_ip_address("192.168.1.-1"));
    REQUIRE(!is_valid_ip_address("192.168.1.a"));
}

TEST_CASE("port check","[network]"){
//    REQUIRE(!is_port_open("localhost",65536));
//    REQUIRE(!is_port_open("localhost",0));
}

TEST_CASE("own ip","[network]"){
    REQUIRE(get_own_ip("lo")=="127.0.0.1");
    REQUIRE(get_own_ip("-lo-").has_value()==false);
}

TEST_CASE("get ip by hostname","[network]"){
    REQUIRE(get_ip_by_hostname("127.0.0.1")=="127.0.0.1");
    REQUIRE(get_ip_by_hostname("localhost")=="127.0.0.1");
    REQUIRE(get_ip_by_hostname("localhost2").has_value()==false);
}

TEST_CASE("get ip subnets","[network]"){
    std::map<std::string,std::string> subnets=get_subnets();
    auto lo = subnets.find("lo");
    REQUIRE(lo!=subnets.end());
    REQUIRE((*lo).second=="127.0.0.");
}

TEST_CASE("get interfaces","[network]"){
    std::set<std::string> ifaces = get_ifaces();
    REQUIRE(ifaces.find("lo")!=ifaces.end());
}

TEST_CASE("failed udp stream","[network]"){
    UDPStreamSender sender1("localhost2",8888);
    REQUIRE(!sender1.connect());
    UDPStreamSender sender2("localhost",100000);
    REQUIRE(!sender2.connect());
}

TEST_CASE("udp stream","[network]"){
    std::vector<double> data;
    data.resize(0);
    UDPStreamSender sender("localhost",8888);
    UDPStreamReceiver receiver(8888,4096,1,0,20,[&](std::vector<double>& payload)->void{
        std::cout<<"Received: ["<<payload[0]<<","<<payload[1]<<","<<payload[2]<<","<<payload[3]<<","<<payload[4]<<"]\n";
        data=payload;
    });
    REQUIRE(sender.connect());
    REQUIRE(receiver.connect());
    std::vector<double> payload = {1,2,3,4,5};
    REQUIRE(sender.send(payload));
    sleep(1);
    REQUIRE((data[0]==1 && data[1]==2 && data[2]==3 && data[3]==4 && data[4]==5));
}

}
