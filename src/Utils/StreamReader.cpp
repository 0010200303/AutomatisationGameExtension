#include "StreamReader.hpp"

void StreamReader::set_data(PackedByteArray& _data) {
    data = _data;
    index = 0;
}

PackedByteArray StreamReader::get_data() const {
    return data;
}

void StreamReader::clear() {
    data.clear();
    index = 0;
}

int StreamReader::read_int() {
    int value = data.decode_s32(index);
    index += 4;
    return value;
}

unsigned int StreamReader::read_uint() {
    unsigned int value = data.decode_u32(index);
    index += 4;
    return value;
}

float StreamReader::read_float() {
    float value = data.decode_float(index);
    index += 4;
    return value;
}

uint8_t StreamReader::read_byte() {
    uint8_t value = (uint8_t)data.decode_s8(index);
    index += 1;
    return value;
}

bool StreamReader::read_bool() {
    return (bool)read_byte();
}

String StreamReader::read_string() {
    int length = read_int();
    String value;

    for (int i = 0; i < length; i++)
        value += (char)read_byte();

    return value;
}

Vector3 StreamReader::read_vector3() {
    return Vector3(read_float(), read_float(), read_float());
}

PackedStringArray StreamReader::read_array_string() {
    int size = read_int();
    PackedStringArray value;
    value.resize(size);

    for (int i = 0; i < size; i++)
        value[i] = read_string();
    
    return value;
}

std::vector<Vector3> StreamReader::read_array_vector3() {
    int size = read_int();
    std::vector<Vector3> value;
    value.resize(size);

    for (int i = 0; i < size; i++)
        value[i] = read_vector3();

    return value;
}