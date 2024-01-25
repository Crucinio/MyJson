#ifndef JSON_H
#define JSON_H

#include<unordered_map>
#include<string>
#include<list>

class JSONObject
{
private:
    std::unordered_map <std::string, std::string> key_to_value;

    std::unordered_map <std::string, long double> key_to_num;

    std::unordered_map <std::string, bool> key_to_bool;

    std::unordered_map <std::string, JSONObject> key_to_object;

    std::unordered_map <std::string, std::list<JSONObject> > key_to_object_list;

    std::unordered_map <std::string, std::list<std::string> > key_to_value_list;

    std::unordered_map <std::string, std::list<bool> > key_to_boolean_list;

    std::unordered_map <std::string, std::list<long double> > key_to_num_list;

    static int find_block_end_array(const std::string&, size_t); // this guy is almost useless...
public:

    ~JSONObject() = default;

    JSONObject() = default;

    // CONSTRUCTORS
    JSONObject(std::string& source);

    JSONObject(std::string& source, size_t& begin);

    // PROCESSING
    static std::string pick_val(std::string& source, size_t& begin);

    static long double pick_num(std::string& source, size_t& begin);

    static std::list<std::string> pick_val_list(std::string& source, size_t& begin);

    static std::list<JSONObject> pick_obj_list(std::string& source, size_t& begin);
    
    static std::list<bool> pick_bool_list(std::string& source, size_t& begin);

    static std::list<long double> pick_num_list(std::string& source, size_t& begin);

    static bool has_next_key(std::string& source, size_t& begin);
    
    static bool tryParseLDouble(std::string num);

    // FIELD ACCESS
    std::string& get_value(std::string key);

    bool get_bool(std::string key);

    long double get_num(std::string key);

    JSONObject& get_object(std::string key);

    std::list<JSONObject>& get_obj_list(std::string key);

    std::list<std::string>& get_val_list(std::string key);

    std::list<bool>& get_bool_list(std::string key);

    std::list<long double>& get_num_list(std::string key);

    // CONTAINERS ACCESS
    const std::unordered_map <std::string, JSONObject>& get_name_to_object() const;

    const std::unordered_map <std::string, std::list<JSONObject> >& get_name_to_objects() const;

    const std::unordered_map <std::string, std::list<std::string> >& get_name_to_values() const;

    const std::unordered_map <std::string, std::list<long double> >& get_name_to_nums() const;

    const std::unordered_map <std::string, std::list<bool> >& get_name_to_bools() const;

    const std::unordered_map <std::string, std::string>& get_name_to_value() const;

    const std::unordered_map <std::string, long double>& get_name_to_num() const;

    const std::unordered_map <std::string, bool>& get_name_to_bool() const;

    // CONTAINS && EMPTIES
    bool is_in_values(std::string key) const;

    bool is_in_objects(std::string key) const;

    bool is_in_bools(std::string key) const;

    bool is_in_nums(std::string key) const;

    bool is_in_val_lists(std::string key) const;

    bool is_in_obj_lists(std::string key) const;

    bool is_in_bool_lists(std::string key) const;

    bool is_in_num_lists(std::string key) const;

    bool empty_values() const;

    bool empty_objects() const;

    bool empty_nums() const;

    bool empty_bools() const;

    bool empty_val_lists() const;

    bool empty_obj_lists() const;

    bool empty_num_lists() const;

    bool empty_bool_lists() const;

    bool empty() const;

    // INSERTS/OVERWRITES
    void set_value(std::string key, std::string value);

    void set_value(std::string& source);

    void set_bool(std::string key, bool value);

    void set_num(std::string key, long double value);

    void set_object(std::string key, JSONObject obj);

    void set_object(std::string& source);

    void set_obj_list(std::string key, std::list<JSONObject> list);

    void set_obj_list(std::string& source);

    void set_val_list(std::string key, std::list<std::string> list);

    void set_val_list(std::string& source);

    void set_bool_list(std::string key, std::list<bool> list);

    void set_bool_list(std::string& source);

    void set_num_list(std::string key, std::list<long double> list);

    void set_num_list(std::string& source);

    // ERASALS && CLEANS
    void erase_value(std::string key);

    void erase_object(std::string key);

    void erase_num(std::string key);

    void erase_bool(std::string key);

    void erase_val_list(std::string key);

    void erase_obj_list(std::string key);

    void erase_bool_list(std::string key);

    void erase_num_list(std::string key);

    void clear_bools();

    void clear_nums();

    void clear_values();

    void clear_objects();

    void clear_val_lists();

    void clear_obj_lists();

    void clear_num_lists();

    void clear_bool_lists();

    void clear();

    // READ && WRITE
    static JSONObject read(std::string path);

    void read_as_field(std::string path);

    void read_and_overwrite(std::string path);

    //void read_as_binary(std::string path);

    void write(std::string file_path);

    //void write_as_binary(std::string file_name, std::string dir);

    // FORMATTING
    std::string format(std::string value);

    std::string to_string(int tabs);
};

#endif // JSON_H
