#ifndef TENACITAS_REMOTE_H
#define TENACITAS_REMOTE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

/// \brief master namespace
namespace tenacitas {

/// \brief
namespace remote {

/// \brief
enum class protocol : uint8_t { INET4 = 0, INET6, BLUETOOTH };

/// \brief
enum class communication : uint8_t {
    STEAM = 0,
    DATAGRAM,
    SEQUENCIAL_PACKET,
    RAW
};

/// \brief
enum class address : uint8_t { IPV4 = 0, IPV6 };

/// \brief Amount of bits that an IP V4 may have
struct BitsV4 {
    static const BitsV4 b32;
    static const BitsV4 b31;
    static const BitsV4 b30;
    static const BitsV4 b29;
    static const BitsV4 b28;
    static const BitsV4 b27;
    static const BitsV4 b26;
    static const BitsV4 b25;
    static const BitsV4 b24;
    static const BitsV4 b23;
    static const BitsV4 b22;
    static const BitsV4 b21;
    static const BitsV4 b20;
    static const BitsV4 b19;
    static const BitsV4 b18;
    static const BitsV4 b17;
    static const BitsV4 b16;
    static const BitsV4 b15;
    static const BitsV4 b14;
    static const BitsV4 b13;
    static const BitsV4 b12;
    static const BitsV4 b11;
    static const BitsV4 b10;
    static const BitsV4 b09;
    static const BitsV4 b08;
    static const BitsV4 b07;
    static const BitsV4 b06;
    static const BitsV4 b05;
    static const BitsV4 b04;
    static const BitsV4 b03;
    static const BitsV4 b02;
    static const BitsV4 b01;

    /// \brief Automatic conversion
    inline operator uint8_t() const { return mValue; }

    /// \brief Minus operator
    inline BitsV4 operator-(BitsV4 bits) const {
        return BitsV4(mValue - bits.mValue);
    }

    /// \brief Output operator
    inline friend std::ostream &operator<<(std::ostream &out,
                                           const BitsV4 bits) {
        out << static_cast<uint16_t>(bits.mValue);
        return out;
    }

private:
    BitsV4(uint8_t value)
        : mValue(value) {}

private:
    uint8_t mValue {1};
};

/// \brief IP v4
struct IpV4 {

    IpV4()
        : mNumber(0) {}

    IpV4(const IpV4 &) = default;

    IpV4(IpV4 &&) = default;

    /// \brief Constructor
    inline IpV4(std::string &&value) noexcept(false) {
        number(value.begin(), value.end());
    }

    /// \brief Constructor
    inline IpV4(const std::string &value) noexcept(false) {
        number(value.begin(), value.end());
    }

    inline IpV4(std::string::const_iterator begin,
                std::string::const_iterator end) noexcept(false) {
        number(begin, end);
    }

    /// \brief Constructor
    explicit inline IpV4(uint32_t number)
        : mNumber(number) {}

    /// \brief Output
    inline friend std::ostream &operator<<(std::ostream &out, const IpV4 &ip) {
        out << ip.string();
        return out;
    }

    /// \brief Informs if this IP is the empty, i.e., it was not defined
    inline bool empty() const { return (mNumber == 0); }

    /// \brief Calculates the mininum e maximum number IP given a number of bits
    inline static std::pair<IpV4, IpV4> ipMinMax(BitsV4 bitsForIp) {
        return {IpV4 {1}, IpV4 {static_cast<uint32_t>(std::pow(
                                    2, static_cast<float>(bitsForIp))) -
                                2}};
    }

    IpV4 &operator=(const IpV4 &) = default;

    IpV4 &operator=(IpV4 &&) = default;

    /// \brief Equal-to operator
    inline bool operator==(const IpV4 &ip) const {
        return mNumber == ip.mNumber;
    }

    /// \brief Not-equal-to operator
    inline bool operator!=(const IpV4 &ip) const {
        return mNumber != ip.mNumber;
    }

    /// \brief Less-than operator
    inline bool operator<(const IpV4 &ip) const { return mNumber < ip.mNumber; }

    /// \brief Less-than-or-equal-to operator
    inline bool operator<=(const IpV4 &ip) const {
        return mNumber <= ip.mNumber;
    }

    /// \brief Adds an IP to this, creating a new one
    inline IpV4 operator+(const IpV4 &ip) const {
        return IpV4 {mNumber + ip.mNumber};
    }

    /// \brief OR a IP to this, creating a new one
    inline IpV4 operator|(const IpV4 &ip) const {
        //        Number number = mNumber;
        return IpV4 {mNumber | ip.mNumber};
    }

    /// \brief Pre increment
    inline IpV4 &operator++() {
        ++mNumber;
        return *this;
    }

    /// \brief Pos increment
    inline IpV4 &operator++(int) {
        mNumber++;
        return *this;
    }

    /// \brief Pre decrement
    inline IpV4 &operator--() {
        --mNumber;
        return *this;
    }

    /// \brief Pos decrement
    inline IpV4 &operator--(int) {
        mNumber--;
        return *this;
    }

    /// \brief Automatic conversion
    operator uint32_t() const { return mNumber; }

    /// \brief Clear the value of the IP
    inline void clear() { mNumber = 0; }

    /// \brief Informs if this IP contains a string
    inline bool contains(const std::string &str) const {
        return (string().find(str) != std::string::npos);
    }

    /// \brief Conversion to string representation
    std::string string() const {
        if (mNumber == 0) {
            return "0.0.0.0";
        }
        std::stringstream stream;
        stream << std::to_string((mNumber >> 24) & 0xFF) << "."
               << std::to_string((mNumber >> 16) & 0xFF) << "."
               << std::to_string((mNumber >> 8) & 0xFF) << "."
               << (mNumber & 0xFF);
        return stream.str();
    }

private:
    /// \brief Conversion to number from a string
    template <typename t_iterator>
    void number(t_iterator b, t_iterator e) {

        mNumber = 0;
        static const std::regex regex("([0-9]{1,3})");

        std::sregex_iterator begin(b, e, regex);
        std::sregex_iterator end;

        if (std::distance(begin, end) != 4) {
            return;
        }

        uint8_t count = 3;

        for (std::sregex_iterator ite = begin; ite != end; ++ite) {
            mNumber |= (static_cast<uint32_t>(std::stoi(ite->str()))
                        << static_cast<uint32_t>(count-- * 8));
        }
    }

private:
    /// \brief Numeric representation of the IP
    uint32_t mNumber {0};
};

/// \brief
template <address add>
struct endpoint;

/// \brief
template <>
struct endpoint<address::IPV4> {

    IpV4 ip;
};

struct incoming {
    template <typename t_string>
    incoming(t_string &&p_str)
        : msg(std::forward(p_str)) {}

    friend std::ostream &operator<<(std ::ostream &p_out,
                                    const incoming &p_incoming) {
        p_out << p_incoming.msg;
        return p_out
    }

    std::string msg;
};

} // namespace remote
} // namespace tenacitas

#endif
