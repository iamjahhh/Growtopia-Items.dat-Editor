#pragma once
#include "ItemInfo.hh"

class ItemDatabase
{
public:
    bool serialize();
    void set_custom_items(bool);
    void encode(bool);

    uint32_t hash(const void* data, std::size_t data_len) {
        if (!data) return 0;
        uint32_t hash = 0x55555555;

        for (std::size_t i = 0; i < data_len; ++i)
            hash = (hash >> 27) + (hash << 5) + static_cast<const uint8_t*>(data)[i];

        return hash;
    }
    std::vector<std::string> explode(const std::string& delimeter, const std::string& str) {
        std::vector<std::string> v;
        int strLength = str.length(), delimeterLength = delimeter.length();
        if (delimeterLength == 0) return v;
        int i = 0, k = 0;
        while (i < strLength) {
            int j = 0;
            while (i + j < strLength && j < delimeterLength && str[i + j] == delimeter[j]) j++;
            if (j == delimeterLength) {
                v.push_back(str.substr(k, i - k));
                i += delimeterLength;
                k = i;
            }
            else i++;
        }
        v.push_back(str.substr(k, i - k));
        return v;
    }
    void remove_word(std::string& line, const std::string& word) {
        auto n = line.find(word);
        if (n != std::string::npos) {
            line.erase(n, word.length());
        }
    }

    static ItemDatabase& get() { static ItemDatabase ret; return ret; }

private:
    std::size_t m_size;
    uint8_t* m_data;

    uint32_t m_hash;
    uint16_t m_version;
    uint32_t m_item_count;

    TankUpdatePacket* m_packet;

private:
    std::vector<ItemInfo> m_items;
};