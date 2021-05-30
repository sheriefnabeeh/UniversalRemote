#include <String>
#include <vector>
#include <array>
#define REMOTESIZELIMIT 10
#define PROTOCOLINDEX 1
#define ADDRESSINDEX 2
#define COMMANDINDEX 3

enum class PROTOCOL
{
    INVALID = 0,
    BOSE = 1,
    Denon = 2,
    JVC = 3,
    KASEIKYO = 4,
    LEGALPOWER = 5,
    LG = 6,
    MAGI = 7,
    NEC = 8,
    RC5 = 9,
    RC6 = 10,
    SAMSUNG = 11,
    SHARP = 12,
    SONY = 13
};
struct IRData
{
    PROTOCOL protocol; ///< UNKNOWN, NEC, SONY, RC5, ...
    uint16_t address;  ///< Decoded address
    uint16_t command;  ///< Decoded command};
};
enum class SIGNALTRANSLATION
{
    UP = 0,
    DOWN = 1,
    VOLUP = 2,
    VOLDOWN = 3,
    ENTER = 4,
    BACK = 5,
    CHANGE = 6,
    MUTE = 7
};

struct RemoteData
{
private:
    std::string name;

public:
    std::array<IRData, 10> signals = {};

    void setName(std::string tobe)
    {
        if (!tobe.empty())
            name = tobe;
    }
    std::string getName()
    {
        return name;
    }
};

template <typename PROTOCOL>
auto as_integer(PROTOCOL const value)
    -> typename std::underlying_type<PROTOCOL>::type
{
    return static_cast<typename std::underlying_type<PROTOCOL>::type>(value);
};

class EEWrapper
{
public:
    EEWrapper() = default;
    RemoteData initRemotes();
    int writeSignal(IRData data);
    int writeRemote(const IRData irList[], int size);
    void endRemoteSeries();
    uint16_t formatMemory();
    uint16_t getRemoteSignalCounter();
    uint16_t getUniversalEECounter();
    std::string protocol_to_string(const PROTOCOL &);
    std::vector<RemoteData> fetchRemotes();
    PROTOCOL protocolmap[14] = {PROTOCOL::INVALID,
                                PROTOCOL::BOSE,
                                PROTOCOL::Denon,
                                PROTOCOL::JVC,
                                PROTOCOL::KASEIKYO,
                                PROTOCOL::LEGALPOWER,
                                PROTOCOL::LG,
                                PROTOCOL::MAGI,
                                PROTOCOL::NEC,
                                PROTOCOL::RC5,
                                PROTOCOL::RC6,
                                PROTOCOL::SAMSUNG,
                                PROTOCOL::SHARP,
                                PROTOCOL::SONY};

    SIGNALTRANSLATION signalEnums[8] = {SIGNALTRANSLATION::UP,
                                        SIGNALTRANSLATION::DOWN,
                                        SIGNALTRANSLATION::VOLUP,
                                        SIGNALTRANSLATION::VOLDOWN,
                                        SIGNALTRANSLATION::ENTER,
                                        SIGNALTRANSLATION::BACK,
                                        SIGNALTRANSLATION::CHANGE,
                                        SIGNALTRANSLATION::MUTE};

private:
    int currentPointer = 0x01;
    uint16_t Status;
    uint16_t dataRead;
    uint16_t EEAddress{0x01};
    int remoteSignalsCounter{0};
};
