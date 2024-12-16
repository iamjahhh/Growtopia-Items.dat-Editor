#pragma once
#include <string>
#include <vector>

class BinaryReader
{
public:
	BinaryReader(uint8_t* data)
		: m_data(data), m_pos(0) {}
	BinaryReader(const std::vector<uint8_t>& data) : m_pos(0) {
		auto alloc = data.size();
		m_data = (uint8_t*)std::malloc(alloc);
		std::memcpy(m_data, data.data(), alloc);
	}
	~BinaryReader() {
		std::free(m_data);
	}

	template<typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
	T read() {
		T val;
		std::memcpy(&val, this->m_data + this->m_pos, sizeof(T));
		this->m_pos += sizeof(T);
		return val;
	}
	std::string read_string() {
		uint16_t str_len = *(int16_t*)&this->m_data[this->m_pos];
		std::string val = std::string(reinterpret_cast<char*>(this->m_data + this->m_pos + 2), str_len);
		this->m_pos += sizeof(uint16_t) + str_len;
		return val;
	}

	uint8_t* getData() { return m_data; };
	uint32_t getPos() { return m_pos;  };

	void addPos(uint32_t c) { m_pos += c; }

	void skip(uint32_t len) {
		this->m_pos += len;
	}
private:
	uint8_t* m_data;
	uint32_t m_pos;
};