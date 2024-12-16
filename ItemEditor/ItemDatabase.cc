 #include "ItemDatabase.hh"

#include <filesystem>
#include <sstream>
#include <fstream>

#include "ItemDatabase.hh"
#include "BinaryReader.hh"
#include "json.hpp"

std::string secret = "PBG892FXX982ABC*";

bool ItemDatabase::serialize() {
    std::ifstream file("items.dat", std::ios::binary | std::ios::ate);
    
    int size = file.tellg();
    if (size == -1)  return false;

    char* data = new char[size];
    file.seekg(0, std::ios::beg);

    if (file.read((char*)(data), size))
    {
        std::cout << "decoding..." << std::endl;
    }
    else {
        std::cout << "decoding failed." << std::endl;
        return false;
    }

    int memPos = 0;
    memcpy(&m_version, data + memPos, 2);
    memPos += 2;
    memcpy(&m_item_count, data + memPos, 4);
    memPos += 4;

    m_items.reserve(m_item_count);
    for (int i = 0; i < m_item_count; i++) {
        m_items.push_back({});

        memcpy(&m_items[i].m_id, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_editable_type, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_item_category, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_item_type, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_marterial, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);

        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_name += data[memPos] ^ (secret[(j + m_items[i].m_id) % secret.length()]);
                memPos++;
            }
            
            std::cout << m_items[i].m_id << ": " << m_items[i].m_name << std::endl;
        }
        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_texture += data[memPos];
                memPos++;
            }
        }

        memcpy(&m_items[i].m_texture_hash, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_visual_effect, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_val1, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_texture_x, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_texture_y, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_spread_type, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_is_stripey_wallpaper, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_collision_type, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);

        uint8_t breakHits = m_items[i].m_break_hits / 6;
        memcpy(&breakHits, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);

        memcpy(&m_items[i].m_reset_time, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_clothing_type, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_rarity, data + memPos, sizeof(uint16_t));
        memPos += sizeof(uint16_t);
        memcpy(&m_items[i].m_max_amount, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);

        memcpy(&m_items[i].m_max_amount, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);

        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_extra_file += data[memPos];
                memPos++;
            }
        }

        memcpy(&m_items[i].m_extra_file_hash, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_audio_volume, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);

        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_pet_name += data[memPos];
                memPos++;
            }
        }
        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_pet_prefix += data[memPos];
                memPos++;
            }
        }
        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_pet_suffix += data[memPos];
                memPos++;
            }
        }
        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_pet_ability += data[memPos];
                memPos++;
            }
        }

        memcpy(&m_items[i].m_seed_base, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_seed_overlay, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_tree_base, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_tree_leaves, data + memPos, sizeof(uint8_t));
        memPos += sizeof(uint8_t);
        memcpy(&m_items[i].m_seed_color, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_seed_overlay_color, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_ingredient, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_grow_time, data + memPos, sizeof(uint32_t));
        memPos += sizeof(uint32_t);
        memcpy(&m_items[i].m_val2, data + memPos, sizeof(uint16_t));
        memPos += sizeof(uint16_t);
        memcpy(&m_items[i].m_rayman, data + memPos, sizeof(uint16_t));
        memPos += sizeof(uint16_t);

        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_extra_options += data[memPos];
                memPos++;
            }
        }
        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_texture2 += data[memPos];
                memPos++;
            }
        }
        {
            int16_t strLen = *(int16_t*)&data[memPos];
            memPos += 2;
            for (int j = 0; j < strLen; j++) {
                m_items[i].m_extra_options2 += data[memPos];
                memPos++;
            }
        }

        memPos += 80;

        if (m_version >= 11) {
            {
                int16_t strLen = *(int16_t*)&data[memPos];
                memPos += 2;
                for (int j = 0; j < strLen; j++) {
                    m_items[i].m_punch_options += data[memPos];
                    memPos++;
                }
            }
        }

        if (m_version >= 12) {
            memcpy(&m_items[i].m_val3, data + memPos, sizeof(uint32_t));
            memPos += sizeof(uint32_t);

            for (auto index = 0; index < 9; index++) {
                memcpy(&m_items[i].m_bodypart[index], data + memPos, sizeof(uint8_t));
                memPos += sizeof(uint8_t);
            }
        }

        if (m_version >= 13) {
            memcpy(&m_items[i].m_val4, data + memPos, sizeof(uint32_t));
            memPos += sizeof(uint32_t);
        }

        if (m_version >= 14) {
            memcpy(&m_items[i].m_val5, data + memPos, sizeof(uint32_t));
            memPos += sizeof(uint32_t);
        }

        if (m_version >= 15) {
            m_items[i].m_can_player_sit = data[memPos];
            memPos++;
            m_items[i].m_sit_offset_x = *(int*)(data + memPos);
            memPos += 4;
            m_items[i].m_sit_offset_y = *(int*)(data + memPos);
            memPos += 4;
            m_items[i].m_sit_overlay_x = *(int*)(data + memPos);
            memPos += 4;
            m_items[i].m_sit_overlay_y = *(int*)(data + memPos);
            memPos += 4;
            m_items[i].m_sit_overlay_offset_x = *(int*)(data + memPos);
            memPos += 4;
            m_items[i].m_sit_overlay_offset_y = *(int*)(data + memPos);
            memPos += 4;

            {
                int16_t strLen = *(int16_t*)&data[memPos];
                memPos += 2;
                for (int j = 0; j < strLen; j++) {
                    m_items[i].m_sit_file += data[memPos];
                    memPos++;
                }
            }
        }

        if (m_version >= 16) {
            {
                int16_t strLen = *(int16_t*)&data[memPos];
                memPos += 2;
                for (int j = 0; j < strLen; j++) {
                    m_items[i].m_unk_eh += data[memPos];
                    memPos++;
                }
            }
        }
    }

    return true;
}
void ItemDatabase::set_custom_items(bool ios) {
    
}
void ItemDatabase::encode(bool ios) {
    std::string path;
    if (ios) path = "iositemsz";
    else path = "itemsz";

    std::ofstream dat("E:\\Growtopia\\Project AGAGA\\x64\\Release\\cache\\" + path + ".dat", std::ios::binary);
    
    uint32_t pos = 0;
    size_t size = m_items.size() * sizeof(ItemInfo) * 10;
    uint8_t* data = static_cast<uint8_t*>(malloc(size));

    auto write_string = [&](const std::string& str)
        {
            uint16_t strsize = static_cast<uint16_t>(str.size());
            memcpy(data + pos, &strsize, 2);
            pos += 2;
            for (int i = 0; i < strsize; ++i)
                data[pos++] = static_cast<uint8_t>(str[i]);
        };

    auto write_string_encrypted = [&](int id, const std::string& str)
        {
            uint16_t strsize = static_cast<uint16_t>(str.size());
            memcpy(data + pos, &strsize, 2);
            pos += 2;

            std::string encrypted = ItemInfo::cypher(str, id);

            for (int i = 0; i < strsize; ++i)
                data[pos++] = static_cast<uint8_t>(encrypted[i]);
        };

    memset(data, 0, size);
    memcpy(data + pos, &m_version, 2);
    memcpy(data + pos + 2, &m_item_count, 4);
    pos += 6;

    for (ItemInfo item : m_items)
    {
        uint32_t id = item.m_id;

        memcpy(data + pos, &id, 4);
        pos += 4;
        memcpy(data + pos, &item.m_editable_type, 1);
        pos += 1;
        memcpy(data + pos, &item.m_item_category, 1);
        pos += 1;
        memcpy(data + pos, &item.m_item_type, 1);
        pos += 1;
        memcpy(data + pos, &item.m_marterial, 1);
        pos += 1;

        write_string_encrypted(item.m_id, item.m_name);
        write_string(item.m_texture);

        memcpy(data + pos, &item.m_texture_hash, 4);
        pos += 4;
        memcpy(data + pos, &item.m_visual_effect, 1);
        pos += 1;
        memcpy(data + pos, &item.m_val1, 4);
        pos += 4;
        memcpy(data + pos, &item.m_texture_x, 1);
        pos += 1;
        memcpy(data + pos, &item.m_texture_y, 1);
        pos += 1;
        memcpy(data + pos, &item.m_spread_type, 1);
        pos += 1;
        memcpy(data + pos, &item.m_is_stripey_wallpaper, 1);
        pos += 1;
        memcpy(data + pos, &item.m_collision_type, 1);
        pos += 1;

        uint8_t breakHits = item.m_break_hits * 6;
        memcpy(data + pos, &breakHits, 1);
        pos += 1;

        memcpy(data + pos, &item.m_reset_time, 4);
        pos += 4;
        memcpy(data + pos, &item.m_clothing_type, 1);
        pos += 1;
        memcpy(data + pos, &item.m_rarity, 2);
        pos += 2;
        memcpy(data + pos, &item.m_max_amount, 1);
        pos += 1;

        write_string(item.m_extra_file);

        memcpy(data + pos, &item.m_extra_file_hash, 4);
        pos += 4;
        memcpy(data + pos, &item.m_audio_volume, 4);
        pos += 4;

        write_string(item.m_pet_name);
        write_string(item.m_pet_prefix);
        write_string(item.m_pet_suffix);
        write_string(item.m_pet_ability);

        memcpy(data + pos, &item.m_seed_base, 1);
        pos += 1;
        memcpy(data + pos, &item.m_seed_overlay, 1);
        pos += 1;
        memcpy(data + pos, &item.m_tree_base, 1);
        pos += 1;
        memcpy(data + pos, &item.m_tree_leaves, 1);
        pos += 1;
        memcpy(data + pos, &item.m_seed_color, 4);
        pos += 4;
        memcpy(data + pos, &item.m_seed_overlay_color, 4);
        pos += 4;
        memcpy(data + pos, &item.m_ingredient, 4);
        pos += 4;
        memcpy(data + pos, &item.m_grow_time, 4);
        pos += 4;
        memcpy(data + pos, &item.m_val2, 2);
        pos += 2;
        memcpy(data + pos, &item.m_rayman, 2);
        pos += 2;

        write_string(item.m_extra_options);
        write_string(item.m_texture2);
        write_string(item.m_extra_options2);

        memcpy(data + pos, item.m_reserved, 80);
        pos += 80;

        if (m_version >= 11)
        {
            write_string(item.m_punch_options);
        }

        if (m_version >= 12)
        {
            memcpy(data + pos, &item.m_val3, 4);
            pos += 4;
            memcpy(data + pos, item.m_bodypart, 9);
            pos += 9;
        }

        if (m_version >= 13)
        {
            memcpy(data + pos, &item.m_val4, 4);
            pos += 4;
        }

        if (m_version >= 14)
        {
            memcpy(data + pos, &item.m_val5, 4);
            pos += 4;
        }

        if (m_version >= 15)
        {
            memcpy(data + pos, &item.m_can_player_sit, sizeof(uint8_t));
            pos += sizeof(uint8_t);
            memcpy(data + pos, &item.m_sit_offset_x, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(data + pos, &item.m_sit_offset_y, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(data + pos, &item.m_sit_overlay_x, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(data + pos, &item.m_sit_overlay_y, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(data + pos, &item.m_sit_overlay_offset_x, sizeof(uint32_t));
            pos += sizeof(uint32_t);
            memcpy(data + pos, &item.m_sit_overlay_offset_y, sizeof(uint32_t));
            pos += sizeof(uint32_t);

            write_string(item.m_sit_file);
        }

        if (m_version >= 16)
        {
            write_string(item.m_unk_eh);
        }
    }

    dat.write(reinterpret_cast<char*>(data), pos);
    dat.close();
    free(data);
}