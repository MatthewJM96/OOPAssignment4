#include <vector>
#include <iostream>
#include <string>

enum class HubbleType {
    E0_7,
    S0,
    Sa,
    Sb,
    Sc,
    SBa,
    SBb,
    SBc,
    Irr
};
std::string toString(HubbleType hubbleType) {
    switch (hubbleType) {
    case HubbleType::E0_7:
        return "E0-7";
    case HubbleType::S0:
        return "S0";
    case HubbleType::Sa:
        return "Sa";
    case HubbleType::Sb:
        return "Sb";
    case HubbleType::Sc:
        return "Sc";
    case HubbleType::SBa:
        return "SBa";
    case HubbleType::SBb:
        return "SBb";
    case HubbleType::SBc:
        return "SBc";
    case HubbleType::Irr:
        return "Irr";
    default:
        return "ERROR";
    }
}

class Galaxy;
using Satellite  = Galaxy;
using Satellites = std::vector<Satellite>;

class Galaxy {
public:
    Galaxy() :
        m_name(""),
        m_hubbleType(HubbleType::E0_7),
        m_redshift(1.0),
        m_totalMass(1e9),
        m_stellarMassFract(0.02)
    {}
    // TODO(Matthew): We may need to validate that values provided are within spec's defined ranges.
    Galaxy(std::string name, HubbleType hubbleType, double redshift, double totalMass, double stellarMass) : Galaxy() {
        setName(name);
        setHubbleType(hubbleType);
        setRedshift(redshift);
        setTotalMass(totalMass);
        setStellarMass(stellarMass);
    }
    ~Galaxy() {
        Satellites().swap(m_satellites);
    }

    void print(unsigned char indent = 0);

    void setName(std::string name) {
        m_name = name;
    }
    void setHubbleType(HubbleType hubbleType) {
        m_hubbleType = hubbleType;
    }
    void setRedshift(double redshift) {
        if (redshift < 0.0 || redshift > 10.0) return;
        m_redshift = redshift;
    }
    void setTotalMass(double totalMass) {
        if (totalMass < 1e7 || totalMass > 1e12) return;
        //if (!setStellarMassFract(getStellarMass() / totalMass)) return;
        m_totalMass = totalMass;
    }
    void setStellarMass(double stellarMass) {
        setStellarMassFract(stellarMass / m_totalMass);
    }

    void addSatellite(Satellite satellite) {
        m_satellites.push_back(satellite);
    }

    double getStellarMass() {
        return m_totalMass * m_stellarMassFract;
    }
private:
    bool setStellarMassFract(double stellarMassFract) {
        if (stellarMassFract < 0.0 || stellarMassFract > 0.05) return false;
        m_stellarMassFract = stellarMassFract;
        return true;
    }

    Satellites m_satellites;

    std::string m_name;
    HubbleType  m_hubbleType;
    double      m_redshift;
    double      m_totalMass;
    double      m_stellarMassFract;
};

void Galaxy::print(unsigned char indents /*= 0*/) {
    auto& indent = [indents]() {
        for (size_t i = indents; i > 0; --i) {
            std::cout << "    ";
        }
    };

    indent();
    std::cout << m_name << ":" << std::endl;
    indent();
    std::cout << "    Hubble type: " << toString(m_hubbleType) << std::endl;
    indent();
    std::cout << "    Redshift: " << m_redshift << std::endl;
    indent();
    std::cout << "    Total mass: " << m_totalMass << std::endl;
    indent();
    std::cout << "    Stellar mass fraction: " << m_stellarMassFract << std::endl;

    indent();
    std::cout << "    Satellite count: " << m_satellites.size() << std::endl;
    indent();
    std::cout << "    Satellites: " << std::endl;
    for (Satellite satellite : m_satellites) {
        satellite.print(indents + 2);
    }
}

int main() {
    std::vector<Galaxy> galaxies;

    /// Galaxy with no satellites.
    galaxies.emplace_back("Numero Uno", HubbleType::SBa, 9.0, 1e11, 1e11 * 0.025);

    /// Galaxy with some satellites.
    galaxies.emplace_back("Prime", HubbleType::S0, 2.0, 1e12, 1e12 * 0.04);
    // Prepare & add the satellites.
    Satellite first("Moooooon", HubbleType::E0_7, 2.1, 1e9, 1e9 * 0.01);
    galaxies.back().addSatellite(first);
    Satellite second("Hyperion", HubbleType::Irr, 1.9, 1e8 / 2.0, 1e8 * 0.001);
    galaxies.back().addSatellite(second);
    
    for (Galaxy galaxy : galaxies) {
        galaxy.print();
        std::cout << std::endl << std::endl;
    }

    getchar();
    return 0;
}
