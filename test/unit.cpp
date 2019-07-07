#include <catch2/catch.hpp>
#include "connection_flavour.hpp"
#include "unit.hpp"

struct NullFlavour : public dbust::flavours::ConnectionFlavour
{
    int send(dbust::models::Unit& unit, const std::string& request) override
    {
        return error ? -1 : 0;
    }

    int receive(dbust::models::Unit& unit, char* buffer, std::size_t length) override
    {
        return error ? -1 : 0;
    }

    int connect(dbust::models::Unit& unit, const ::addrinfo& addrinfo) override
    {
        return error ? -1 : 0;
    }

    int close(dbust::models::Unit& unit) override
    {
        return error ? -1 : 0;
    }

    int prepare(dbust::models::Unit& unit) override
    {
        return error ? -1 : 0;
    }

    explicit NullFlavour(bool error) : error{ error }
    {
    }

private:
    bool error;
};

TEST_CASE("Unit class behaves properly", "[Unit]")
{
    SECTION("after valid connection instance should have EMPTY state")
    {
        NullFlavour nf(false);
        dbust::models::Unit unit(nf, -1);
        addrinfo addr{};
        unit.connect(addr);
        REQUIRE(unit.get_state() == dbust::models::Unit::State::EMPTY);
    }

    SECTION("after invalid connection instance should have DISCONNECTED state")
    {
        NullFlavour nf(true);
        dbust::models::Unit unit(nf, -1);
        addrinfo addr{};
        unit.connect(addr);
        REQUIRE(unit.get_state() == dbust::models::Unit::State::DICONNECTED);
    }

    SECTION("after valid send instance should have SENDED state")
    {
        NullFlavour nf(false);
        dbust::models::Unit unit(nf, -1);
        std::string rqst = "foobar";
        unit.send(rqst);
        REQUIRE(unit.get_state() == dbust::models::Unit::State::SENDED);
    }

    SECTION("after invalid send instance should have BROKEN state")
    {
        NullFlavour nf(true);
        dbust::models::Unit unit(nf, -1);
        std::string rqst = "foobar";
        unit.send(rqst);
        REQUIRE(unit.get_state() == dbust::models::Unit::State::BROKEN);
    }

    SECTION("after valid receive instance should have EMPTY state")
    {
        NullFlavour nf(false);
        dbust::models::Unit unit(nf, -1);
        char* buff;
        unit.receive(buff, 10);
        REQUIRE(unit.get_state() == dbust::models::Unit::State::EMPTY);
    }

    SECTION("after intialisation instance should have 0 milis after epoch in timepoint member")
    {
        NullFlavour nf(false);
        dbust::models::Unit unit(nf, -1);
        REQUIRE(unit.get_time_point() == std::chrono::system_clock::from_time_t(0));
    }
}