#include "CPyObjectHandler.h"

NotifyCategoryDef(CPyObjectHandler, "");

TypeHandle CPyObjectHandler::_type_handle;

CPyObjectHandler *CPyObjectHandler::_global_ptr = nullptr;

CPyObjectHandler::CPyObjectHandler() {
    object_count_limit = object_map.max_size(); // Default count limit.
}

CPyObjectHandler::~CPyObjectHandler() {
    object_map.clear();
}

void CPyObjectHandler::add_object(std::string object_name, PyObject *obj, unsigned int object_importance) {
    if (object_map.size() == object_count_limit || object_map.size() == object_map.max_size()) {
        pmap<std::string, std::pair<unsigned int, PyObject *>>::iterator x;
        for (x = object_map.begin(); x != object_map.end(); ++x) {
            if (x->second.first == EFFECT_OBJ) { //Check if it is infact a EFFECT_OBJ.
                remove_object(x->first); // Erase it from the map.
                break; // And break.
            }
        }
    }
    if (obj == nullptr || obj == NULL) {
        CPyObjectHandler_cat.error() << "Failed to add object because it was invalid!" << std::endl;
        return;
    }
    
    object_map[object_name] = std::make_pair(object_importance, obj);
}

void CPyObjectHandler::remove_object(std::string object_name) {
    if (object_map.find(object_name) == object_map.end()) {
        return;
    } else {
        pmap<std::string, std::pair<unsigned int, PyObject *>>::iterator it;
        it = object_map.find(object_name);
        if (it != object_map.end()) {
            // Found it? - Delete it!
            object_map.erase(it);
        } else {
            return;
        }
    }
}

void CPyObjectHandler::set_object_count_limit(unsigned int limit) {
    object_count_limit = limit;
}

void CPyObjectHandler::clear_list() {
    object_map.clear();
}

unsigned int CPyObjectHandler::get_object_count_limit() {
    return object_count_limit;
}

PyObject *CPyObjectHandler::get_py_object_dictonary() {
    PyObject *dict = PyDict_New();
    if (!dict) {
        throw logic_error("Unable to allocate memory for Python dictonary");
    }
    pmap<std::string, std::pair<unsigned int, PyObject *>>::iterator x;
    for (x = object_map.begin(); x != object_map.end(); ++x) {
        #if PY_MAJOR_VERSION >= 3 && SIZEOF_INT < SIZEOF_LONG
          PyObject *importance = PyLong_FromLong((long)x->second.first);
        #elif PY_MAJOR_VERSION >= 3
          PyObject *importance = PyLong_FromUnsignedLong((unsigned long)x->second.first);
        #elif SIZEOF_INT < SIZEOF_LONG
          PyObject *importance = PyInt_FromLong((long)x->second.first);
        #else
          PyObject *importance = (x->second.first > LONG_MAX)
            ? PyLong_FromUnsignedLong((unsigned long)x->second.first)
            : PyInt_FromLong((long)x->second.first);
        #endif
        if (!importance) {
            throw logic_error("Unable to allocate memory for Python integer");
        }
        #if PY_MAJOR_VERSION >= 3
          PyObject *key = PyUnicode_FromStringAndSize(x->first.data(), (Py_ssize_t)x->first.length());
        #else
          PyObject *key = PyString_FromStringAndSize(x->first.data(), (Py_ssize_t)x->first.length());
        #endif
        if (!key) {
            throw logic_error("Unable to allocate memory for Python string");
        }
        PyObject *tuple = PyTuple_New(2);
        if (!tuple) {
            throw logic_error("Unable to allocate memory for Python tuple");
        }
        PyTuple_SetItem(tuple, 0, importance);
        PyTuple_SetItem(tuple, 1, x->second.second);
        PyDict_SetItem(dict, key, tuple);
    }
    return dict;
}

CPyObjectHandler *CPyObjectHandler::get_global_ptr() {
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new CPyObjectHandler;
    }
    return _global_ptr;
}