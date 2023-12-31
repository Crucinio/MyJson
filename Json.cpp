
#include "Json.h"
#include<stack>
#include<stdexcept>
#include<fstream>

enum expecting { key, value, number, object, array, object_array, number_array, boolean_array, unidentified, unidentified_array };

JSONObject::JSONObject(std::string& source) // ???
{
    size_t begin = source.find_first_of('{', 0);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expecting current = key;
    while (has_next_key(source, ++begin)) {
        
        // pick the key
        k = pick_val(source, begin);
        begin++;

        // identify target field using control symbols \", {, [ or T&F
        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == 't' && source.size() - begin - 3 > 0 && source.substr(begin, 4) == "true") {
                current = key;
                this->key_to_bool[k] = true;
                begin += 3;
            }
            else if (source[begin] == 'f' && source.size() - begin - 4 > 0 && source.substr(begin, 5) == "false") {
                current = key;
                this->key_to_bool[k] = false;
                begin += 4;
            }
            else if ((source[begin] >= '0' && source[begin] <= '9') || (source[begin] == '-' && source.size() - 1 > begin && source[begin + 1] >= '0' && source[begin + 1] <= '9')) {
                current = number;
            }
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                throw new std::invalid_argument("Key does not correspond to anything! Key = " + k);

            break;
        }

        // if the target is an array, determine the type of the array using control symbols \", {, T&F
        if (current == unidentified_array) {
            size_t tmp = begin;
            for (; tmp < source.size(); tmp++) {
                if (source[tmp] == '\"' || source[tmp] == ']')
                    current = array;
                else if ((source[tmp] == 't' && source.size() - tmp - 3 > 0 && source.substr(tmp, 4) == "true")
                    || (source[tmp] == 'f' && source.size() - tmp - 4 > 0 && source.substr(tmp, 5) == "false")) {
                    current = boolean_array;
                }
                else if (source[tmp] == '{')
                    current = object_array;
                else if ((source[tmp] == '-' && source[tmp + 1] >= '0' && source[tmp + 1] <= '9') || (source[tmp] >= '0' && source[tmp] <= '9'))
                    current = number_array;
                else
                    continue;

                if (tmp == source.size())
                    throw new std::invalid_argument("Array is not closed! Key = " + k);

                break;
            }
        }

        // after identifying the target, pick it and insert/rewrite
        if (current == value) {
            this->key_to_value[k] = pick_val(source, begin);
        }
        else if (current == number) {
            this->key_to_num[k] = pick_num(source, begin);
        }
        else if (current == object) {
            JSONObject tmp(source, begin);
            this->key_to_object[k] = tmp;
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_list(source, begin);
            this->key_to_value_list[k] = tmp;
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_list(source, begin);
            this->key_to_object_list[k] = tmp;
        }
        else if (current == number_array) {
            std::list<long long> tmp = pick_num_list(source, begin);
            this->key_to_num_list[k] = tmp;
        }
        else if (current == boolean_array) {
            std::list<bool> tmp = pick_bool_list(source, begin);
            this->key_to_boolean_list[k] = tmp;
        }
    }
}

JSONObject::JSONObject(std::string& source, size_t& begin) // ???
{
    begin = source.find_first_of('{', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expecting current = key;
    while (has_next_key(source, ++begin)) {

        // pick the key
        k = pick_val(source, begin);
        begin++;

        // identify target field using control symbols \", { or [
        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == 't' && source.size() - begin - 3 > 0 && source.substr(begin, 4) == "true") {
                current = key;
                this->key_to_bool[k] = true;
                begin += 3;
            }
            else if (source[begin] == 'f' && source.size() - begin - 4 > 0 && source.substr(begin, 5) == "false") {
                current = key;
                this->key_to_bool[k] = false;
                begin += 4;
            }
            else if ((source[begin] >= '0' && source[begin] <= '9') || (source[begin] == '-' && source.size() - 1 > begin && source[begin + 1] >= '0' && source[begin + 1] <= '9')) {
                current = number;
            }
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                throw new std::invalid_argument("Key does not correspond to anything! Key = " + k);

            break;
        }

        // if the target is an array, determine the type of the array using control symbols \", {, T&F
        if (current == unidentified_array) {
            size_t tmp = begin;
            for (; tmp < source.size(); tmp++) {
                if (source[tmp] == '\"' || source[tmp] == ']')
                    current = array;
                else if ((source[tmp] == 't' && source.size() - tmp - 3 > 0 && source.substr(tmp, 4) == "true")
                    || (source[tmp] == 'f' && source.size() - tmp - 4 > 0 && source.substr(tmp, 5) == "false")) {
                    current = boolean_array;
                }
                else if (source[tmp] == '{')
                    current = object_array;
                else if ((source[tmp] == '-' && source[tmp + 1] >= '0' && source[tmp + 1] <= '9') || (source[tmp] >= '0' && source[tmp] <= '9'))
                    current = number_array;
                else
                    continue;

                if (tmp == source.size())
                    throw new std::invalid_argument("Array is not closed! Key = " + k);

                break;
            }
        }

        // after identifying the target, pick it and insert/rewrite
        if (current == value) {
            this->key_to_value[k] = pick_val(source, begin);
        }
        else if (current == number) {
            this->key_to_num[k] = pick_num(source, begin);
        }
        else if (current == object) {
            JSONObject tmp(source, begin);
            this->key_to_object[k] = tmp;
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_list(source, begin);
            this->key_to_value_list[k] = tmp;
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_list(source, begin);
            this->key_to_object_list[k] = tmp;
        }
        else if (current == number_array) {
            std::list<long long> tmp = pick_num_list(source, begin);
            this->key_to_num_list[k] = tmp;
        }
        else if (current == boolean_array) {
            std::list<bool> tmp = pick_bool_list(source, begin);
            this->key_to_boolean_list[k] = tmp;
        }
    }
}

int JSONObject::find_block_end_array(const std::string& source, int curpos)
{
    int pos = curpos + 1;
    bool counts = true;
    while (pos < source.size()) {
        if (source[pos] == '\"') {
            if (source[pos - 1] != '\\')
                counts = (counts + 1) % 2;
        }
        else if (counts && source[pos] == ']')
            return pos;

        pos++;
    }

    return -1;
}

std::string JSONObject::pick_val(std::string& source, size_t& begin) // (OK)
{
    if (begin + 1 >= source.size())
        throw new std::invalid_argument("begin");

    begin = source.find_first_of('\"', begin);
    if (begin + 1 > source.size() || begin == -1)
        throw new std::invalid_argument("source");

    begin++;
    std::stack<char> pile;
    while (begin < source.size()) {
        char cur = source[begin];
        if (cur == '\\') {
            if (begin + 1 < source.size()) {
                char next = source[begin + 1];
                if (next == '\\')
                    pile.push(cur);
                else if (next == '"')
                    pile.push('\"');
                else if (next == 'b')
                    pile.pop();
                else if (next == 't')
                    pile.push('\t');

                begin += 2;
            }
            else {
                throw new std::invalid_argument("source");
            }
        }
        else if (cur == '\"')
            break;
        else {
            pile.push(cur);
            begin++;
        }
    }

    if (begin >= source.size())
        throw new std::invalid_argument("source");

    std::string res(pile.size(), '0');
    while (pile.size() > 0) {
        res[pile.size() - 1] = pile.top();
        pile.pop();
    }

    return res;
}

long long JSONObject::pick_num(std::string& source, size_t& begin)
{
    long long num = 1;
    if (source[begin] == '-') {
        if (begin < source.size() - 1 && source[begin + 1] >= '0' && source[begin + 1] <= '9') {
            num *= -1;
            begin++;
        }
        else
            throw new std::invalid_argument("invalid number at " + std::to_string(begin));
    }

    num *= source[begin] - '0';
    begin++;

    while (begin < source.size() && source[begin] == '0')
        begin++;

    while (source[begin] >= '0' && source[begin] <= '9') {
        begin++;
        num *= 10;
        num += source[begin] - '0';
    }

    return num;
}

std::list<std::string> JSONObject::pick_val_list(std::string& source, size_t& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<std::string> values;
    while (begin < end && has_next_key(source, begin))
    {
        std::string value = pick_val(source, begin);
        values.push_back(value);
        begin++;
    }

    return values;
}

std::list<JSONObject> JSONObject::pick_obj_list(std::string& source, size_t& begin)
{
    size_t pos1 = source.find_first_of('{', begin);
    size_t pos2 = source.find_first_of(']', begin);

    if (pos2 == -1)
        throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

    std::list<JSONObject> tmp;
    if (pos1 == -1 || pos2 < pos1) {
        begin = pos2;
        return tmp;
    }

    while (pos1 != -1) {

        JSONObject obj(source, pos1);
        begin = source.find_first_of(']', pos1);
        if (begin == -1)
            throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

        tmp.push_back(obj);
        pos1++;

        pos1 = source.find_first_of('{', pos1);

        if (pos1 > begin) {
            begin = pos2;
            break;
        }
    }
    return tmp;
}

std::list<bool> JSONObject::pick_bool_list(std::string& source, size_t& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<bool> bools;
    while (begin < end)
    {
        if (source[begin] == 't' && source.size() - begin - 3 > 0 && source.substr(begin, 4) == "true") {
            bools.push_back(true);
            begin += 3;
        }
        else if (source[begin] == 'f' && source.size() - begin - 4 > 0 && source.substr(begin, 5) == "false") {
            bools.push_back(false);
            begin += 4;
        }
        
        begin++;
    }

    return bools;
}

std::list<long long> JSONObject::pick_num_list(std::string& source, size_t& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    ++begin;

    std::list<long long> nums;
    while (begin < end)
    {
        if ((source[begin] >= '0' && source[begin] <= '9') || source[begin] == '-') {
            long long num = 1;
            if (source[begin] == '-') {
                if (begin < source.size() - 1 && source[begin + 1] >= '0' && source[begin + 1] <= '9') {
                    num *= -1;
                    begin++;
                }
                else
                    throw new std::invalid_argument("invalid number at " + std::to_string(begin));
            }

            num *= source[begin] - '0';
            begin++;

            while (begin < source.size() && source[begin] == '0')
                begin++;

            while (begin < end && source[begin] >= '0' && source[begin] <= '9') {
                begin++;

                num *= 10;
                num += source[begin] - '0';
            }
        }
        else if (source[begin] == ' ' || source[begin] == ':' || source[begin] == ';' || source[begin] == ',') {
            begin++;
            continue;
        }
        else if (source[begin] == ']')
            return nums;
        else
            throw new std::invalid_argument("invalid array at " + std::to_string(begin));

        begin++;
    }

    return nums;
}

bool JSONObject::has_next_key(std::string& source, size_t& pos)
{
    if (pos == source.size())
        return false;
    size_t end = source.find_first_of('}', pos);
    if (end == -1)
        throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

    size_t begin = source.find_first_of('\"', pos);
    if (begin == -1 || begin > end) {
        pos = end;
        return false;
    }

    begin++;
    while (begin < source.size()) {
        if (source[begin] == '\"' && source[begin - 1] != '\\') {
            if (source.find_first_of('}', begin) == -1)
                throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

            return true;
        }

        begin++;
    }

    return false;
}

JSONObject& JSONObject::get_object(std::string key)
{
    return key_to_object.at(key);
}

std::list<JSONObject>& JSONObject::get_obj_list(std::string key)
{
    return key_to_object_list.at(key);
}

std::list<std::string>& JSONObject::get_val_list(std::string key)
{
    return key_to_value_list.at(key);
}

std::list<bool>& JSONObject::get_bool_list(std::string key)
{
    return key_to_boolean_list.at(key);
}

std::list<long long>& JSONObject::get_num_list(std::string key)
{
    return key_to_num_list.at(key);
}

std::string& JSONObject::get_value(std::string key)
{
    return key_to_value.at(key);
}

bool JSONObject::get_bool(std::string key)
{
    return key_to_bool.at(key);
}

long long JSONObject::get_num(std::string key)
{
    return key_to_num.at(key);
}

const std::unordered_map<std::string, JSONObject>& JSONObject::get_name_to_object() const
{
    return key_to_object;
}

const std::unordered_map<std::string, std::list<JSONObject> >& JSONObject::get_name_to_objects() const
{
    return key_to_object_list;
}

const std::unordered_map<std::string, std::list<std::string> >& JSONObject::get_name_to_values() const
{
    return key_to_value_list;
}

const std::unordered_map<std::string, std::list<long long>>& JSONObject::get_name_to_nums() const
{
    return key_to_num_list;
}

const std::unordered_map<std::string, std::list<bool>>& JSONObject::get_name_to_bools() const
{
    return key_to_boolean_list;
}

const std::unordered_map<std::string, std::string>& JSONObject::get_name_to_value() const
{
    return key_to_value;
}

const std::unordered_map<std::string, long long>& JSONObject::get_name_to_num() const
{
    return key_to_num;
}

const std::unordered_map<std::string, bool>& JSONObject::get_name_to_bool() const
{
    return key_to_bool;
}

bool JSONObject::is_in_values(std::string key) const
{
    if (key_to_value.find(key) == key_to_value.end())
        return false;

    return true;
}

bool JSONObject::is_in_objects(std::string key) const
{
    if (key_to_object.find(key) == key_to_object.end())
        return false;

    return true;
}

bool JSONObject::is_in_bools(std::string key) const
{
    if (key_to_bool.find(key) == key_to_bool.end())
        return false;

    return true;
}

bool JSONObject::is_in_nums(std::string key) const
{
    if (key_to_num.find(key) == key_to_num.end())
        return false;

    return true;
}

bool JSONObject::is_in_val_lists(std::string key) const
{
    if (key_to_value_list.find(key) == key_to_value_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_obj_lists(std::string key) const
{
    if (key_to_object_list.find(key) == key_to_object_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_bool_lists(std::string key) const
{
    if (key_to_boolean_list.find(key) == key_to_boolean_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_num_lists(std::string key) const
{
    if (key_to_num_list.find(key) == key_to_num_list.end())
        return false;

    return true;
}

bool JSONObject::empty_values() const
{
    return key_to_value.empty();
}

bool JSONObject::empty_objects() const
{
    return key_to_object.empty();
}

bool JSONObject::empty_nums() const
{
    return key_to_num.empty();
}

bool JSONObject::empty_bools() const
{
    return key_to_bool.empty();
}

bool JSONObject::empty_val_lists() const
{
    return key_to_value_list.empty();
}

bool JSONObject::empty_obj_lists() const
{
    return key_to_object_list.empty();
}

bool JSONObject::empty_num_lists() const
{
    return key_to_num_list.empty();
}

bool JSONObject::empty_bool_lists() const
{
    return key_to_boolean_list.empty();
}

bool JSONObject::empty() const
{
    return key_to_value.size() == 0 &&
        key_to_object.size() == 0 &&
        key_to_value_list.size() == 0 &&
        key_to_object_list.size() == 0;
}

void JSONObject::set_value(std::string key, std::string value)
{
    key_to_value[key] = value;
}

void JSONObject::set_value(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    start++;
    std::string value = pick_val(source, start);
    key_to_value[key] = value;
}

void JSONObject::set_bool(std::string key, bool value)
{
    key_to_bool[key] = value;
}

void JSONObject::set_num(std::string key, long long value)
{
    key_to_num[key] = value;
}

void JSONObject::set_object(std::string key, JSONObject obj)
{
    key_to_object[key].clear();
    key_to_object[key] = obj;
}

void JSONObject::set_object(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_object[key].clear();
    key_to_object[key] = JSONObject(source, ++start);
}

void JSONObject::set_obj_list(std::string key, std::list<JSONObject> list)
{
    key_to_object_list[key].clear();
    for (auto i : list)
        key_to_object_list[key].push_back(i);
}

void JSONObject::set_obj_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_object_list[key].clear();
    key_to_object_list[key] = pick_obj_list(source, ++start);
}

void JSONObject::set_val_list(std::string key, std::list<std::string> list)
{
    key_to_value_list[key].clear();
    for (auto i : list)
        key_to_value_list[key].push_back(i);
}

void JSONObject::set_val_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_value_list[key] = pick_val_list(source, ++start);
}

void JSONObject::set_bool_list(std::string key, std::list<bool> list)
{
    key_to_boolean_list[key].clear();
    for (auto i : list)
        key_to_boolean_list[key].push_back(i);
}

void JSONObject::set_bool_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_boolean_list[key] =  pick_bool_list(source, ++start);
}

void JSONObject::set_num_list(std::string key, std::list<long long> list)
{
    key_to_num_list[key].clear();
    for (auto i : list)
        key_to_num_list[key].push_back(i);
}

void JSONObject::set_num_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_num_list[key] = pick_num_list(source, ++start);
}

void JSONObject::erase_value(std::string key)
{
    key_to_value.erase(key);
}

void JSONObject::erase_object(std::string key)
{
    key_to_object.erase(key);
}

void JSONObject::erase_num(std::string key)
{
    key_to_num.erase(key);
}

void JSONObject::erase_bool(std::string key)
{
    key_to_bool.erase(key);
}

void JSONObject::erase_val_list(std::string key)
{
    key_to_value_list.erase(key);
}

void JSONObject::erase_obj_list(std::string key)
{
    key_to_object_list.erase(key);
}

void JSONObject::erase_bool_list(std::string key)
{
    key_to_boolean_list.erase(key);
}

void JSONObject::erase_num_list(std::string key)
{
    key_to_num_list.erase(key);
}

void JSONObject::clear_bools()
{
    key_to_bool.clear();
}

void JSONObject::clear_nums()
{
    key_to_num.clear();
}

void JSONObject::clear_values()
{
    key_to_value.clear();
}

void JSONObject::clear_objects()
{
    key_to_object.clear();
}

void JSONObject::clear_val_lists()
{
    key_to_value_list.clear();
}

void JSONObject::clear_obj_lists()
{
    key_to_object_list.clear();
}

void JSONObject::clear_num_lists()
{
    key_to_num_list.clear();
}

void JSONObject::clear_bool_lists()
{
    key_to_boolean_list.clear();
}

void JSONObject::clear()
{
    key_to_bool.clear();
    key_to_num.clear();
    key_to_value.clear();
    key_to_object.clear();
    key_to_value_list.clear();
    key_to_object_list.clear();
    key_to_boolean_list.clear();
    key_to_num_list.clear();
}

void JSONObject::read(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    JSONObject tmp(source);
    key_to_value = tmp.get_name_to_value();
    key_to_object = tmp.get_name_to_object();
    key_to_value_list = tmp.get_name_to_values();
    key_to_object_list = tmp.get_name_to_objects();
}

void JSONObject::read_as_field(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    size_t start = 0;
    std::string key = pick_val(source, start);
    JSONObject tmp(source, ++start);
    key_to_object[key] = tmp;
}

void JSONObject::read_and_overwrite(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    JSONObject tmp(source);
    for (auto l : tmp.get_name_to_value())
        key_to_value[l.first] = l.second;

    for (auto l : tmp.get_name_to_object())
        key_to_object[l.first] = l.second;

    for (auto l : tmp.get_name_to_values())
        key_to_value_list[l.first] = l.second;

    for (auto l : tmp.get_name_to_objects())
        key_to_object_list[l.first] = l.second;

}

std::string JSONObject::format(std::string value)
{
    std::string res = "\"";
    for (int i = 0; i < value.size(); i++) {
        if (value[i] == '\"')
            res += "\\\"";
        else if (value[i] == '\'')
            res += "\\\'";
        else if (value[i] == '\t')
            res += "\\t";
        else if (value[i] == '\v')
            res += "\\v";
        else if (value[i] == '\b')
            res += "\\b";
        else if (value[i] == '\n')
            res += "\\n";
        else if (value[i] == '\\')
            res += "\\\\";
        else
            res += value[i];
    }

    res += "\"";
    return res;
}

std::string JSONObject::to_string(int tabs)
{
    std::string spacing(tabs, '\t');
    std::string res = "{\n";
    size_t counter = key_to_object.size() + key_to_value.size() + key_to_value_list.size() + key_to_object_list.size();
    for (auto l : key_to_value) {
        std::string key = spacing + '\t' + format(l.first);
        std::string val = format(l.second);
        res += key + ":" + val;
        if (counter > 1)
            res += ',';

        res += '\n';
        counter--;
    }

    for (auto l : key_to_value_list) {
        std::string key = spacing + '\t' + format(l.first);
        res += key + " : [";
        std::list<std::string>::iterator it = l.second.begin();
        if (it != l.second.end())
            res += format(*it);

        it++;
        while (it != l.second.end()) {
            res += ", " + format(*it);
        }

        counter--;
        res += "]";
        if (counter > 1)
            res += ',';
        res += '\n';
    }

    for (auto l : key_to_object) {

    }

    res += spacing + "}";
    return res;
}


