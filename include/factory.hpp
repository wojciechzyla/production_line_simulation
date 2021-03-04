//
// Created by Wojciech Żyła on 29/12/2020.
//

#ifndef NET_SIMULATION_FACTORY_HPP
#define NET_SIMULATION_FACTORY_HPP
#include <iostream>
#include <vector>
#include "types.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"
#include <sstream>
#include <istream>


enum class NodeColor{
    UNVISITED, VISITED, VERIFIED
};

template<class Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    const_iterator cend() const {return collection_.cend();}
    const_iterator cbegin() const {return collection_.cbegin();}
    iterator end(){ return collection_.end();}
    const_iterator end() const{return collection_.cend();}
    iterator begin(){ return collection_.begin();}
    const_iterator begin() const{return collection_.cbegin();}

    void add(Node&& n){collection_.emplace_back(std::move(n));}
    iterator find_by_id(ElementID id) {return std::find_if(collection_.begin(),collection_.end(),[id](const Node& n){return (n.get_id() == id);});}
    const_iterator find_by_id(ElementID id) const {return std::find_if(collection_.begin(),collection_.end(),[id](const Node& n){return (n.get_id() == id);});}
    void remove_by_id(ElementID id);
private:
    container_t collection_;
};

template <class Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    iterator it = find_by_id(id);
    if (it != collection_.end()) {
        collection_.erase(it);
    }
}


class Factory{
public:
    void add_ramp(Ramp&& r) {ramps_.add(std::move(r));}
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramps_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps_.cend();}

    void add_worker(Worker&& w) {workers_.add(std::move(w));}
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return workers_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const{return workers_.cend();}

    void add_storehouse(Storehouse&& s) {storehouses_.add(std::move(s));}
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouses_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses_.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return storehouses_.cend();}

    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
    bool has_reachable_storehouse(PackageSender *p_sender, std::map<PackageSender*, NodeColor> s_color);
};

enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ElementType element_type_;
    std::map<std::string, std::string> parameters_;
};

// File loading
template <class Node>
std::vector<std::list<int>> sorted_workers_stores(Node source);
extern ElementType string_to_element_type(std::string type_string);
extern ParsedLineData parse_line(std::string line);
extern void save_factory_structure(Factory& factory, std::ostream& os);
template <class Node>
extern void save_links(std::ostream& os, Node source);
extern Factory load_factory_structure(std::istream& is);
extern void add_links(std::map<std::string, std::string> link_data, Factory* factory);
extern PackageQueueType string_to_queue_type(std::string queue_string);
#endif //NET_SIMULATION_FACTORY_HPP
