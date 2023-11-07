
#ifndef JSON_H
#define JSON_H


#include<unordered_map>
#include<string>
#include<list>

class JSONObject
{
private:
    // key - valuse
    std::unordered_map <std::string, std::string> name_to_value;

    // key - object
    std::unordered_map <std::string, JSONObject> name_to_object;

    // key - list<object>
    std::unordered_map <std::string, std::list<JSONObject> > name_to_objects;

    // key - list<value>
    std::unordered_map <std::string, std::list<std::string> > name_to_values;
    
    // assisting parsing function, return index of OBJECT string block end
    int find_block_end_obj(const std::string&, int) const;

    // assisting parsing function, return index of ARRAY string block end
    int find_block_end_array(const std::string&, int) const;

    // picks and processes a value beginning from /begin/
    std::string pick_val(std::string& source, int begin);

    // picks and processes an array of values beginning from /begin/
    std::list<std::string> pick_val_array(std::string& source, int begin);

    // picks and processes an array of objects from /begin/
    std::list<JSONObject> pick_obj_array(std::string& source, int begin);

    int valid_value(std::string& source, int begin);

    bool valid_array(std::string& source, int begin, int end);

    bool valid_obj_array(std::string& source, int begin, int end);
public:

    ~JSONObject() = default;
    JSONObject() = default;

    // String constructor
    explicit JSONObject(std::string& source);

    explicit JSONObject(std::string& source, int begin);

    // Copying
    JSONObject(const JSONObject&);

    // returns object in from name_to_object
    const JSONObject& get_object(const std::string& key) const;

    // returns vector<object> in from name_to_object
    const std::list<JSONObject>& get_objects(const std::string& key) const;

    // returns object in from name_to_object
    const std::list<std::string>& get_values(const std::string& key) const;

    // returns value from name_to_value
    const std::string& get_value(const std::string& key) const;

    const std::unordered_map <std::string, JSONObject>& get_name_to_object() const;
    const std::unordered_map <std::string, std::list<JSONObject> >& get_name_to_objects() const;
    const std::unordered_map <std::string, std::list<std::string> >& get_name_to_values() const;
    const std::unordered_map <std::string, std::string>& get_name_to_value() const;

    // methods that return whether key - X pair exists
    // X = value
    bool is_in_values(const std::string& key) const;

    // X = object
    bool is_in_objects(const std::string& key) const;

    // X = list of values
    bool is_in_arrays(const std::string& key) const;

    // X = list of objects
    bool is_in_object_arrays(const std::string& key) const;

    // values
    void insert_value(const std::string& key, std::string value);

    // objects
    void insert_object(const std::string& key, JSONObject& obj);

    // arrays of objects
    void insert_object_array(const std::string& key, const std::string& array);

    // retrun whether we can make a valid object from source or not
    bool is_valid(std::string& source, int begin);
};





#endif // JSON_H
